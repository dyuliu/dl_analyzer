
#include <recorder/recorder.h>
#include <fstream>
#include <regex>

namespace analyzer {

	void Recorders::parse_from_log_file_caffepro(std::string filename) {

		std::string line_buffer;
		
		std::ifstream in(filename);
		if (!in) throw("Unable to open config file!");

		const std::regex p_phase("--TRAIN");
		const std::regex p_iter_1("--TRAIN: iter = (.*)");
		const std::regex p_iter_2("--TEST: iter = (.*), ");

		std::smatch s;
		while (std::getline(in, line_buffer)) {
			// std::cout << line_buffer << std::endl;
			
			std::regex_search(line_buffer, s, p_iter_2);
			for (auto s1 : s) std::cout << s1 << std::endl;
			
		}

	}

}