
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
			QUANTILE_1_2 = 9U,
			QUANTILE_1_4 = 10U,
			QUANTILE_3_4 = 11U,
			END
		};

		enum class TYPE_STAT_KERNEL : unsigned int {
			CR_NORM_1 = 0U,
			CR_NORM_2 = 1U,
			END
		};

		enum class TYPE_SEQ : unsigned int {
			HISTOGRAM = 0U,
			CHANGERATIO = 1U,
			END
		};

		enum class TYPE_DISTANCE : unsigned int {
			EUCLIDEAN		= 0U,
			COSINE			= 1U,
			MANHATTAN		= 2U,
			CORRELATION		= 3U,
			EUCLIDEAN_NORM	= 4U,
			COSINE_NORM		= 5U,
			END
		};

		enum class TYPE_CONTENT : unsigned int {
			GRAD	= 0U,
			WEIGHT	= 1U,
			END
		};

		enum class TYPE_CLUSTER : unsigned int {
			KMEANS	= 0U,
			END
		};

		enum class HyperParam : unsigned int {
			STAT = 0U,
			DISTANCE = 1U,
			SEQ = 2U
		};

	public:

		// dump to file
		void save_to_file(std::string foldname);

		// load from file
		void load_from_file(std::string filename);

	public:

		// get
		unsigned int index(TYPE_STAT stat_type, TYPE_CONTENT data_content);
		unsigned int index(TYPE_DISTANCE distance_type, TYPE_CONTENT data_content);
		unsigned int index(TYPE_SEQ seq_type, TYPE_CONTENT data_content);
		unsigned int index(TYPE_CLUSTER cluster_type, TYPE_CONTENT data_content);
		unsigned int index(TYPE_STAT_KERNEL stat_kernel_type, TYPE_CONTENT data_content);

		// string to type
		template <typename Tout>
		Tout to_type(std::string);

		// stat
		void compute_stat(TYPE_STAT stat_type, TYPE_CONTENT data_content);
		void compute_stat_list(std::vector<TYPE_STAT> stat_list, TYPE_CONTENT data_content);
		void compute_stat_all(TYPE_CONTENT data_content);

		// distance
		void compute_dist(TYPE_DISTANCE distrance_type, TYPE_CONTENT data_content, Infos &other);
		void compute_dist_list(std::vector<TYPE_DISTANCE> const distance_list, TYPE_CONTENT data_content, Infos &other);
		void compute_dist_all(TYPE_CONTENT data_content, Infos &other);

		// seq
		void compute_seq(TYPE_SEQ seq_type, TYPE_CONTENT data_content);
	    void compute_seq_list(std::vector<TYPE_SEQ> stat_list, TYPE_CONTENT data_content);
		void compute_seq_all(TYPE_CONTENT data_content);

		// stat_kernel
		void compute_stat_kernel(TYPE_STAT_KERNEL seq_type, TYPE_CONTENT data_content);
		void compute_stat_kernel_list(std::vector<TYPE_STAT_KERNEL> stat_list, TYPE_CONTENT data_content);
		void compute_stat_kernel_all(TYPE_CONTENT data_content);

		// data transfer

		// get data
		std::vector<DType> get_content_data(TYPE_CONTENT content_type, std::string layer_name);
		std::vector<DType> get_content_data(std::string content_type, std::string layer_name);

		// cluster
		void compute_cluster(TYPE_CLUSTER cluster_type, TYPE_CONTENT data_content, unsigned int maxlayer);
		static void compute_cluster_kmeans(analyzer::Cluster *ptr, int channels, const float *data, int size, int length, int max_iter, int num_K = 5, int rand_seed = 0);

	// Print
	public:
		void print_total_info();
		void print_file_info();
		void print_conv_layer_info();
		void print_stat_info(TYPE_CONTENT data_content);
		void print_distance_info(TYPE_CONTENT data_content);
		void print_seq_info(TYPE_CONTENT data_content);
		void print_cluster_info(TYPE_CONTENT data_content);

	// Interface
	public:

		Info& get() { return info; }

		// constructor
		Infos();
		Infos(std::string filename);
		// for rank, it will be depleted in future
		Infos(std::string path, int rank_size);

		// copy
		void copy_hyperparam(Infos &other, TYPE_CONTENT content_type, HyperParam hp);

		// init
		void init_stat();
		void init_type_name();
	
	private:
		
		// dump info
		Info info;

		std::map<TYPE_DISTANCE, std::string> name_distance_type;
		std::map<TYPE_STAT, std::string> name_stat_type;
		std::map<TYPE_CONTENT, std::string> name_content_type;
		std::map<TYPE_SEQ, std::string> name_seq_type;
		std::map<TYPE_CLUSTER, std::string> name_cluster_type;
		std::map<TYPE_STAT_KERNEL, std::string> name_stat_kernel_type;


	};
}