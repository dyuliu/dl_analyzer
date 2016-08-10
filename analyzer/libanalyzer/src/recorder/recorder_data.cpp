
#include <recorder/recorder.h>

namespace analyzer {

	std::vector<std::tuple<int, std::string, float>> Recorders::get_specify_type(TYPE_RECORD record_type, int iterval) {
		return get_specify_type(name_of_type[record_type], iterval);
	}

	std::vector<std::tuple<int, std::string, float>> Recorders::get_specify_type(std::string record_type, int iterval) {
		// search
		std::vector<std::tuple<int, std::string, float>> res;
		for (int i = 0; i < recorder.tuple_size(); i++) {
			if (recorder.tuple(i).iteration() % iterval == 0 && recorder.tuple(i).type() == record_type) {
				res.push_back(std::tuple<int, std::string, float>(
					recorder.tuple(i).iteration(), 
					recorder.tuple(i).type(), 
					recorder.tuple(i).value()));
			}
		}
		return res;
	}

	std::vector<std::tuple<int, std::string, float>> Recorders::get_total_info(int iterval) {
		std::vector<std::tuple<int, std::string, float>> res;
		for (int i = 0; i < recorder.tuple_size(); i++) {
			if (recorder.tuple(i).iteration() % iterval == 0) {
				res.push_back(std::tuple<int, std::string, float>(
					recorder.tuple(i).iteration(),
					recorder.tuple(i).type(),
					recorder.tuple(i).value()));
			}
		}
		return res;
	}

}