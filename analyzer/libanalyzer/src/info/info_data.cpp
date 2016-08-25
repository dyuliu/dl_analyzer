

#include <info/info.h>

namespace analyzer {

	std::vector<DType> Infos::get_content_data(TYPE_CONTENT content_type, std::string layer_name) {
		for (int i = 0; i < info.layers_size(); i++) {
			if (info.layers(i).name() == layer_name) {
				if (content_type == TYPE_CONTENT::GRAD)
					return ArrayToVector(info.layers(i).grad());
				else if (content_type == TYPE_CONTENT::WEIGHT)
					return ArrayToVector(info.layers(i).weight());
				else
					THROW("can not find content type in get_content_data function.");
			}
		}
		return std::vector<DType>();
	}

	std::vector<DType> Infos::get_content_data(std::string content_type, std::string layer_name) {
		return get_content_data(to_type<TYPE_CONTENT>(content_type), layer_name);
	}

}