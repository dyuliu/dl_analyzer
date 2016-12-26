
#include <info/info.h>
#include <emath/include/distance.h>
#include <emath/include/matrix.h>

namespace analyzer {

	unsigned int Infos::index(TYPE_CLUSTER cluster_type, TYPE_CONTENT data_content) {
		return ((int)data_content * (int)TYPE_CLUSTER::END + (int)cluster_type);
	}

	static void print_vector(std::vector<DType> arr) {
		for (int i = 0; i < arr.size(); i++)
			printf("%3.3f\t", arr[i]);
		std::cout << std::endl;
	}

	static void print_kernels(std::vector<std::vector<DType>> &arr) {
		auto size = arr.size();
		std::cout << std::endl;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < arr[0].size(); j++)
				std::cout << arr[i][j];
			std::cout << std::endl;
		}
	}

	// message ClusterPoint{
	//	optional string name = 1;
	//	optional string type = 2;
	//	optional int32 index = 3;
	//	optional int32 group_id = 4;
	//	// one-dim value
	//	optional float value = 5;
	//	// multi-dim value
	//	repeated float data = 6;
	// }

	// message Cluster{
	//	optional string type = 1;
	//	// weight or grad
	//	optional string content = 2;
	//	// number of group
	//	optional float num = 3;
	//	repeated ClusterPoint points = 4;
	//	repeated ClusterPoint centre = 5;
	// }

	// data is all data from all kernels, arranged by a vector
	// length is stride_w * stride_h
	// size related with data
	void Infos::compute_cluster_kmeans(analyzer::Cluster *ptr, int channels, const float *data, int size, int length, int max_iter, int num_K, int rand_seed) {

		// divdie into small kernel
		std::vector<std::vector<DType>> kernels;
		for (int i = 0; i < size; i += length) {
			std::vector<DType> ker(length);
			std::copy(data + i, data + i + length, ker.begin());
			kernels.push_back(ker);
		}

		// initialize parameter
		const int num_clusters = num_K;
		const int iter_max = max_iter;

		std::vector<std::vector<DType>> centre;
		std::vector<DType> error(num_clusters);
		std::vector<std::pair<int, DType>> arr(kernels.size());

		// initialize centre point
		srand(rand_seed);
		for (int idx_clusters = 0; idx_clusters < num_clusters; idx_clusters++) {
			int pos = rand() % (kernels.size());
			centre.push_back(std::vector<DType>(kernels[pos].begin(), kernels[pos].end()));
		}

		for (int iter = 0; iter < iter_max; iter++) {
			// compute distance with each centre point for each kernels
			for (int idx_ker = 0; idx_ker < kernels.size(); idx_ker++) {
				double val_min = std::numeric_limits<double>::max();
				int pos_min = 0;
				for (int idx_clusters = 0; idx_clusters < num_clusters; idx_clusters++) {
					auto val = emath::distance(kernels[idx_ker], centre[idx_clusters], emath::DISTANCE::EUCLIDEAN);
					if (val < val_min) {
						val_min = val;
						pos_min = idx_clusters;
					}
				}
				arr[idx_ker].first = pos_min;
				arr[idx_ker].second = val_min;
			}
			// compute new centre points
			for (int idx_clusters = 0; idx_clusters < num_clusters; idx_clusters++) {
				centre[idx_clusters].assign(length, 0);
				error[idx_clusters] = 0;
				int count = 0;
				for (int idx_ker = 0; idx_ker < kernels.size(); idx_ker++) {
					if (arr[idx_ker].first == idx_clusters) {
						emath::add(centre[idx_clusters], kernels[idx_ker]);
						error[idx_clusters] += arr[idx_ker].second;
						count++;
					}
				}
				emath::element_mul(centre[idx_clusters], 1.0 / count);
			}

#ifdef __DEBUG_INFO_OUTPUT
			std::cout << "iter: " << iter << " ";
			print_vector(error);
#endif
		}

		// copy result to struct
		ptr->set_num(num_K);
		for (int idx_clusters = 0; idx_clusters < num_clusters; idx_clusters++) {
			auto ptr_centre = ptr->add_centre();
			ptr_centre->set_name("");
			ptr_centre->set_type("centre");
			ptr_centre->set_index(idx_clusters);
			ptr_centre->set_group_id(idx_clusters);
			for (int idx_num = 0; idx_num < centre[idx_clusters].size(); idx_num++)
				ptr_centre->add_data(centre[idx_clusters][idx_num]);
			ptr_centre->set_value(error[idx_clusters]);
		}
		int channel_id = 0;
		for (int idx_ker = 0; idx_ker < kernels.size(); idx_ker++) {
			auto ptr_point = ptr->add_points();
			ptr_point->set_name("");
			ptr_point->set_type("point");
			ptr_point->set_index(idx_ker);

			ptr_point->set_channel_id(channel_id++);
			if (channel_id == channels) channel_id = 0;
			
			ptr_point->set_group_id(arr[idx_ker].first);
			for (int idx_num = 0; idx_num < kernels[idx_ker].size(); idx_num++)
				ptr_point->add_data(kernels[idx_ker][idx_num]);
			ptr_point->set_value(arr[idx_ker].second);
		}

	}


	void Infos::compute_cluster(TYPE_CLUSTER cluster_type, TYPE_CONTENT data_content, unsigned int maxlayer) {
		
		if (cluster_type == TYPE_CLUSTER::END) return;

		int count = 0;
		for (int i = 0; i < info.layers_size(); i++) {

#ifdef __DEBUG_INFO_OUTPUT
			COUT_WARN << "Compute cluster of layer: " << info.layers(i).name() << std::endl;
#endif
			if (info.layers(i).type() == "batch_norm") 
				{ continue; } 
			else 
				{ count++; }

			if (count > maxlayer) return;

			if (data_content == TYPE_CONTENT::GRAD && !info.layers(i).grad_size()) return;
			if (data_content == TYPE_CONTENT::WEIGHT && !info.layers(i).weight_size()) return;

			// index of stat
			const int idx = index(cluster_type, data_content);
			auto ptr = info.mutable_layers(i)->mutable_cluster(idx);
			ptr->set_content(name_content_type[data_content]);
			ptr->set_type(name_cluster_type[cluster_type]);

			const float *data = NULL;
			int length = 0, size = 0;
			
			if (data_content == TYPE_CONTENT::WEIGHT) {
				data = info.layers(i).weight().data();
				length = info.layers(i).width() * info.layers(i).height();
				size = info.layers(i).weight().size();
			}
			else if (data_content == TYPE_CONTENT::GRAD) {
				data = info.layers(i).grad().data();
				length = info.layers(i).width() * info.layers(i).height();
				size = info.layers(i).grad().size();
			}

			if (cluster_type == TYPE_CLUSTER::KMEANS) {
				compute_cluster_kmeans(ptr, info.layers(i).channels(), data, size, length, 10, 5, 0);
			}

		}
	}

}