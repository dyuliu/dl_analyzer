
#include <info/info.h>
#include <fstream>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <exception>

namespace analyzer {

	Infos::Infos() {
		init_type_name();
		init_stat();
	}

	Infos::Infos(std::string path) {
		if (!filesystem::exist(path.c_str()))
			THROW("Error: Missing input file!");
		
		load_from_file(path);
		init_type_name();
		init_stat();
	}

	Infos::Infos(std::string path, int rank_size) {
		if (!filesystem::exist(path.c_str()))
			THROW("Error: Missing input file!");

		load_from_file(path);
		init_type_name();
		init_stat();
	}

	void Infos::init_type_name() {
		name_content_type = std::map<TYPE_CONTENT, std::string> {
			{ TYPE_CONTENT::GRAD, "grad" },
			{ TYPE_CONTENT::WEIGHT, "weight" }
		};

		name_distance_type = std::map<TYPE_DISTANCE, std::string> {
			{ TYPE_DISTANCE::EUCLIDEAN,			"Euclidean" },
			{ TYPE_DISTANCE::COSINE,			"Cosine" },
			{ TYPE_DISTANCE::MANHATTAN,			"Manhattan" },
			{ TYPE_DISTANCE::CORRELATION,		"Correlation" },
			{ TYPE_DISTANCE::EUCLIDEAN_NORM,	"Euclidean_Norm" },
			{ TYPE_DISTANCE::COSINE_NORM,		"Cosine_Norm" }
		};

		name_stat_type = std::map<TYPE_STAT, std::string> {
			{ TYPE_STAT::MAX,	  "max" },
			{ TYPE_STAT::MIN,	  "min" },
			{ TYPE_STAT::MEAN,	  "mean" },
			{ TYPE_STAT::SUM,	  "sum" },
			{ TYPE_STAT::VAR,	  "var" },
			{ TYPE_STAT::STD,	  "std" },
			{ TYPE_STAT::NORM_0,  "norm0" },
			{ TYPE_STAT::NORM_1,  "norm1" },
			{ TYPE_STAT::NORM_2,  "norm2" },
			{ TYPE_STAT::QUANTILE_1_2, "mid" },
			{ TYPE_STAT::QUANTILE_1_4, "quarter1" },
			{ TYPE_STAT::QUANTILE_3_4, "quarter3" }
		};

		name_seq_type = std::map<TYPE_SEQ, std::string> {
			{ TYPE_SEQ::HISTOGRAM,  "histogram"},
			{ TYPE_SEQ::CHANGERATIO, "changeratio" }
		};

		name_cluster_type = std::map<TYPE_CLUSTER, std::string> {
			{ TYPE_CLUSTER::KMEANS, "kmeans"}
		};

		name_stat_kernel_type = std::map<TYPE_STAT_KERNEL, std::string> {
			{ TYPE_STAT_KERNEL::CR_NORM_1, "cr_norm1"},
			{ TYPE_STAT_KERNEL::CR_NORM_2, "cr_norm2"}
		};
	}

	void Infos::init_stat() {
		// init computational related parameters
		for (int i = 0; i < info.layers_size(); i++) {
			for (int idx = (int)TYPE_CONTENT::GRAD; idx < (int)TYPE_CONTENT::END; idx++) {
				for (int j = (int)TYPE_STAT::MAX; j < (int)TYPE_STAT::END; j++) {
					auto ptr = info.mutable_layers(i)->add_stat();
					ptr->set_value(0.0);
					ptr->set_type(name_stat_type[(TYPE_STAT)j].c_str());
					ptr->set_content(name_content_type[(TYPE_CONTENT)idx].c_str());
				}
				for (int j = (int)TYPE_DISTANCE::EUCLIDEAN; j < (int)TYPE_DISTANCE::END; j++) {
					auto ptr = info.mutable_layers(i)->add_distance();
					ptr->set_value(0.0);
					ptr->set_type(name_distance_type[(TYPE_DISTANCE)j].c_str());
					ptr->set_content(name_content_type[(TYPE_CONTENT)idx].c_str());
				}
				for (int j = (int)TYPE_SEQ::HISTOGRAM; j < (int)TYPE_SEQ::END; j++) {
					auto ptr = info.mutable_layers(i)->add_seq();
					ptr->set_value(0.0);
					ptr->set_type(name_seq_type[(TYPE_SEQ)j].c_str());
					ptr->set_content(name_content_type[(TYPE_CONTENT)idx].c_str());
				}
				for (int j = (int)TYPE_CLUSTER::KMEANS; j < (int)TYPE_CLUSTER::END; j++) {
					auto ptr = info.mutable_layers(i)->add_cluster();
					ptr->set_type(name_cluster_type[(TYPE_CLUSTER)j].c_str());
					ptr->set_num(0);
					ptr->set_content(name_content_type[(TYPE_CONTENT)idx].c_str());
				}
				for (int j = (int)TYPE_STAT_KERNEL::CR_NORM_1; j < (int)TYPE_STAT_KERNEL::END; j++) {
					auto ptr = info.mutable_layers(i)->add_stat_kernel();
					ptr->set_value(0.0);
					ptr->set_type(name_stat_kernel_type[(TYPE_STAT_KERNEL)j].c_str());
					ptr->set_content(name_content_type[(TYPE_CONTENT)idx].c_str());
				}
			}
		}
	}

