#include "edges.h"
#include "euler_characteristic.h"
#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <igl/edges.h>
#include <igl/euler_characteristic.h>

int main(int argc, char* argv[])
{
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;
	// Load in a mesh
	igl::read_triangle_mesh(argc > 1 ? argv[1] : "asset/bumpy.off", V, F);

	Eigen::MatrixXi E_m = gmp::edges(F);
	int Chi_m = gmp::euler_characteristic(F);

	Eigen::MatrixXi E_i;
	igl::edges(F, E_i);
	int Chi_i = igl::euler_characteristic(F);

	std::cout << "Edge list E_m is " << E_m.rows() << "x" << E_i.cols() << std::endl;
	std::cout << "Edge list E_i is " << E_i.rows() << "x" << E_i.cols() << std::endl;
	std::cout << "Euler Characteristic m: " << Chi_m << std::endl;
	std::cout << "Euler Characteristic i: " << Chi_i << std::endl;

	// Create a libigl Viewer object 
	igl::opengl::glfw::Viewer viewer;
	// Set the vertices and faces for the viewer
	viewer.data().set_mesh(V, F);
	// Launch a viewer instance
	viewer.launch();
	return 0;
}