

#pragma once

#include <proto/analyzer.pb.h>
#include <config.h>

namespace analyzer {

	class Recorders {

	public:

		Recorders();
		~Recorders();

	public:

		// print related
		void print_total_info();

		// dump to file
		void save_to_file(std::string filename);

		// load from file
		void load_from_file(std::string filename);

	public:

		Recorder& get() { return recorder; }

	private:
		Recorder recorder;

	};

}