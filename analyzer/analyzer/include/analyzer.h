
#pragma once

#include <proto/analyzer.pb.h>
#include <config.h>
#include <map>

namespace analyzer {

	class Infos {

	public:

		using data_type = float;

		enum STAT_TYPE : unsigned int {
			LAYER_STAT_MAX		= 0U,
			LAYER_STAT_MIN		= 1U,
			LAYER_STAT_MEAN		= 2U,
			LAYER_STAT_SUM		= 3U,
			LAYER_STAT_VAR		= 4U,
			LAYER_STAT_STD		= 5U,
			LAYER_STAT_NORM_0	= 6U,
			LAYER_STAT_NORM_1	= 7U,
			LAYER_STAT_NORM_2	= 8U,
			STAT_NUM_MAX
		};

		enum DISTANCE_TYPE : unsigned int {
			LAYER_DIS_EUCLIDEAN		= 0U,
			LAYER_DIS_COSINE		= 1U,
			LAYER_DIS_MANHATTAN		= 2U,
			LAYER_DIS_CORRELATION	= 3U,
			DISTANCE_NUM_MAX
		};

		enum DATA_CONTENT : unsigned int {
			CONTENT_GRAD	= 0U,
			CONTENT_WEIGHT = 1U
		};

	public:
		
		void init(std::string path);

		// dump to file
		void save_to_file(std::string foldname);

		// load from file
		void load_from_file(std::string filename);

	public:

		void compute_stat(STAT_TYPE stat_type, DATA_CONTENT data_content);
		void compute_all_stat(DATA_CONTENT data_content);

		void compute_distance(DISTANCE_TYPE distrance_type);

		void RepeatedToVector(const ::google::protobuf::RepeatedField<float>& x, std::vector<data_type> &y);


	// Print
	public:
		void print_total_info();
		void print_file_info();
		void print_conv_layer_info();
		void print_stat_info(DATA_CONTENT data_content);

	// Interface
	public:

		Infos();
		~Infos();

	private:
		
		// dump info
		Info info;

		// running recorder info
		Recorder recorder;

		std::map<DISTANCE_TYPE, std::string> name_distance_type;
		std::map<STAT_TYPE, std::string> name_stat_type;

	};
}