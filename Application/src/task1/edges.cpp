#include "edges.h"

#include <vector>
#include <algorithm>

namespace gmp {
	// From a list of triangle indices into some vertex set V, determine the
	// set of unique undirected edges.
	//
	// Inputs:
	//   F  #F by 3 list of triangles indices into some vertex set V
	// Returns E  #E by 2 list of undirected edge indices into V
	Eigen::MatrixXi edges(const Eigen::MatrixXi& F)
	{
		Eigen::MatrixXi E;

		uint32_t max = F.maxCoeff() + 1;
		std::vector<std::vector<bool>> edge(max + 1, std::vector<bool>(max + 1, false));
		uint32_t edgeCount = 0;
		for (int i = 0; i < F.rows(); i++) {
			std::vector<int> row(F.cols());
			for (int j = 0; j < F.cols(); j++) {
				row[j] = F(i, j);
			}
			// lost order
			std::sort(row.begin(), row.end());
			if (!edge[row[0]][row[1]]) { edgeCount++; edge[row[0]][row[1]] = true; }
			if (!edge[row[1]][row[2]]) { edgeCount++; edge[row[1]][row[2]] = true; }
			if (!edge[row[0]][row[2]]) { edgeCount++; edge[row[0]][row[2]] = true; }
		}

		E.resize(edgeCount, 2);
		uint32_t edgeIndex = 0;
		for (int i = 0; i < edge.size(); i++) {
			for (int j = 0; j < edge[i].size(); j++) {
				if (edge[i][j]) {
					E(edgeIndex, 0) = i;
					E(edgeIndex, 1) = j;
				}
			}
		}

		return E;
	}
}