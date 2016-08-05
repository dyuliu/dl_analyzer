
#pragma once

#include <config.h>
#include <vector>

namespace emath {

	// distance method
	enum distance_method : unsigned int {
		EUCLIDEAN = 1U,
		COSINE = 2U,
		MANHATTAN = 3U,
		CORRELATION = 4U
	};

	DType distance(const std::vector<DType> &x, const std::vector<DType> &y, distance_method method);
}