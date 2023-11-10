#include "closest_rotation.h"

#include "Eigen/SVD"

void closest_rotation(
	const Eigen::Matrix3d& M,
	Eigen::Matrix3d& R)
{
	// Replace with your code
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeThinU | Eigen::ComputeThinV);

	Eigen::Matrix3d U = svd.matrixU();
	Eigen::Matrix3d V = svd.matrixV();
	Eigen::Matrix3d omega;
	Eigen::Matrix3d UVT = U * V.transpose();
	omega << 1, 0, 0,
			 0, 1, 0,
			 0, 0, UVT.determinant();
	R = U * omega * V.transpose();
}
