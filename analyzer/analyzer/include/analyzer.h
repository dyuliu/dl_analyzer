
#pragma once

#include <proto/analyzer.pb.h>
#include <config.h>

namespace analyzer {

	class Infos {

		Info info;

	public:

		enum STAT_TYPE : unsigned int {
			LAYER_MAX = 0U,
			LAYER_MIN = 1U,
			LAYER_MEAN = 2U,
			LAYER_SUM = 3U,
			LAYER_VAR = 4U,
			LAYER_STD = 5U,
			LAYER_NORM_0 = 6U,
			LAYER_NORM_1 = 7U,
			LAYER_NORM_2 = 8U,
			STAT_NUM_MAX
		};

		enum DISTANCE_TYPE : unsigned int {
			LAYER_EUCLIDEAN = 0U,
			LAYER_COSINE = 1U,
			LAYER_MANHATTAN = 2U,
			LAYER_CORRELATION = 3U,
			DISTANCE_NUM_MAX
		};

	public:
		
		void init(std::string path);

		void print();

		// dump to file
		void save_to_file(std::string foldname);

		// load from file
		void Infos::load_from_file(std::string filename);

	// Interface
	public:
		Info& get() { return info; }

	};

}