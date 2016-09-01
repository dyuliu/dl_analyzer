
#include <info/info.h>
#include <emath/include/distance.h>

namespace analyzer {

	unsigned int Infos::index(TYPE_DISTANCE distance_type, TYPE_CONTENT data_content) {
		return ((int)data_content * (int)TYPE_DISTANCE::END + (int)distance_type);
	}

	void Infos::compute_dist(TYPE_DISTANCE distrance_type, TYPE_CONTENT data_content, Infos &other) {

		if (distrance_type == TYPE_DISTANCE::END) return;

		for (int i = 0; i < info.layers_size(); i++) {

#ifdef __DEBUG_INFO_OUTPUT
			COUT_WARN << "Compute distance of layer: " << info.layers(i).name() << std::endl;
#endif
			if (info.layers(i).type() == "batch_norm") continue;

			if (data_content == TYPE_CONTENT::GRAD && !info.layers(i).grad_size()) return;
			if (data_content == TYPE_CONTENT::WEIGHT && !info.layers(i).weight_size()) return;

			// index of stat
			const int idx = index(distrance_type, data_content);
			auto ptr = info.mutable_layers(i)->mutable_distance(idx);
			
			std::vector<DType> data;
			if (data_content == TYPE_CONTENT::GRAD) data = ArrayToVector(other.get().layers(i).grad());
			if (data_content == TYPE_CONTENT::WEIGHT) data = ArrayToVector(other.get().layers(i).weight());

			if (distrance_type == TYPE_DISTANCE::COSINE) {
				if (data_content == TYPE_CONTENT::GRAD)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).grad()), data, emath::DISTANCE::COSINE));
				if (data_content == TYPE_CONTENT::WEIGHT)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).weight()), data, emath::DISTANCE::COSINE));
			}

			if (distrance_type == TYPE_DISTANCE::EUCLIDEAN) {
				if (data_content == TYPE_CONTENT::GRAD)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).grad()), data, emath::DISTANCE::EUCLIDEAN));
				if (data_content == TYPE_CONTENT::WEIGHT)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).weight()), data, emath::DISTANCE::EUCLIDEAN));
			}

			if (distrance_type == TYPE_DISTANCE::CORRELATION) {
				if (data_content == TYPE_CONTENT::GRAD)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).grad()), data, emath::DISTANCE::CORRELATION));
				if (data_content == TYPE_CONTENT::WEIGHT)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).weight()), data, emath::DISTANCE::CORRELATION));
			}

			if (distrance_type == TYPE_DISTANCE::MANHATTAN) {
				if (data_content == TYPE_CONTENT::GRAD)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).grad()), data, emath::DISTANCE::MANHATTAN));
				if (data_content == TYPE_CONTENT::WEIGHT)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).weight()), data, emath::DISTANCE::MANHATTAN));
			}

			if (distrance_type == TYPE_DISTANCE::COSINE_NORM) {
				if (data_content == TYPE_CONTENT::GRAD)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).grad()), data, emath::DISTANCE::COSINE_NORM));
				if (data_content == TYPE_CONTENT::WEIGHT)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).weight()), data, emath::DISTANCE::COSINE_NORM));
			}

			if (distrance_type == TYPE_DISTANCE::EUCLIDEAN_NORM) {
				if (data_content == TYPE_CONTENT::GRAD)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).grad()), data, emath::DISTANCE::EUCLIDEAN_NORM));
				if (data_content == TYPE_CONTENT::WEIGHT)
					ptr->set_value(emath::distance(ArrayToVector(info.layers(i).weight()), data, emath::DISTANCE::EUCLIDEAN_NORM));
			}

		}
	}

	void Infos::compute_dist_list(std::vector<TYPE_DISTANCE> distance_list, TYPE_CONTENT data_content, Infos &other) {

#ifdef __DEBUG_INFO_OUTPUT
		COUT_METD << "func: compute_list_distance" << std::endl;
#endif

		for (unsigned int j = 0; j < distance_list.size(); j++) {
#ifdef __DEBUG_INFO_OUTPUT
			__FUNC_TIME_CALL(compute_dist(distance_list[j], data_content, other), name_distance_type[distance_list[j]]);
#else
			compute_dist((TYPE_DISTANCE)distance_list[j], data_content, other);
#endif
		}
	}

	// compute all distance
	void Infos::compute_dist_all(TYPE_CONTENT data_content, Infos &other) {

#ifdef __DEBUG_INFO_OUTPUT
		COUT_METD << "func: compute_all_distance" << std::endl;
#endif

		for (unsigned int j = (int)TYPE_DISTANCE::EUCLIDEAN; j < (int)TYPE_DISTANCE::END; j++) {
#ifdef __DEBUG_INFO_OUTPUT
			__FUNC_TIME_CALL(compute_dist((TYPE_DISTANCE)j, data_content, other), name_distance_type[(TYPE_DISTANCE)j]);
#else
			compute_dist((TYPE_DISTANCE)j, data_content, other);
#endif
		}
	}
}