#include <libanalyzer/include/info/info.h>
#define MAP_TYPE_STATSEQ std::map<analyzer::Infos::TYPE_SEQ, std::string>
#define STAT_INDEX(x, y) (int)y * (int)analyzer::Infos::TYPE_SEQ::END + (int)x

namespace db {

	using TYPE_SEQ = analyzer::Infos::TYPE_SEQ;

	MAP_TYPE_STATSEQ mapTypeStatSeq = {
		{ TYPE_SEQ::HISTOGRAM, "StatSeqHistogram" }
	};

}