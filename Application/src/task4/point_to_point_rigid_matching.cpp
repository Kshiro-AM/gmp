#include "point_to_point_rigid_matching.h"

#include "closest_rotation.h"

void point_to_point_rigid_matching(
	const Eigen::MatrixXd& X,
	const Eigen::MatrixXd& P,
	Eigen::Matrix3d& R,
	Eigen::RowVector3d& t)
{
	// Gravity center of point cloud
	Eigen::RowVector3d XG = X.colwise().mean();
	Eigen::RowVector3d PG = P.colwise().mean();

	// Points relative to gravity
	Eigen::MatrixXd X_ = X.rowwise() - XG;
	Eigen::MatrixXd P_ = P.rowwise() - PG;

	// rigid transform
	Eigen::Matrix3d M = X_.transpose() * P_;
	closest_rotation(M, R);

	t = PG.transpose() - R * XG.transpose();

}

