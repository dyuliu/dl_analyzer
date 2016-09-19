#include <libanalyzer/include/info/info.h>
#define MAP_TYPE_CLUSTER std::map<analyzer::Infos::TYPE_CLUSTER, std::string>
#define CLUSTER_INDEX(x, y) (int)y * (int)analyzer::Infos::TYPE_CLUSTER::END + (int)x

namespace db {

	using TYPE_CLUSTER = analyzer::Infos::TYPE_CLUSTER;

	MAP_TYPE_CLUSTER mapTypeCluster = {
		{ TYPE_CLUSTER::KMEANS, "ClusterKmeans" }
	};

}
