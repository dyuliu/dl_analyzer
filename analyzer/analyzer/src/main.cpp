
#include <gflags.h>

#include <iostream>
#include <string>
#include <config.h>

#include <analyzer/analyzer.h>
#include <recorder/recorder.h>
#include <db/include/entry.h>
db::DB *dbInstance;

DEFINE_string(action, "test_record", "");
// DEFINE_string(src, "records/00000000_000_000.info", "");
DEFINE_string(src, "running_info.log", "");

const std::string version_info =
R"(

Versions view:
    Analyzer version:  v1.0.0
    Math lib version:  v1.0.0
    DB lib version:    v1.0.0
)";

const std::string help_info =
R"(
Analyzer Commands:
	
Syntax:
    analyzer -action=key1 -src=key2
	
nParameters:
    -help         - Display this help message
    -action       - Running function: test_record, test
    -src          - Source of folder or file
	
For more information about Analyzer Tools,
See http://msra-dl:5000/
)";

void test() {
	if (FLAGS_src == "") {
		COUT_ERRO << "Missing file path, please set -src" << std::endl;
		return;
	}
}

void read_from_file() {

	COUT_METD << "Running Function -> Read From File" << std::endl;

	analyzer::Infos info;
	info.init(FLAGS_src);
	info.compute_stat(analyzer::Infos::LAYER_STAT_MAX, analyzer::Infos::DATA_CONTENT::CONTENT_GRAD);
	info.print_total_info();
	dbInstance = new db::DB();
	dbInstance->importAllStats();
}

void read_from_folder() {

	COUT_METD << "Running Function -> Read From Folder" << std::endl;

	if (!analyzer::filesystem::exist(FLAGS_src.c_str())) {
		COUT_ERRO << "Missing file path, please set -src" << std::endl;
		return;
	}
	
	auto file_list = analyzer::filesystem::get_files(FLAGS_src.c_str(), "*.*", false);
	for (auto file : file_list) {
		FLAGS_src = file;
		read_from_file();
	}

}

void test_record() {
	if (FLAGS_src == "") {
		COUT_ERRO << "Missing file path, please set -src" << std::endl;
		return;
	}
	analyzer::Recorders recorder;
	recorder.load_from_file(FLAGS_src);
	// recorder.print_specify_type("test_error", 10);
	recorder.print_specify_type(analyzer::Recorders::FORWARD_TIME);
}

static void failureFunction() { exit(0); }

int main(int argc, char *argv[]) {

	gflags::SetVersionString(version_info);
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	gflags::SetUsageMessage(help_info);

	if (FLAGS_action == "test") test();
	if (FLAGS_action == "test_record") test_record();
	if (FLAGS_action == "read_from_file") read_from_file();
	if (FLAGS_action == "read_from_folder") read_from_folder();

	gflags::ShutDownCommandLineFlags();
	system("pause");
	return 0;
}