
#pragma once

#include <config.h>
#include <vector>

namespace emath {

	DType dot(const std::vector<DType> &x, const std::vector<DType> &y);
	void add(std::vector<DType> &dst, const std::vector<DType> &src);
	void element_mul(std::vector<DType> &x, DType y);
}