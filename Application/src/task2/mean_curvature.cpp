#include "mean_curvature.h"

#include "igl/pinv.h"
#include "igl/adjacency_list.h"

#include <iostream>

void mean_curvature(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	Eigen::VectorXd& H)
{
	// Replace with your code
	H = Eigen::VectorXd::Zero(V.rows());

	std::vector<std::vector<int>> A;
	igl::adjacency_list(F, A);

	for (int i = 0; i < A.size(); i++) {
		// get P matirx
		auto& indices = A[i];
		Eigen::MatrixXd P(indices.size(), 3);
		for (int j = 0; j < indices.size(); j++) {
			P.row(j) = V.row(indices[j]) - V.row(i);
		}

		// get EigenVector
		auto PTP = P.transpose() * P;
		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> solver(PTP);
		auto eigenVals = solver.eigenvalues();
		auto eigenVectors = solver.eigenvectors();

		// get uvwCoeff
		Eigen::MatrixXd points(indices.size(), 5);
		Eigen::VectorXd W(indices.size());
		for (int j = 0; j < indices.size(); j++) {
			Eigen::VectorXd uvw = eigenVectors.transpose().colPivHouseholderQr().solve(P.row(j).transpose());
			auto u = uvw.coeff(0); auto v = uvw.coeff(1); auto w = uvw.coeff(2);
			
			points.row(j) << u, v, pow(u, 2), u * v, pow(v, 2);
			W(j) = w;
		}
		Eigen::MatrixXd plus(5, 5);
		Eigen::MatrixXd ptp = points.transpose() * points;
		igl::pinv(ptp, plus);
		auto A = plus * points.transpose() * W;
		
		// calculate S
		Eigen::Matrix2d S_1, S_2;
		double denominator = sqrt(pow(A(0), 2) + pow(A(1), 2) + 1);
		S_1 << 2 * A(2), A(3),
			A(3), 2 * A(4);
		S_1 = S_1 * (1 / denominator);
		S_2 << 1 + pow(A(0), 2), A(0)* A(1),
			A(0)* A(1), 1 + pow(A(1), 2);
		S_2 = S_2.reverse();
		Eigen::Matrix2d S = S_1 * S_2 * -1;
		
		// get k1, k2
		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> Ssolver(S);
		auto K = Ssolver.eigenvalues();
		auto dirctionK = Ssolver.eigenvectors();
		H(i) = (K(0) + K(1)) / 2;
	}

}
