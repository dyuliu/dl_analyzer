

#pragma once

#include <proto/analyzer.pb.h>
#include <config.h>

namespace analyzer {

	class Recorders {

	public:

		enum RECORD_TYPE : unsigned int {
			TRAIN_ERROR = 0U,
			TRAIN_LOSS = 1U,
			TEST_ERROR = 2U,
			TEST_LOSS = 3U,
			FORWARD_TIME = 4U,
			BACKWARD_TIME = 5U,
			UPDATE_TIME = 6U,
			LEARNING_RATE = 7U
		};

		Recorders();
		~Recorders();

	public:

		// print related
		void print_total_info();
		void print_specify_type(RECORD_TYPE record_type, int iterval = 1);

		// dump to file
		void save_to_file(std::string filename);

		// load from file
		void load_from_file(std::string filename);

	private:
		Recorder recorder;
		std::map<RECORD_TYPE, std::string> name_of_type;

	};

}