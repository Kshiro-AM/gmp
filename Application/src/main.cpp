#include <iostream>
#include <Eigen/Core>

#include <igl/readOFF.h>

int main() {
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;

	igl::readOFF("asset/bumpy.off", V, F);
	std::cout << V << std::endl;

	return 0;
}