
#include <analyzer.h>
#include <fstream>
#include <iomanip>
#include <google/protobuf/io/zero_copy_stream_impl.h>


namespace analyzer {

	void Infos::init(std::string path) {

		if (!filesystem::exist(path.c_str())) return;

		load_from_file(path);

		// init computational related parameters
		for (int i = 0; i < info.layers_size(); i++) {
			for (int j = LAYER_MAX; j < STAT_NUM_MAX; j++) {
				info.mutable_layers(i)->add_stat(0.0);
			}
			for (int j = LAYER_EUCLIDEAN; j < DISTANCE_NUM_MAX; j++) {
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

	// dump to file
	void Infos::save_to_file(std::string foldname) {

		if (!filesystem::exist(foldname.c_str()))
			filesystem::create_directory(foldname.c_str());

		std::string filename = foldname + "/" + info.filename() + ".info";
		std::ofstream fp(filename.c_str(), std::ios::binary);
		
		info.SerializeToOstream(&fp);
		fp.close();
	}

}