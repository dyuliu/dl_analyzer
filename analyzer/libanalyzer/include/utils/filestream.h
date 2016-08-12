
#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace analyzer {

	class filestream {
	public:
		
		template<typename T>
		static void output_vector_to_file(std::vector<T> &data, std::string filepath) {
			std::ofstream fp(filepath.c_str());
			for (auto ele : data) {
				fp << ele << std::endl;
			}
			fp.close();
		}

	};
}