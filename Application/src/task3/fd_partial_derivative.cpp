#include "fd_partial_derivative.h"

#include "CoreMath.h"

void fd_partial_derivative(
	const int nx,
	const int ny,
	const int nz,
	const double h,
	const int dir,
	Eigen::SparseMatrix<double>& D)
{
	int nxd = nx; int nyd = ny; int nzd = nz;
	if (dir == 0) // direction x
		nxd--;
	if (dir == 1) // direction y
		nyd--;
	if (dir == 2) // direction z
		nzd--;

	D.resize(nxd * nyd * nzd, nx * ny * nz);
	std::vector<Eigen::Triplet<double>> D_list(nxd * nyd * nzd * 2);
	auto currentD = D_list.begin();
	for (int i = 0; i < nxd; i++) {
		for (int j = 0; j < nyd; j++) {
			for (int k = 0; k < nzd; k++) {
				*currentD = Eigen::Triplet<double>(MATRIX_POINT_INDEX(i, j, k, nxd, nyd), MATRIX_POINT_INDEX(i, j, k, nx, ny), -1); currentD++;
				if (dir == 0) // direction x
					*currentD = Eigen::Triplet<double>(MATRIX_POINT_INDEX(i, j, k, nxd, nyd), MATRIX_POINT_INDEX(i + 1, j, k, nx, ny), 1);
				if (dir == 1) // direction y
					*currentD = Eigen::Triplet<double>(MATRIX_POINT_INDEX(i, j, k, nxd, nyd), MATRIX_POINT_INDEX(i, j + 1, k, nx, ny), 1);
				if (dir == 2) // direction z
					*currentD = Eigen::Triplet<double>(MATRIX_POINT_INDEX(i, j, k, nxd, nyd), MATRIX_POINT_INDEX(i, j, k + 1, nx, ny), 1);
				currentD++;
			}
		}
	}
	D.setFromTriplets(D_list.begin(), D_list.end());
}
