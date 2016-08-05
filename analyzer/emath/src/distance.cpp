
#include <cmath>
#include <algorithm>
#include <numeric>

#include <matrix.h>
#include <statistic.h>
#include <distance.h>

namespace emath {

	DType distance(const std::vector<DType> &x, const std::vector<DType> &y, distance_method method) {

		CHECK_EQ(x.size(), y.size());
		CHECK_NE(x.size(), 0);

		size_t len = x.size();
		DType res = 0;

		if (method == distance_method::EUCLIDEAN) {
			for (int i = 0; i < len; i++) {
				res += (x[i] - y[i])*(x[i] - y[i]);
			}
			res = std::sqrt(res);
		}

		if (method == distance_method::COSINE) {
			DType sum_xy = dot(x, y);
			DType norm_x = norm(x, 2);
			DType norm_y = norm(y, 2);
			res = sum_xy / (norm_x * norm_y);
		}

		if (method == distance_method::MANHATTAN) {
			for (int i = 0; i < len; i++) {
				res += abs(x[i] - y[i]);
			}
		}

		if (method == distance_method::CORRELATION) {
			DType xy_cov = cov(x, y);
			DType x_std = std(x);
			DType y_std = std(y);
			res = 1 - xy_cov / (x_std * y_std);
		}

		return res;
	}

}