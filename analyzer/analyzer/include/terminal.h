
#pragma once

#include <config.h>
#include <recorder/recorder.h>
#include <analyzer/analyzer.h>


namespace analyzer {

	enum class COMPUTE_METHOD : unsigned int {
	};


	// stat
	void compute_file(std::string filename);

	void compute_batch_file(std::vector<std::string> filenames);
	
	void compute_folder(std::string foldername, int batch_size);

	// distance

}