
#include <gflags.h>

#include <iostream>
#include <string>
#include <config.h>

#include <analyzer/analyzer.h>
#include <recorder/recorder.h>

DEFINE_string(action, "test_record", "");
// DEFINE_string(src, "records/00000000_000_000.info", "");
DEFINE_string(src, "running_info.log", "");

void test() {
	if (FLAGS_src == "") {
		COUT_ERRO << "Missing file path, please set --src" << std::endl;
		return;
	}

	analyzer::Infos info;
	info.init(FLAGS_src);
	info.compute_all_stat(analyzer::Infos::DATA_CONTENT::CONTENT_GRAD);
	info.print_total_info();
}

void test_record() {
	if (FLAGS_src == "") {
		COUT_ERRO << "Missing file path, please set --src" << std::endl;
		return;
	}
	analyzer::Recorders recorder;
	recorder.load_from_file(FLAGS_src);
	recorder.print_specify_type(analyzer::Recorders::TRAIN_LOSS, 30);
}

static void failureFunction() { exit(0); }

int main(int argc, char *argv[]) {

	gflags::ParseCommandLineFlags(&argc, &argv, true);

	if (FLAGS_action == "test") test();
	if (FLAGS_action == "test_record") test_record();

	gflags::ShutDownCommandLineFlags();
	system("pause");
	return 0;
}