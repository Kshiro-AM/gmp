#include "poisson_surface_reconstruction.h"

#include "fd_interpolate.h"
#include "fd_grad.h"

#include "igl/marching_cubes.h"
#include "igl/cat.h"

#include <algorithm>

void poisson_surface_reconstruction(
	const Eigen::MatrixXd& P,
	const Eigen::MatrixXd& N,
	Eigen::MatrixXd& V,
	Eigen::MatrixXi& F)
{
	////////////////////////////////////////////////////////////////////////////
	// Construct FD grid, CONGRATULATIONS! You get this for free!
	////////////////////////////////////////////////////////////////////////////
	// number of input points
	const int n = P.rows();
	// Grid dimensions
	int nx, ny, nz;
	// Maximum extent (side length of bounding box) of points
	double max_extent =
		(P.colwise().maxCoeff() - P.colwise().minCoeff()).maxCoeff();
	// padding: number of cells beyond bounding box of input points
	const double pad = 8;
	// choose grid spacing (h) so that shortest side gets 30+2*pad samples
	double h = max_extent / double(30 + 2 * pad);
	// Place bottom-left-front corner of grid at minimum of points minus padding
	Eigen::RowVector3d corner = P.colwise().minCoeff().array() - pad * h;
	// Grid dimensions should be at least 3 
	nx = std::max((P.col(0).maxCoeff() - P.col(0).minCoeff() + (2. * pad) * h) / h, 3.);
	ny = std::max((P.col(1).maxCoeff() - P.col(1).minCoeff() + (2. * pad) * h) / h, 3.);
	nz = std::max((P.col(2).maxCoeff() - P.col(2).minCoeff() + (2. * pad) * h) / h, 3.);
	// Compute positions of grid nodes
	Eigen::MatrixXd x(nx * ny * nz, 3);
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				// Convert subscript to index
				const auto ind = i + nx * (j + k * ny);
				x.row(ind) = corner + h * Eigen::RowVector3d(i, j, k);
			}
		}
	}
	Eigen::VectorXd g = Eigen::VectorXd::Zero(nx * ny * nz);

	////////////////////////////////////////////////////////////////////////////
	// Add your code here
	////////////////////////////////////////////////////////////////////////////

	Eigen::RowVector3d xCorner = corner + Eigen::RowVector3d(h / 2, 0, 0);
	Eigen::RowVector3d yCorner = corner + Eigen::RowVector3d(0, h / 2, 0);
	Eigen::RowVector3d zCorner = corner + Eigen::RowVector3d(0, 0, h / 2);
	Eigen::SparseMatrix<double> Wx, Wy, Wz;
	fd_interpolate(nx - 1, ny, nz, h, xCorner, P, Wx);
	fd_interpolate(nx, ny - 1, nz, h, yCorner, P, Wy);
	fd_interpolate(nx, ny, nz - 1, h, zCorner, P, Wz);
	
	Eigen::VectorXd vx, vy, vz;
	Eigen::VectorXd vxy, v;
	vx = Wx.transpose() * N.col(0);
	vy = Wy.transpose() * N.col(1);
	vz = Wz.transpose() * N.col(2);
	igl::cat(1, vx, vy, vxy);
	igl::cat(1, vxy, vz, v);

	Eigen::SparseMatrix<double> G;
	fd_grad(nx, ny, nz, h, G);

	Eigen::BiCGSTAB<Eigen::SparseMatrix<double>> solver(G.transpose() * G);
	g = solver.solve(G.transpose() * v);

	Eigen::SparseMatrix<double> W;
	fd_interpolate(nx, ny, nz, h, corner, P, W);
	double sigma = (W * g).sum() / n;
	////////////////////////////////////////////////////////////////////////////
	// Run black box algorithm to compute mesh from implicit function: this
	// function always extracts g=0, so "pre-shift" your g values by -sigma
	////////////////////////////////////////////////////////////////////////////
	// TODO: Check if marching_cubes work without copyleft
	igl::marching_cubes(g, x, nx, ny, nz, sigma, V, F);
}

