#include "principal_curvatures.h"

#include "igl/adjacency_matrix.h"
#include "igl/pinv.h"
#include "igl/sort.h"

#include <iostream>

void principal_curvatures(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	Eigen::MatrixXd& D1,
	Eigen::MatrixXd& D2,
	Eigen::VectorXd& K1,
	Eigen::VectorXd& K2)
{
	// Replace with your code
	K1 = Eigen::VectorXd::Zero(V.rows());
	K2 = Eigen::VectorXd::Zero(V.rows());
	D1 = Eigen::MatrixXd::Zero(V.rows(), 3);
	D2 = Eigen::MatrixXd::Zero(V.rows(), 3);

	Eigen::SparseMatrix<int> A_1;
	igl::adjacency_matrix(F, A_1);
	Eigen::SparseMatrix<int> A_2 = A_1 * A_1;

	for (int i = 0; i < V.rows(); i++) {
		int num_a1 = A_1.col(i).nonZeros();
		int num_a2 = A_2.col(i).nonZeros() - 1;

		Eigen::MatrixXd P(num_a1 + num_a2, 3);
		int j; 
		j = 0;
		for (Eigen::SparseMatrix<int>::InnerIterator it(A_1, i); it; ++it, j++) {
			P.row(j) = V.row(it.row()) - V.row(i);
		}
		j = 0;
		for (Eigen::SparseMatrix<int>::InnerIterator it(A_2, i); it; ++it, j++) {
			P.row(j) = V.row(it.row()) - V.row(i);
		}

		Eigen::JacobiSVD<Eigen::MatrixXd> svd;
		svd.compute(P, Eigen::ComputeThinU | Eigen::ComputeThinV);
		svd.computeV();
		Eigen::MatrixXd W = svd.matrixV();

		Eigen::MatrixXd S = P * W.leftCols(2);
		Eigen::VectorXd B = P * W.col(2);

		Eigen::MatrixXd T(P.rows(), 5);
		for (int j = 0; j < S.rows(); j++) {
			T(j, 0) = S(j, 0);
			T(j, 1) = S(j, 1);
			T(j, 2) = S(j, 0) * S(j, 0);
			T(j, 3) = S(j, 0) * S(j, 1);
			T(j, 4) = S(j, 1) * S(j, 1);
		}

		Eigen::MatrixXd T_pinv;
		igl::pinv(T, T_pinv);

		Eigen::VectorXd LS = T_pinv * B;

		double E = 1 + LS(0) * LS(0);
		double F = LS(0) * LS(1);
		double G = 1 + LS(1) * LS(1);
		double e = 2 * LS(2) / sqrt(LS(0) * LS(0) + LS(1) * LS(1) + 1);
		double f = LS(3) / sqrt(LS(0) * LS(0) + LS(1) * LS(1) + 1);
		double g = 2 * LS(4) / sqrt(LS(0) * LS(0) + LS(1) * LS(1) + 1);

		Eigen::MatrixXd S_1(2, 2);
		Eigen::MatrixXd S_2(2, 2);

		S_1 << e, f, f, g;
		S_2 << E, F, F, G;
		Eigen::MatrixXd S_2pinv;
		igl::pinv(S_2, S_2pinv);
		Eigen::MatrixXd Sop = -S_1 * S_2pinv;

		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(Sop);
		if (solver.info() != Eigen::Success) {
			K1(i) = 0;
			K2(i) = 0;
			D1.row(i) = Eigen::VectorXd(3).transpose();
			D2.row(i) = Eigen::VectorXd(3).transpose();
			continue;
		}
		Eigen::VectorXd eigen_vals = solver.eigenvalues().real();

		std::vector<double> vals(eigen_vals.rows());
		std::vector<int> indices(eigen_vals.rows());
		for (int j = 0; j < eigen_vals.rows(); j++) {
			vals[j] = eigen_vals[j];
			indices[j] = j;
		}
		std::sort(indices.begin(), indices.end(), [&](int a, int b) {
			return vals[a] > vals[b];
			});
		//igl::sort(solver.eigenvalues().real(), 1, false, eigen_vals, eigen_indices);

		K1(i) = eigen_vals(0);
		K2(i) = eigen_vals(1);

		Eigen::MatrixXcd D = solver.eigenvectors();
		Eigen::VectorXd d1 = D.col(indices[0]).real();
		Eigen::VectorXd d2 = D.col(indices[1]).real();

		D1.row(i) = W.leftCols(2) * d1;
		D1.row(2) = W.leftCols(2) * d2;
	}
}
