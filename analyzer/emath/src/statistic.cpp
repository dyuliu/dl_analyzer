

#include <statistic.h>
#include <cmath>
#include <limits>
#include <algorithm>
#include <numeric>
#include <matrix.h>
#include <iostream>

namespace emath {

	DType sum(const std::vector<DType> &x) {
		return DType(std::accumulate(x.begin(), x.end(), 0.0));
	}

	DType mean(const std::vector<DType> &x) {

		CHECK_NE(x.size(), 0);

		DType res = 0;

		if (x.size() <= 0)
			return 0;

		res = sum(x) / x.size();
		return res;
	}

	// q in [0, 1]
	// return: the q*size() th number of x
	DType quantile(const std::vector<DType> &x, double q) {

		CHECK_NE(x.size(), 0 );

		CHECK_GE(1, q);
		CHECK_GE(q, 0);
		
		int pos = x.size()*q;
		if (pos == x.size()) {
			pos = x.size() - 1;
		}

		std::vector<DType> x_c(x.size());
		std::copy(x.begin(), x.end(), x_c.begin());

		std::nth_element(x_c.begin(), x_c.begin() + pos, x_c.end());

		return *(x_c.begin() + pos);
	}


	std::vector<DType> changeratio(const std::vector<DType> &w, const std::vector<DType> &g, 
		std::vector<STNumScaleBin> bins) {

		CHECK_NE(w.size(), 0);
		CHECK_EQ(w.size(), g.size());

		size_t len = w.size();
		DType ratio;
		// count numbers
		for (int i = 0; i < len; i++) {
			ratio = abs(g[i] / w[i]);
			for (auto &b : bins) {
				if (ratio <= b.ls) 
					b.count++;
			}
		}

		// divide total length
		size_t lenBins = bins.size();
		std::vector<DType> res(lenBins);
		for (int i = 0; i < lenBins; i++) {
			res[i] = (DType)bins[i].count / len;
		}

		return res;
	}

	std::vector<DType> histogram(const std::vector<DType> &x, int bins) {

		CHECK_NE(x.size(), 0);
		CHECK_GE(bins, 0);

		std::vector<DType> x_c(x.size());
		std::copy(x.begin(), x.end(), x_c.begin());

		std::sort(x_c.begin(), x_c.end());
		auto v_min = *(x_c.begin());
		auto v_max = *(x_c.end()-1);

		DType band = (v_max - v_min) / bins;
		std::vector<DType> hist(bins);

		auto count = 0;
		DType inv_size = 1.0 / x_c.size();
		for (auto v : x_c) {
			auto band_max = x_c[0] + (count + 1) * band;
			if (v > band_max) {
				count++;
			}
			hist[count] += inv_size;
		}

		return hist;
	}

	DType var(const std::vector<DType> &x) {

		CHECK_NE(x.size(), 0);
		
		DType avr = mean(x);
		DType res = 0;

		for (auto x_v : x) 
			res += (x_v - avr)*(x_v - avr);

		res /= x.size();
		return res;
	}

	DType std(const std::vector<DType> &x) {
		return std::sqrt(var(x));
	}

	DType max(const std::vector<DType> &x) {
		return *std::max_element(x.begin(), x.end());
	}

	DType min(const std::vector<DType> &x) {
		return *std::min_element(x.begin(), x.end());
	}

	DType cov(const std::vector<DType> &x, const std::vector<DType> &y) {
		
		CHECK_EQ(x.size(), y.size());
		CHECK_NE(x.size(), 0);

		DType res = 0;
		size_t len = x.size();

		auto x_aver = mean(x);
		auto y_aver = mean(y);
		for (int i = 0; i < len; i++) {
			res += (x[i] - x_aver)*(y[i] - y_aver);
		}

		res /= len;
		return res;
	}

	// 0^0 = 0, n^0 = 1 (n != 0)
	DType norm(const std::vector<DType> &x, unsigned int norm) {

		CHECK_GE(norm, 0);

		DType res = 0;

		for (auto x_v : x) {
			if (norm == 0 && x_v == 0)
				res += 0;
			else
				res += abs(std::pow(x_v, norm));
		}

		if (norm != 0)
			res = std::pow(res, 1.0 / norm);
		return res;
	}

	// return to x
	void normalization(std::vector<DType> &x, norm_method method) {

		CHECK_NE(x.size(), 0);

		if (method == norm_method::MINMAX) {
			auto x_min = min(x);
			auto x_max = max(x);
			auto x_dis = x_max - x_min;
			CHECK_NE(x_dis, 0);
			for (auto &x_v : x) {
				x_v = (x_v - x_min) / (x_dis);
			}
		}

		if (method == norm_method::ZSCORE) {
			auto x_avr = mean(x);
			auto x_std = std(x);
			CHECK_NE(x_std, 0);
			for (auto &x_v : x) {
				x_v = (x_v - x_avr) / (x_std);
			}
		}
	}
}