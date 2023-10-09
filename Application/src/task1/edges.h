#pragma once

#include <Eigen/Core>

namespace gmp {
	// From a list of triangle indices into some vertex set V, determine the
	// set of unique undirected edges.
	//
	// Inputs:
	//   F  #F by 3 list of triangles indices into some vertex set V
	// Returns E  #E by 2 list of undirected edge indices into V
	Eigen::MatrixXi edges(const Eigen::MatrixXi& F);
}