
#include <recorder/recorder.h>
#include <proto/analyzer.pb.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>

namespace analyzer {

	Recorders::Recorders() {

	}

	Recorders::~Recorders() {

	}

	void Recorders::print_total_info() {

		if (recorder.has_name())
			COUT_READ << "Processing file: " << recorder.name() << std::endl;

		for (int i = 0; i < recorder.tuple_size(); i++) {
			COUT_CHEK << "Iteration: " << recorder.tuple(i).iteration()
				<< ", Type: " << recorder.tuple(i).type()
				<< ", Value: " << recorder.tuple(i).value() << std::endl;
		}	

		COUT_SUCC << "Has outputed all infos." << std::endl;
	}

	void Recorders::save_to_file(std::string filename) {

		std::ofstream fp(filename.c_str(), std::ios::binary);

		recorder.SerializeToOstream(&fp);
		fp.close();
	}

	// load from file
	void Recorders::load_from_file(std::string filename) {

		std::ifstream fp(filename.c_str(), std::ios::binary);

		google::protobuf::io::IstreamInputStream fstr(&fp);
		google::protobuf::io::CodedInputStream code_input(&fstr);
		code_input.SetTotalBytesLimit((int)MAX_PROTOFILE_SIZE, (int)MAX_PROTOFILE_SIZE);

		recorder.ParseFromCodedStream(&code_input);

		fp.close();

	}

}