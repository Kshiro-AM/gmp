#include "fd_interpolate.h"

#include "CoreMath.h"

void fd_interpolate(
	const int nx,
	const int ny,
	const int nz,
	const double h,
	const Eigen::RowVector3d& corner,
	const Eigen::MatrixXd& P,
	Eigen::SparseMatrix<double>& W)
{
	std::vector<Eigen::Triplet<double>> W_list(P.rows() * 8);
	auto currentW = W_list.begin();
	W.resize(P.rows(), nx * ny * nz);
	for (int i = 0; i < P.rows(); i++) {
		const Eigen::RowVector3d pointPosition = P.row(i);
		int ix = floor((pointPosition.x() - corner.x()) / h);
		int iy = floor((pointPosition.y() - corner.y()) / h);
		int iz = floor((pointPosition.z() - corner.z()) / h);
		double x0 = ix * h + corner.x(); double y0 = iy * h + corner.y(); double z0 = iz * h + corner.z();
		double x = pointPosition.x(); double y = pointPosition.y(); double z = pointPosition.z();
		double xd = (x - x0) / h; double yd = (y - y0) / h; double zd = (z - z0) / h;

		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix, iy, iz, nx, ny), (1 - xd) * (1 - yd) * (1 - zd));	currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix + 1, iy, iz, nx, ny), xd * (1 - yd) * (1 - zd));	currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix, iy + 1, iz, nx, ny), (1 - xd) * yd * (1 - zd));	currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix, iy, iz + 1, nx, ny), (1 - xd) * (1 - yd) * zd);	currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix + 1, iy, iz + 1, nx, ny), xd * (1 - yd) * zd);		currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix, iy + 1, iz + 1, nx, ny), (1 - xd) * yd * zd);		currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix + 1, iy + 1, iz, nx, ny), xd * yd * (1 - zd));		currentW++;
		*currentW = Eigen::Triplet<double>(i, WPOINT_INDEX(ix + 1, iy + 1, iz + 1, nx, ny), xd * yd * zd);		currentW++;
	}
	W.setFromTriplets(W_list.begin(), W_list.end());
}
