
#include <recorder/recorder.h>
#include <proto/analyzer.pb.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <fstream>

namespace analyzer {

	Recorders::Recorders() {
		name_of_type = std::map <TYPE_RECORD, std::string> {
			{ TYPE_RECORD::TRAIN_ERROR,   "train_error" },
			{ TYPE_RECORD::TRAIN_LOSS,	  "train_loss" },
			{ TYPE_RECORD::TEST_ERROR,	  "test_error" },
			{ TYPE_RECORD::TEST_LOSS,	  "test_loss" },
			{ TYPE_RECORD::FORWARD_TIME,  "forward_time" },
			{ TYPE_RECORD::BACKWARD_TIME, "backward_time" },
			{ TYPE_RECORD::UPDATE_TIME,	  "update_time" },
			{ TYPE_RECORD::LEARNING_RATE, "learning_rate" }
		};
	}

	Recorders::Recorders(std::string path) {
		
		if (!filesystem::exist(path.c_str()))
			throw("Error: Missing input file!");

		load_from_file(path);

		Recorders::Recorders();
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

	void Recorders::print_specify_type(TYPE_RECORD record_type, int iterval) {

		COUT_READ << "Processing file: " << recorder.name() << std::endl;

		for (int i = 0; i < recorder.tuple_size(); i++) {
			if ( recorder.tuple(i).iteration()%iterval == 0 && recorder.tuple(i).type() == name_of_type[record_type]) {
				COUT_CHEK << "Iteration: " << recorder.tuple(i).iteration()
					<< ", " << recorder.tuple(i).type() << " = " << recorder.tuple(i).value() << std::endl;
			}
		}

		COUT_SUCC << "Has outputed all infos." << std::endl;
	}

	void Recorders::print_specify_type(std::string record_type, int iterval) {

		COUT_READ << "Processing file: " << recorder.name() << std::endl;

		for (int i = 0; i < recorder.tuple_size(); i++) {
			if (recorder.tuple(i).iteration() % iterval == 0 && recorder.tuple(i).type() == record_type) {
				COUT_CHEK << "Iteration: " << recorder.tuple(i).iteration()
					<< ", " << recorder.tuple(i).type() << " = " << recorder.tuple(i).value() << std::endl;
			}
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