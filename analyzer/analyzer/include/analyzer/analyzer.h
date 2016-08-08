
#pragma once

#include <proto/analyzer.pb.h>
#include <config.h>
#include <map>

namespace analyzer {

	class Infos {

	public:

		enum class TYPE_STAT : unsigned int {
			MAX		= 0U,
			MIN		= 1U,
			MEAN	= 2U,
			SUM		= 3U,
			VAR		= 4U,
			STD		= 5U,
			NORM_0	= 6U,
			NORM_1	= 7U,
			NORM_2	= 8U,
			END
		};

		enum class TYPE_DISTANCE : unsigned int {
			EUCLIDEAN	= 0U,
			COSINE		= 1U,
			MANHATTAN	= 2U,
			CORRELATION	= 3U,
			END
		};

		enum class TYPE_CONTENT : unsigned int {
			GRAD	= 0U,
			WEIGHT	= 1U,
			END
		};

		enum class HyperParam : unsigned int {
			STAT = 0U,
			DISTANCE = 1U
		};

	public:

		// dump to file
		void save_to_file(std::string foldname);

		// load from file
		void Infos::load_from_file(std::string filename);

	public:

		// get
		unsigned int index(TYPE_STAT stat_type, TYPE_CONTENT data_content);
		unsigned int index(TYPE_DISTANCE distance_type, TYPE_CONTENT data_content);

		// stat
		void compute(TYPE_STAT stat_type, TYPE_CONTENT data_content);
		void compute_list(std::vector<TYPE_STAT> stat_list, TYPE_CONTENT data_content);
		void compute_all(TYPE_CONTENT data_content);

		// distance
		void compute(TYPE_DISTANCE distrance_type, TYPE_CONTENT data_content, Infos &other);
		void compute_list(std::vector<TYPE_DISTANCE> const distance_list, TYPE_CONTENT data_content, Infos &other);
		void compute_all(TYPE_CONTENT data_content, Infos &other);

		// data transfer
		void RepeatedToVector(const ::google::protobuf::RepeatedField<float>& x, std::vector<DType> &y);

	// Print
	public:
		void print_total_info();
		void print_file_info();
		void print_conv_layer_info();
		void print_stat_info(TYPE_CONTENT data_content);
		void print_distance_info(TYPE_CONTENT data_content);

	// Interface
	public:

		Info& get() { return info; }
		
		Info* getInfo() { return &info; }

		// constructor
		Infos();
		Infos(std::string filename);
		// for rank, it will be depleted in future
		Infos(std::string path, int rank_size);

		// copy
		void Infos::copy_hyperparam(Infos &other, TYPE_CONTENT content_type, HyperParam hp);

		// init
		void init_stat();
		void init_type_name();
	
	private:
		
		// dump info
		Info info;

		// running recorder info
		Recorder recorder;

		std::map<TYPE_DISTANCE, std::string> name_distance_type;
		std::map<TYPE_STAT, std::string> name_stat_type;
		std::map<TYPE_CONTENT, std::string> name_content_type;

	};
}