#include "angle_defect.h"

#include "internal_angles.h"

#include "igl/squared_edge_lengths.h"
#include "igl/vertex_triangle_adjacency.h"

#include <iostream>

void angle_defect(
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	Eigen::VectorXd& D)
{
	D = Eigen::VectorXd::Zero(V.rows());

	Eigen::MatrixXd l_sqr;
	igl::squared_edge_lengths(V, F, l_sqr);
	Eigen::MatrixXd A;
	internal_angles(l_sqr, A);

	std::vector<std::vector<int>> VF, VI;
	igl::vertex_triangle_adjacency(F.maxCoeff() + 1, F, VF, VI);

	for (int i = 0; i < V.rows(); i++) {
		double sum = 0;
		auto& faces = VF[i];
		auto& indices = VI[i];
		for (int j = 0; j < faces.size(); j++) {
			sum += A(faces[j], indices[j]);
		}
		D(i) = 2 * 3.14 - sum;
	}
}
