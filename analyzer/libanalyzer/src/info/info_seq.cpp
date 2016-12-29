
#include <info/info.h>
#include <emath/include/statistic.h>

namespace analyzer {

	unsigned int Infos::index(TYPE_SEQ seq_type, TYPE_CONTENT data_content) {
		return ((int)data_content * (int)TYPE_SEQ::END + (int)seq_type);
	}

	void Infos::compute_seq(TYPE_SEQ seq_type, TYPE_CONTENT data_content) {

		if (seq_type == TYPE_SEQ::END) return;

		std::vector<emath::STNumScaleBin> bins(NUM_CHANGERATIO_BINS);
		bins[0] = emath::NumScaleBin{ 0, std::numeric_limits<DType>::max() };
		bins[1] = emath::NumScaleBin{ 0, 1 };
		int bini = 2;
		DType r = 0.1;
		for (int j = 0; j < NUM_CHANGERATIO_SCALES; j++) {
			for (int mul = 9; mul > 0; mul--) {
				bins[bini] = emath::STNumScaleBin{ 0, r * mul };
				bini += 1;
			}
			r /= 10;
		}

		for (int i = 0; i < info.layers_size(); i++) {

#ifdef __DEBUG_INFO_OUTPUT
			COUT_WARN << "Compute seq of layer: " << info.layers(i).name() << std::endl;
#endif
			if (info.layers(i).type() == "batch_norm") continue;

			if (data_content == TYPE_CONTENT::GRAD && !info.layers(i).grad_size()) return;
			if (data_content == TYPE_CONTENT::WEIGHT && !info.layers(i).weight_size()) return;
			std::vector<DType> ret_data;

			const int idx = index(seq_type, data_content);
			auto ptr = info.mutable_layers(i)->mutable_seq(idx);

			if (ptr->data().size() != 0) continue;

			if (seq_type == TYPE_SEQ::CHANGERATIO) {
				ret_data.resize(NUM_CHANGERATIO_BINS);
				// generate Bins here
				if (data_content == TYPE_CONTENT::WEIGHT) {
					ret_data = emath::changeratio(ArrayToVector(info.layers(i).weight()), 
						ArrayToVector(info.layers(i).grad()), bins);
				}
			}

			if (seq_type == TYPE_SEQ::HISTOGRAM) {
				ret_data.resize(NUM_HISTOGRAM_BINS);
				if (data_content == TYPE_CONTENT::GRAD)
					ret_data = emath::histogram(ArrayToVector(info.layers(i).grad()), NUM_HISTOGRAM_BINS);
				if (data_content == TYPE_CONTENT::WEIGHT)
					ret_data = emath::histogram(ArrayToVector(info.layers(i).weight()), NUM_HISTOGRAM_BINS);
			}

			// copy to data
			for (auto val : ret_data) {
				ptr->mutable_data()->Add(val);
			}
		}
	}

	void Infos::compute_seq_all(TYPE_CONTENT data_content) {

#ifdef __DEBUG_INFO_OUTPUT
		COUT_METD << "func: compute_all_seq" << std::endl;
#endif

		for (unsigned int j = (int)TYPE_SEQ::HISTOGRAM; j < (int)TYPE_SEQ::END; j++) {
#ifdef __DEBUG_INFO_OUTPUT
			__FUNC_TIME_CALL(compute_seq((TYPE_SEQ)j, data_content), name_seq_type[(TYPE_SEQ)j]);
#else
			compute_seq((TYPE_SEQ)j, data_content);
#endif
		}
	}

	void Infos::compute_seq_list(std::vector<TYPE_SEQ> seq_list, TYPE_CONTENT data_content) {

#ifdef __DEBUG_INFO_OUTPUT
		COUT_METD << "func: compute_list_seq" << std::endl;
#endif

		for (unsigned int j = 0; j < seq_list.size(); j++) {
#ifdef __DEBUG_INFO_OUTPUT
			__FUNC_TIME_CALL(compute_seq(seq_list[j], data_content), name_seq_type[seq_list[j]]);
#else
			compute_seq((TYPE_SEQ)seq_list[j], data_content);
#endif
		}
	}
}
