#include "point_mesh_distance.h"

#include "point_triangle_distance.h"

#include "igl/per_face_normals.h"

void point_mesh_distance(
	const Eigen::MatrixXd& X,
	const Eigen::MatrixXd& VY,
	const Eigen::MatrixXi& FY,
	Eigen::VectorXd& D,
	Eigen::MatrixXd& P,
	Eigen::MatrixXd& N)
{
	// init
	D.resize(X.rows());
	P.resizeLike(X);
	N.resizeLike(X);

	// normal
	Eigen::MatrixXd normals;
	igl::per_face_normals(VY, FY, Eigen::Vector3d(1, 1, 1).normalized(), normals);

	// loop for point * mesh
	for (int i = 0; i < X.rows(); i++) {
		Eigen::Vector3d point = X.row(i);

		int faceIndex = 0;
		double minDistance = 99999999;
		Eigen::Vector3d pointOnMesh;
		for (int j = 0; j < FY.rows(); j++) {
			Eigen::Vector3i indices = FY.row(j);
			Eigen::RowVector3d pointOnTriangle;
			double distance;
			point_triangle_distance(point, VY.row(indices(0)), VY.row(indices(1)), VY.row(indices(2)), distance, pointOnTriangle);
			if (distance < minDistance) {
				minDistance = distance;
				faceIndex = j;
				pointOnMesh = pointOnTriangle;
			}
		}

		P.row(i) = pointOnMesh;
		D(i) = minDistance;
		N.row(i) = normals.row(faceIndex);
	}
}
