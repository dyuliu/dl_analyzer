
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

	// dst = dst + src;
	void add(std::vector<DType> &dst, const std::vector<DType> &src) {

		assert(dst.size() == src.size());

		size_t len = dst.size();

		for (int i = 0; i < len; i++) {
			dst[i] = dst[i] + src[i];
		}
	}

	// x * scale for each element
	void element_mul(std::vector<DType> &x, DType y) {
		
		size_t len = x.size();

		for (int i = 0; i < len; i++) {
			x[i] = x[i] * y;
		}

	}

}