
#pragma once

#include <config.h>
#include <vector>

namespace emath {

	// distance method
	enum class DISTANCE : unsigned int {
		EUCLIDEAN = 1U,
		COSINE = 2U,
		MANHATTAN = 3U,
		CORRELATION = 4U,
		EUCLIDEAN_NORM = 5U,
		COSINE_NORM = 6U
	};

	DType distance(std::vector<DType> &x, std::vector<DType> &y, DISTANCE method);
}