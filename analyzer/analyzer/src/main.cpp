
#include <gflags.h>

#include <iostream>
#include <string>
#include <config.h>

#include <analyzer/analyzer.h>
#include <recorder/recorder.h>
#include <db/include/entry.h>

#include <omp.h>

db::DB *dbInstance;

DEFINE_string(action, "all", "");
// DEFINE_string(src, "records/00000000_000_008.info", "");
// DEFINE_string(src, "caffepro_log", "");
DEFINE_string(src, "records", "");
DEFINE_uint64(batchsize, 9, "batch size of records");

using analyzer::Infos;
using analyzer::Recorders;

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

	Recorders recorder;
	recorder.load_from_log_file(FLAGS_src, Recorders::TYPE_FRAMEWORK::CAFFEPRO);

}

void read_from_file() {

	COUT_METD << "Running Function -> Read From File" << std::endl;
	if (FLAGS_src == "") {
		COUT_ERRO << "Missing file path, please set -src" << std::endl;
		return;
	}

	Infos info(FLAGS_src, 8);
	//info.compute_list({ Infos::TYPE_STAT::MEAN, Infos::TYPE_STAT::VAR}, Infos::TYPE_CONTENT::GRAD);
	//info.compute_all(analyzer::Infos::TYPE_CONTENT::GRAD);
	//info.compute_all(analyzer::Infos::TYPE_CONTENT::WEIGHT);
	//info.print_total_info();
	dbInstance->bindInfo(info.getInfo());
	dbInstance->importLayerAttrs();
	//dbInstance->importGradient("grad");
	//dbInstance->importStat(Infos::TYPE_STAT::MEAN, Infos::TYPE_CONTENT::GRAD);
	//dbInstance->importAll();
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
	Recorders recorder(FLAGS_src);
	// recorder.print_specify_type("test_error", 10);
	// recorder.print_specify_type(Recorders::TYPE_RECORD::FORWARD_TIME);
	dbInstance->bindRecorder(recorder.getRecorder());
	dbInstance->importRecorderInfo();
}

void compute_all_batch(std::string foldername, int batch_size) {
	if (!analyzer::filesystem::exist(foldername.c_str()))
		throw("Error: Missing folder path!");
	auto files = analyzer::filesystem::get_files(foldername.c_str(), "*.info", false);

#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < files.size(); i += batch_size) {
		std::vector<Infos> batch_infos;

		// depleted
		if (i + batch_size > files.size()) continue;

		// process
		for (int j = i; j < i + batch_size; j++) {
			Infos info(files[j], batch_size - 1);
			auto clock_t = clock();
			COUT_READ << "Process file with grad: " << info.get().filename() << std::endl;
			info.compute_all(Infos::TYPE_CONTENT::GRAD);
			COUT_READ << "All grad statistic has been computed, spend: " << clock() - clock_t << std::endl;

			// process last one
			if (j == i + batch_size - 1) {
				clock_t = clock();
				COUT_CHEK << "Process file with weight: " << info.get().filename() << std::endl;
				info.compute_all(Infos::TYPE_CONTENT::WEIGHT);
				COUT_CHEK << "All weight statistic has been computed, spend: " << clock() - clock_t << std::endl;
				for (int m = 0; m < batch_size - 1; m++) {
					// copy weight hyper statistic
					clock_t = clock();
					COUT_CHEK << "File: " << batch_infos[m].get().filename() << " prepare to compute distance from aggregate!" << std::endl;
					batch_infos[m].copy_hyperparam(info, Infos::TYPE_CONTENT::WEIGHT, Infos::HyperParam::STAT);
					// calcute distance to aggregate
					// batch_infos[m].compute(Infos::TYPE_DISTANCE::EUCLIDEAN, Infos::TYPE_CONTENT::GRAD, info);
					batch_infos[m].compute_all(Infos::TYPE_CONTENT::GRAD, info);
					batch_infos[m].compute_all(Infos::TYPE_CONTENT::WEIGHT, info);
					COUT_CHEK << "Finish compute distance from aggregate, spend: " << clock() - clock_t << std::endl;
				}
			}
			batch_infos.push_back(info);
		}

		for (int x = 0; x < batch_size - 1; x++) {
			batch_infos[x].get().set_worker_id(batch_infos[x].get().worker_id() + 1);
			dbInstance->bindInfo(batch_infos[x].getInfo());
			dbInstance->importAll();
			//COUT_CHEK << "work_id: " << (batch_infos[x].get().worker_id()) << std::endl;
		}
		batch_infos[batch_size - 1].get().set_worker_id(0);
		dbInstance->bindInfo(batch_infos[batch_size - 1].getInfo());
		dbInstance->importAll();
		//COUT_CHEK << "work_id: " << (batch_infos[batch_size - 1].get().worker_id()) << std::endl;

		COUT_SUCC << "FINISH A GROUP OF FILES" << std::endl;
	}
	COUT_SUCC << "FINISH ALL JOBS" << std::endl;
}

static void failureFunction() { exit(0); }

int main(int argc, char *argv[]) {

	gflags::SetVersionString(version_info);
	gflags::ParseCommandLineFlags(&argc, &argv, true);
	gflags::SetUsageMessage(help_info);

	dbInstance = new db::DB("cnnvis");

	if (FLAGS_action == "test") test();
	if (FLAGS_action == "test_record") test_record();
	if (FLAGS_action == "read_from_file") read_from_file();
	if (FLAGS_action == "read_from_folder") read_from_folder();
	if (FLAGS_action == "all") compute_all_batch(FLAGS_src, FLAGS_batchsize);

	gflags::ShutDownCommandLineFlags();
	system("pause");
	return 0;
}