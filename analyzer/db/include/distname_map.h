#include <libanalyzer/include/info/info.h>
#define MAP_TYPE_DIST std::map<analyzer::Infos::TYPE_DISTANCE, std::string>
#define DIST_INDEX(x, y) (int)y * (int)analyzer::Infos::TYPE_DISTANCE::END + (int)x

namespace db {

	using TYPE_DISTANCE = analyzer::Infos::TYPE_DISTANCE;

	MAP_TYPE_DIST mapTypeDist = {
		{ TYPE_DISTANCE::CORRELATION, "DistCorrelation" },
		{ TYPE_DISTANCE::COSINE, "DistCosine" },
		{ TYPE_DISTANCE::EUCLIDEAN, "DistEuclidean" },
		{ TYPE_DISTANCE::MANHATTAN, "DistManhattan" },
		{ TYPE_DISTANCE::COSINE_NORM, "DistCosineNorm" },
		{ TYPE_DISTANCE::EUCLIDEAN_NORM, "DistEuclideanNorm" }

	};

}