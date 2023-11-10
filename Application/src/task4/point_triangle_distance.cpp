#include "point_triangle_distance.h"

#include <iostream>

void point_triangle_distance(
	const Eigen::RowVector3d& x,
	const Eigen::RowVector3d& a,
	const Eigen::RowVector3d& b,
	const Eigen::RowVector3d& c,
	double& d,
	Eigen::RowVector3d& p)
{
	// Replace with your code
	d = 0;
	p = a;

	Eigen::RowVector3d v1 = b - a;
	Eigen::RowVector3d v2 = c - a;
	Eigen::RowVector3d x_v = x - a;

	// normal
	Eigen::RowVector3d n = v1.cross(v2);
	n.normalize();

	// projection on plane
	double projectDistance = n.dot(x_v);
	Eigen::RowVector3d projectX = x_v - projectDistance * n; // projectX is x projection on triangle plane, based on a

	// projectX coordinate in trangle space
	double denominator = ((v1.dot(v2)) * (v1.dot(v2)) - (v1.dot(v1)) * (v2.dot(v2)));
	double s = v1.dot(v2) * projectX.dot(v2) - v2.dot(v2) * projectX.dot(v1) / denominator;
	double t = v1.dot(v2) * projectX.dot(v1) - v1.dot(v1) * projectX.dot(v2) / denominator;

	if (s >= 0 && t >= 0 && s + t <= 1) { // projection in triangle
		p = a + s * v1 + t * v2;
	}
	else if (s + t > 1) {
		if (s < 1) p = b;									// projection beyond b
		else if (t < 1) p = c;								// projection beyond c
		else p = a + s * v1 / (s + t) + t * v2 / (s + t);	// projection on bc
	}
	else if (t < 1) {
		if (s < 1) p = a;
		else { // projcetion on ab
			Eigen::RowVector3d v1 = a - c;
			Eigen::RowVector3d v2 = b - c;
			double denominator = ((v1.dot(v2)) * (v1.dot(v2)) - (v1.dot(v1)) * (v2.dot(v2)));
			double s = v1.dot(v2) * projectX.dot(v2) - v2.dot(v2) * projectX.dot(v1) / denominator;
			double t = v1.dot(v2) * projectX.dot(v1) - v1.dot(v1) * projectX.dot(v2) / denominator;
			p = c + s * v1 / (s + t) + t * v2 / (s + t);
		}
	}
	else { // projcetion on ac
		Eigen::RowVector3d v1 = a - b;
		Eigen::RowVector3d v2 = c - b;
		double denominator = ((v1.dot(v2)) * (v1.dot(v2)) - (v1.dot(v1)) * (v2.dot(v2)));
		double s = v1.dot(v2) * projectX.dot(v2) - v2.dot(v2) * projectX.dot(v1) / denominator;
		double t = v1.dot(v2) * projectX.dot(v1) - v1.dot(v1) * projectX.dot(v2) / denominator;
		p = b + s * v1 / (s + t) + t * v2 / (s + t);
	}

	d = (p - x).norm();
}
