

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
			SUM_TIME = 7U,
			LEARNING_RATE = 8U
		};

		enum class TYPE_FRAMEWORK : unsigned int {
			CAFFEPRO = 0U,
			CAFFE = 1U,
			TORCH = 2U,
			TENSORFLOW = 3U,
			END
		};

		Recorders();
		Recorders(std::string filename);
		~Recorders();

	public:
		// string to TYPE
		template <typename Tout>
		Tout to_type(std::string);

		// print related
		void print_total_info();
		void print_specify_type(TYPE_RECORD record_type, int iterval = 1);
		void print_specify_type(std::string record_type, int iterval = 1);

		// dump to file
		void save_to_file(std::string filename);

		// load from file
		void load_from_file(std::string filename);

		// get function
		Recorder* get() { return &recorder; }

		// parse
		void parse_from_log(std::string src, std::string framework);
		void parse_from_log_caffepro(std::string src);

	public:
		void add_data(int iter, TYPE_RECORD type, DType value, DType *data = NULL, int len_data = 0);

	public:
		// data related
		std::vector<std::tuple<int, std::string, float>> get_specify_type(TYPE_RECORD record_type, int iterval = 1);
		std::vector<std::tuple<int, std::string, float>> get_specify_type(std::string record_type, int iterval = 1);

		std::vector<std::tuple<int, std::string, float>> get_total_info(int iterval = 1);

	private:
		Recorder recorder;
		std::map<TYPE_RECORD, std::string> name_of_type;

		std::map<TYPE_FRAMEWORK, std::string> name_of_framework;
	};

}