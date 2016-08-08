#include <analyzer/include/analyzer/analyzer.h>
#define MAP_TYPE std::map<unsigned int, std::string>

namespace db {

	using STAT_TYPE = analyzer::Infos::STAT_TYPE_CLASS;

	MAP_TYPE nameMap = { 
		{ STAT_TYPE::LAYER_STAT_MEAN_C, "Mean" },
		{ STAT_TYPE::LAYER_STAT_MIN_C, "Min" },
		{ STAT_TYPE::STAT_NUM_MAX_C, "Max" },
		{ STAT_TYPE::LAYER_STAT_NORM_0_C, "Norm0" },
		{ STAT_TYPE::LAYER_STAT_NORM_1_C, "Norm1" },
		{ STAT_TYPE::LAYER_STAT_NORM_2_C, "Norm2" },
		{ STAT_TYPE::LAYER_STAT_STD_C, "Std" },
		{ STAT_TYPE::LAYER_STAT_SUM_C, "Sum" },
		{ STAT_TYPE::LAYER_STAT_VAR_C, "Var" }
	};
}