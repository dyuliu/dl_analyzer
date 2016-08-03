
#pragma once

#include <config.h>
#include <vector>

namespace emath {

	// statistic values
	DType sum(const std::vector<DType> &x);
	DType mean(const std::vector<DType> &x);

	DType var(const std::vector<DType> &x);
	DType std(const std::vector<DType> &x);

	DType max(const std::vector<DType> &x);
	DType min(const std::vector<DType> &x);

	DType norm(const std::vector<DType> &x, unsigned int norm);

	// normalization method
	enum norm_method : unsigned int {
		MINMAX = 1U,
		ZSCORE = 2U
	};

	void nomrlization(std::vector<DType> &x, norm_method method);

	// distance method
	enum distance_method : unsigned int {
		EUCLIDEAN	= 1U,
		COSINE		= 2U,
		MANHATTAN	= 3U,
		CORRELATION = 4U
	};

	DType distance(const std::vector<DType> &x, const std::vector<DType> &y, distance_method method);

}