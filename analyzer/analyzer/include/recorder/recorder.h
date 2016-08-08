

#pragma once

#include <proto/analyzer.pb.h>
#include <config.h>

namespace analyzer {

	class Recorders {

	public:

		enum class TYPE_RECORD : unsigned int {
			TRAIN_ERROR = 0U,
			TRAIN_LOSS = 1U,
			TEST_ERROR = 2U,
			TEST_LOSS = 3U,
			FORWARD_TIME = 4U,
			BACKWARD_TIME = 5U,
			UPDATE_TIME = 6U,
			LEARNING_RATE = 7U
		};

		enum class TYPE_FRAMEWORK : unsigned int {
			CAFFEPRO = 0U,
			CAFFE = 1U,
			CNTK = 2U,
			TORCH = 3U,
			TENSORFLOW = 4U
		};

		Recorders();
		Recorders(std::string filename);
		~Recorders();

	public:

		// print related
		void print_total_info();
		void print_specify_type(TYPE_RECORD record_type, int iterval = 1);
		void print_specify_type(std::string record_type, int iterval = 1);

		// dump to file
		void save_to_file(std::string filename);

		// load from file
		void load_from_file(std::string filename);

		// parse log file
		void load_from_log_file(std::string filename, TYPE_FRAMEWORK framework_type);

	protected:
		// parse
		void parse_from_log_file_caffepro(std::string filename);
		void parse_from_log_file_caffe(std::string filename);
		void parse_from_log_file_cntk(std::string filename);

	private:
		Recorder recorder;
		std::map<TYPE_RECORD, std::string> name_of_type;

	};

}