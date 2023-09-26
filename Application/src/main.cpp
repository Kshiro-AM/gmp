#include <iostream>
#include <Eigen/Core>

#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>


int main() {
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;

	igl::readOFF("asset/bumpy.off", V, F);
	
	igl::opengl::glfw::Viewer viewer;
	viewer.data().set_mesh(V, F);
	viewer.launch();

	return 0;
}