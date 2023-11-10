#include "random_points_on_mesh.h"

#include "igl/doublearea.h"
#include "igl/cat.h"

#include <iostream>

static double random() {
	return rand() / double(RAND_MAX);
}

/*
	V is 3 Points for one triangle
	X is output a sample point on trianle
*/
static void SampleOnTriangle(const Eigen::Matrix3d& V, Eigen::Vector3d& X) {
	double a1 = random();
	double a2 = random();
	if (a1 + a2 > 1) {
		a1 = 1 - a1;
		a2 = 1 - a2;
	}

	X = V.row(0) + a1 * (V.row(1) - V.row(0)) + a2 * (V.row(2) - V.row(0));
}

void random_points_on_mesh(
	const int n,
	const Eigen::MatrixXd& V,
	const Eigen::MatrixXi& F,
	Eigen::MatrixXd& X)
{
	Eigen::MatrixXd area;
	igl::doublearea(V, F, area);

	// Build Vecotr C
	double Ai = 0;
	double Ax = area.sum();
	std::vector<double> C(area.rows());
	for (int i = 0; i < area.rows(); i++) {
		Ai += area(i) / Ax;
		C[i] = Ai;
	}

	// Sample on triangles
	X.resize(n, 3);
	for (int i = 0; i < n; i++) {
		auto it = std::lower_bound(C.begin(), C.end(), random());
		int index = it - C.begin();
		Eigen::Vector3i Fi = F.row(index);

		Eigen::Matrix3d Vi;
		Vi << V.row(Fi(0)), V.row(Fi(1)), V.row(Fi(2));

		Eigen::Vector3d Xi;
		SampleOnTriangle(Vi, Xi);
		X.row(i) = Xi;
	}
}

