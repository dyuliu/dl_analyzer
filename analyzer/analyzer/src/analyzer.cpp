
#include <analyzer.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <google/protobuf/io/zero_copy_stream_impl.h>

#include <emath/include/config.h>
#include <emath/include/statistic.h>

namespace analyzer {

	void Infos::init(std::string path) {

		if (!filesystem::exist(path.c_str())) return;

		load_from_file(path);

		// init computational related parameters
		for (int i = 0; i < info.layers_size(); i++) {
			for (int j = LAYER_STAT_MAX; j < STAT_NUM_MAX; j++) {
				info.mutable_layers(i)->add_stat(0.0);
			}
			for (int j = LAYER_DIS_EUCLIDEAN; j < DISTANCE_NUM_MAX; j++) {
				info.mutable_layers(i)->add_distance(0.0);
			}
		}
	}

	void Infos::load_from_file(std::string filename) {

		std::ifstream fp(filename.c_str(), std::ios::binary);

		google::protobuf::io::IstreamInputStream fstr(&fp);
		google::protobuf::io::CodedInputStream code_input(&fstr);
		code_input.SetTotalBytesLimit((int)5368709120, (int)5368709120);

		info.ParseFromCodedStream(&code_input);

		fp.close();
	}

	void Infos::print() {

		print_file_info();
		print_conv_layer_info();
		print_stat_info(DATA_CONTENT::CONTENT_GRAD);

	}

	void Infos::print_file_info() {
		
		std::cout << std::endl;
		COUT_METD << "----- PRINT FILE INFO -----" << std::endl;

		// print basic information
		if (info.has_filename())
			COUT_CHEK << "Running file: " << info.filename();
		if (info.has_iteration())
			std::cout << ", iteration: " << info.iteration();
		if (info.has_worker_id())
			std::cout << ", worker id: " << info.worker_id();
		if (info.has_sim_id())
			std::cout << ", sim id: " << info.sim_id();

		std::cout << std::endl;
	}

	void Infos::print_conv_layer_info() {

		std::cout << std::endl;
		COUT_METD << "----- PRINT LAYER INFO (NO BN) -----" << std::endl;

		// print layer infomation
		size_t weight_size = 0, grad_size = 0;
		for (int i = 0; i < info.layers_size(); i++) {

			weight_size += info.layers(i).weight_size();
			grad_size += info.layers(i).grad_size();

			if (info.layers(i).type() == "batch_norm") continue;

			COUT_READ << std::setw(3) << i << ", " << info.layers(i).name() << ", " << info.layers(i).type() << ", ("
				<< info.layers(i).num() << " " << info.layers(i).channels() << " "
				<< info.layers(i).height() << " " << info.layers(i).width() << "), "
				<< "weight size: " << info.layers(i).weight_size() << ", "
				<< "grad size: " << info.layers(i).grad_size() << std::endl;
		}
		COUT_CHEK << "Total weight size: " << weight_size << ", Total grad size: " << grad_size << std::endl;
	}

	void Infos::print_stat_info(DATA_CONTENT data_content) {

		std::cout << std::endl;
		if (data_content == CONTENT_GRAD)
			COUT_METD << "----- PRINT GRAD STAT INFO -----" << std::endl;

		if (data_content == CONTENT_WEIGHT)
			COUT_METD << "----- PRINT WEIGHT STAT INFO -----" << std::endl;

		// print stat infomation
		size_t weight_size = 0, grad_size = 0;
		for (int i = 0; i < info.layers_size(); i++) {

			weight_size += info.layers(i).weight_size();
			grad_size += info.layers(i).grad_size();

			if (info.layers(i).type() == "batch_norm") continue;

			COUT_READ << std::setw(3) << i << ", " << info.layers(i).name() << ", " << info.layers(i).type() << std::endl;
			std::cout << std::endl;
//			COUT_CHEK << std::setw(10) << "MIN: " << std::setw(10) << info.layers(i).stat[STAT_TYPE::LAYER_STAT_MAX] << std::endl;
				
		}
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

	void Infos::compute_stat(STAT_TYPE stat_type, DATA_CONTENT data_content) {

		if (stat_type == STAT_NUM_MAX) return;

		for (int i = 0; i < info.layers_size(); i++) {
			
			if (stat_type == LAYER_STAT_MAX) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_MAX] = emath::max(ArrayToVector(info.layers(i).grad()));
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_MAX] = emath::max(ArrayToVector(info.layers(i).weight()));
			}

			if (stat_type == LAYER_STAT_MIN) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_MIN] = emath::min(ArrayToVector(info.layers(i).grad()));
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_MAX] = emath::min(ArrayToVector(info.layers(i).weight()));
			}

			if (stat_type == LAYER_STAT_MEAN) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_MEAN] = emath::mean(ArrayToVector(info.layers(i).grad()));
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_MEAN] = emath::mean(ArrayToVector(info.layers(i).weight()));
			}

			if (stat_type == LAYER_STAT_STD) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_STD] = emath::std(ArrayToVector(info.layers(i).grad()));
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_STD] = emath::std(ArrayToVector(info.layers(i).weight()));
			}

			if (stat_type == LAYER_STAT_SUM) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_SUM] = emath::sum(ArrayToVector(info.layers(i).grad()));
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_SUM] = emath::sum(ArrayToVector(info.layers(i).weight()));
			}

			if (stat_type == LAYER_STAT_VAR) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_VAR] = emath::var(ArrayToVector(info.layers(i).grad()));
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_VAR] = emath::var(ArrayToVector(info.layers(i).weight()));
			}

			if (stat_type == LAYER_STAT_NORM_0) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_NORM_0] = emath::norm(ArrayToVector(info.layers(i).grad()), 0);
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_NORM_0] = emath::norm(ArrayToVector(info.layers(i).weight()), 0);
			}

			if (stat_type == LAYER_STAT_NORM_1) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_NORM_1] = emath::norm(ArrayToVector(info.layers(i).grad()), 1);
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_NORM_1] = emath::norm(ArrayToVector(info.layers(i).weight()), 1);
			}

			if (stat_type == LAYER_STAT_NORM_2) {
				if (data_content == CONTENT_GRAD)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_NORM_2] = emath::norm(ArrayToVector(info.layers(i).grad()), 2);
				if (data_content == CONTENT_WEIGHT)
					info.mutable_layers(i)->mutable_stat()->mutable_data()[LAYER_STAT_NORM_2] = emath::norm(ArrayToVector(info.layers(i).weight()), 2);
			}
		}
	}

	void Infos::compute_distance(DISTANCE_TYPE distrance_type) {}

}