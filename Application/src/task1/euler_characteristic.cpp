#include "euler_characteristic.h"

#include "edges.h"

namespace gmp {

	int euler_characteristic(const Eigen::MatrixXi& F)
	{
		int Chi = 0;

		int vNum = F.maxCoeff() + 1;
		auto E = gmp::edges(F);
		int eNum = E.rows();
		int fNum = F.rows();

		Chi = vNum - eNum + fNum;
		return Chi;
	}
}
