
#include <recorder/recorder.h>
#include <streambuf>
#include <regex>

namespace analyzer {

	void Recorders::parse_from_log(std::string src, std::string framework) {
		TYPE_FRAMEWORK fw_type = to_type<TYPE_FRAMEWORK>(framework);

		if (fw_type == TYPE_FRAMEWORK::CAFFEPRO) {
			parse_from_log_caffepro(src);
		}
	}

	// there, src is some log file path
	void Recorders::parse_from_log_caffepro(std::string src) {
		if (!filesystem::exist(src.c_str()))
			THROW("The destination does not exist!");

		std::ifstream fp(src.c_str());
		std::string str;
		
		int count = 0;

		bool TRAIN_PHASE = true; // 0-train 1-test

		int iter = 0;
		std::map<std::string, float> val;

		while (std::getline(fp, str)) {

			std::smatch match;

			if (std::regex_search(str, match, std::regex("--TRAIN: iter = (.*),"))) {
				TRAIN_PHASE = true;
				iter = atoi(match[1].str().c_str());
			}
			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Error: (.*)"))) {
				val["train_error"] = atof(match[1].str().c_str());
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Loss: (.*)"))) {
				val["train_loss"] = atof(match[1].str().c_str());
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Forward time: (.*)"))) {
				val["forward_time"] = atof(match[1].str().c_str());
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Backward time: (.*)"))) {
				val["backward_time"] = atof(match[1].str().c_str());
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Update time: (.*)"))) {
				val["update_time"] = atof(match[1].str().c_str());
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Sum time: (.*)"))) {
				val["sum_time"] = atof(match[1].str().c_str());
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("Learning rate: (.*)"))) {
				val["learning_rate"] = atof(match[1].str().c_str());
				//std::cout << "iter: " << iter << ", train error: " << val["train_error"] << ", train loss: " << val["train_loss"]
				//	<< ", forward time: " << val["forward_time"] << ", backward time: " << val["backward_time"]
				//	<< ", update time: " << val["update_time"] << ", sum time: " << val["sum_time"] << ", learning rate:" << val["learning_rate"] << std::endl;
				add_data(iter, TYPE_RECORD::TRAIN_ERROR, val["train_error"]);
				add_data(iter, TYPE_RECORD::TRAIN_LOSS, val["train_loss"]);
				add_data(iter, TYPE_RECORD::FORWARD_TIME, val["forward_time"]);
				add_data(iter, TYPE_RECORD::BACKWARD_TIME, val["backward_time"]);
				add_data(iter, TYPE_RECORD::UPDATE_TIME, val["update_time"]);
				add_data(iter, TYPE_RECORD::SUM_TIME, val["sum_time"]);
			}

			if (TRAIN_PHASE && std::regex_search(str, match, std::regex("--TEST: iter = (.*),"))) {
				TRAIN_PHASE = false;
				iter = atoi(match[1].str().c_str());
			}

			if (!TRAIN_PHASE && std::regex_search(str, match, std::regex("Error: (.*)"))) {
				val["test_error"] = atof(match[1].str().c_str());
			}

			if (!TRAIN_PHASE && std::regex_search(str, match, std::regex("Loss: (.*)"))) {
				val["test_loss"] = atof(match[1].str().c_str());
				//std::cout << "iter: " << iter << ", test error: " << val["test_error"] << ", test loss: " << val["test_loss"] << std::endl;
				add_data(iter, TYPE_RECORD::TEST_ERROR, val["test_error"]);
				add_data(iter, TYPE_RECORD::TEST_LOSS, val["test_loss"]);
			}

		}

		fp.close();
	}

}