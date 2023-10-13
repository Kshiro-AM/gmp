#include "mean_curvature.h"

#include "igl/cotmatrix.h"
#include "igl/massmatrix.h"
#include "igl/invert_diag.h"

#include <iostream>
#include <optional>

void mean_curvature(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	Eigen::VectorXd& H)
{
	// Replace with your code
	H = Eigen::VectorXd::Zero(V.rows());

	Eigen::SparseMatrix<double> cotMatrix;
	igl::cotmatrix(V, F, cotMatrix);
	Eigen::SparseMatrix<double> massMatrix;
	igl::massmatrix(V, F, igl::MassMatrixType::MASSMATRIX_TYPE_VORONOI, massMatrix);
	Eigen::SparseMatrix<double> Minv;
	igl::invert_diag(massMatrix, Minv);
	Eigen::MatrixXd Hn = Minv * cotMatrix * V;
	Hn = Minv * cotMatrix * V;

	H = Hn.rowwise().norm();
}
