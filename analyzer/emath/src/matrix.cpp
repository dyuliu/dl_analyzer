
#include <matrix.h>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <cassert>

namespace emath {

	void mul(const std::vector<DType> &x, const std::vector<DType> &y) {
		NOT_REALIZE;
	}

	DType dot(const std::vector<DType> &x, const std::vector<DType> &y) {
		
		assert(x.size() == y.size());

		DType res = 0;
		size_t len = x.size();

		for (int i = 0; i < len; i++)
			res += x[i] * y[i];

		return res;
	}

}