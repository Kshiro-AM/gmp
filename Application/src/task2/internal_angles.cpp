#include "internal_angles.h"

#include <iostream>
#include <cmath>

void internal_angles(
	const Eigen::MatrixXd& l_sqr,
	Eigen::MatrixXd& A)
{
	// Add with your code
	A.resize(l_sqr.rows(), l_sqr.cols());
	for (int i = 0; i < l_sqr.rows(); i++) {
		A(i, 0) = std::acos((l_sqr(i, 1) + l_sqr(i, 2) - l_sqr(i, 0)) / sqrt(4 * l_sqr(i, 1) * l_sqr(i, 2)));
		A(i, 1) = std::acos((l_sqr(i, 0) + l_sqr(i, 2) - l_sqr(i, 1)) / sqrt(4 * l_sqr(i, 0) * l_sqr(i, 2)));
		A(i, 2) = std::acos((l_sqr(i, 1) + l_sqr(i, 0) - l_sqr(i, 2)) / sqrt(4 * l_sqr(i, 1) * l_sqr(i, 0)));
	}
}
