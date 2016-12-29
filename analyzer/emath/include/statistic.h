
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

	DType cov(const std::vector<DType> &x, const std::vector<DType> &y);

	// normalization method
	enum norm_method : unsigned int {
		MINMAX = 1U,
		ZSCORE = 2U
	};

	void normalization(std::vector<DType> &x, norm_method method);
	
	DType quantile(const std::vector<DType> &x, double q);

	std::vector<DType> histogram(const std::vector<DType> &x, int bins);
	std::vector<DType> changeratio(const std::vector<DType> &w, const std::vector<DType> &g, 
		std::vector<STNumScaleBin> bins);

	
}