	void Infos::load_from_file(std::string filename) {

		std::ifstream fp(filename.c_str(), std::ios::binary);

		google::protobuf::io::IstreamInputStream fstr(&fp);
		google::protobuf::io::CodedInputStream code_input(&fstr);
		code_input.SetTotalBytesLimit((int)MAX_PROTOFILE_SIZE, (int)MAX_PROTOFILE_SIZE);

		info.ParseFromCodedStream(&code_input);
		fp.close();
	}

	// dump to file
	void Infos::save_to_file(std::string foldname) {

		if (!filesystem::exist(foldname.c_str()))
			filesystem::create_directory(foldname.c_str());

		std::string filename = foldname + "/" + info.filename() + ".info";
		std::ofstream fp(filename.c_str(), std::ios::binary);

		info.SerializeToOstream(&fp);
		fp.close();
	}

	void Infos::copy_hyperparam(Infos &other, TYPE_CONTENT content_type, HyperParam hp) {

		CHECK_EQ(info.layers_size(), other.get().layers_size());

		for (int i = 0; i < info.layers_size(); i++) {
			CHECK_EQ(info.layers(i).stat_size(), other.get().layers(i).stat_size());
			if (hp == HyperParam::STAT) {
				for (int j = (int)TYPE_STAT::MAX; j < (int)TYPE_STAT::END; j++) {
					auto idx = index((TYPE_STAT)j, content_type);
					info.mutable_layers(i)->mutable_stat(idx)->CopyFrom(other.get().layers(i).stat(idx));
				}
			}
			else if (hp == HyperParam::DISTANCE) {
				for (int j = (int)TYPE_DISTANCE::EUCLIDEAN; j < (int)TYPE_DISTANCE::END; j++) {
					auto idx = index((TYPE_DISTANCE)j, content_type);
					info.mutable_layers(i)->mutable_distance(idx)->CopyFrom(other.get().layers(i).distance(idx));
				}
			}
			else if (hp == HyperParam::SEQ) {
				for (int j = (int)TYPE_SEQ::HISTOGRAM; j < (int)TYPE_SEQ::END; j++) {
					auto idx = index((TYPE_SEQ)j, content_type);
					info.mutable_layers(i)->mutable_seq(idx)->CopyFrom(other.get().layers(i).seq(idx));
				}
			}
		}

	}

	template<typename T>
	inline static T type_search(std::string e, std::map<T, std::string> s) {
		for (auto name : s)
			if (e == name.second) 
				return name.first;
		THROW("Could not find specify type!");
	}

	template<>
	Infos::TYPE_STAT Infos::to_type<Infos::TYPE_STAT>(std::string in) {
		return type_search<TYPE_STAT>(in, name_stat_type);
	}

	template<>
	Infos::TYPE_CONTENT Infos::to_type<Infos::TYPE_CONTENT>(std::string in) {
		return type_search<TYPE_CONTENT>(in, name_content_type);
	}

	template<>
	Infos::TYPE_DISTANCE Infos::to_type<Infos::TYPE_DISTANCE>(std::string in) {
		return type_search<TYPE_DISTANCE>(in, name_distance_type);
	}

	template<>
	Infos::TYPE_SEQ Infos::to_type<Infos::TYPE_SEQ>(std::string in) {
		return type_search<TYPE_SEQ>(in, name_seq_type);
	}

	template<>
	Infos::TYPE_CLUSTER Infos::to_type<Infos::TYPE_CLUSTER>(std::string in) {
		return type_search<TYPE_CLUSTER>(in, name_cluster_type);
	}

	template<>
	Infos::TYPE_STAT_KERNEL Infos::to_type<Infos::TYPE_STAT_KERNEL>(std::string in) {
		return type_search<TYPE_STAT_KERNEL>(in, name_stat_kernel_type);
	}
}