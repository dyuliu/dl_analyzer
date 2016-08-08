#include <analyzer/include/analyzer/analyzer.h>
#define MAP_TYPE_STAT std::map<analyzer::Infos::TYPE_STAT, std::string>
#define MAP_TYPE_CONTENT std::map<analyzer::Infos::TYPE_CONTENT, std::string>
#define INDEX(x, y) (int)y * (int)analyzer::Infos::TYPE_STAT::END + (int)x

namespace db {

	using TYPE_STAT = analyzer::Infos::TYPE_STAT;
	using TYPE_CONTENT = analyzer::Infos::TYPE_CONTENT;

	MAP_TYPE_STAT mapTypeStat = {
		{ TYPE_STAT::MAX, "Max" },
		{ TYPE_STAT::MIN, "Min" },
		{ TYPE_STAT::MEAN, "Mean" },
		{ TYPE_STAT::SUM, "Sum" },
		{ TYPE_STAT::VAR, "Var" },
		{ TYPE_STAT::STD, "Std" },
		{ TYPE_STAT::NORM_0, "Norm0" },
		{ TYPE_STAT::NORM_1, "Norm1" },
		{ TYPE_STAT::NORM_2, "Norm2" }
	};

	MAP_TYPE_CONTENT mapTypeContent = {
		{ TYPE_CONTENT::GRAD, "g" },
		{ TYPE_CONTENT::WEIGHT, "w"}
	};

}