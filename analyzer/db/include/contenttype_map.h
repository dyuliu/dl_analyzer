#include <libanalyzer/include/info/info.h>
#define MAP_TYPE_CONTENT std::map<analyzer::Infos::TYPE_CONTENT, std::string>

namespace db {

	using TYPE_CONTENT = analyzer::Infos::TYPE_CONTENT;

	MAP_TYPE_CONTENT mapTypeContent = {
		{ TYPE_CONTENT::GRAD, "Grad" },
		{ TYPE_CONTENT::WEIGHT, "Weight" }
	};

}