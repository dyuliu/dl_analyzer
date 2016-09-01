
#include <google/gflags/gflags.h>
#include <assert.h>

// DEFINE_string(action, "analyzer", "record, stat, distance, analyzer");
// DEFINE_string(src, "records", "the specify file/folder path");

DEFINE_string(action, "recorder", "recorder, stat, distance, batch");
DEFINE_string(src, "running_info_0.log", "the specify file/folder path");

DEFINE_string(type, "", "specify output type");
DEFINE_string(content, "grad", "grad or weight");

DEFINE_uint64(batchsize, 1, "batch size of records");
DEFINE_uint64(interval, 1, "specify output interval");

DEFINE_bool(all, false, "if output all type info");
DEFINE_bool(db, false, "if upload to db");

DEFINE_string(dbname, "", "database name");

#define CHECK_FLAGS_SRC {if (!FLAGS_src.size()) assert(!"Missing src path!");}
#define CHECK_FLAGS_TYPE {if (!FLAGS_type.size()) assert(!"Missing specify output type!");}
#define CHECK_FLAGS_HP {if (!FLAGS_hp.size()) assert(!"Missing specify hyperparameter type!");}
#define CHECK_FLAGS_BATCHSIZE {if (!FLAGS_batchsize) assert(!"Error input of batchsize!");}
#define CHECK_FLAGS_INTERVAL {if (FLAGS_interval<=0) assert(!"The interval should be larger than 0!");}
#define CHECK_FLAGS_CONTENT {if (FLAGS_content!="grad"&&FLAGS_content!="weight") assert(!"content value is grad or weight");}

#include <iostream>
#include <string>
#include <config.h>

#include <info/info.h>
#include <recorder/recorder.h>
#include <db/include/entry.h>

#include <omp.h>

#include <emath\include\config.h>
#include <emath\include\distance.h>

db::DB *dbInstance;

using analyzer::Infos;
using analyzer::Recorders;

/**********************************************************************
* COMMAND:
* 1. print all type value: 
* -action=recorder -src=*.log -all
* 2. print one type value:  // see recorder.h
* -action=recorder -src=*.log -type=test_error
* 3. set interval
* -interval=10
***********************************************************************/
void analyzer_recorder() {
	CHECK_FLAGS_SRC;

	Recorders recorder(FLAGS_src);
	
	if (FLAGS_all) {
		recorder.print_total_info();
		if (FLAGS_db) {
			dbInstance->bindRecorder(recorder.get());
			dbInstance->importRecorderInfo();
		}
	}
	else {
		CHECK_FLAGS_TYPE;
		CHECK_FLAGS_INTERVAL;
		auto val = recorder.get_specify_type(FLAGS_type);
		for (auto &item : val) {
			COUT_CHEK << "iter: " << std::get<0>(item) << ", type: " << std::get<1>(item) << ", val: " << std::get<2>(item) << std::endl;
		}
		// recorder.print_specify_type(FLAGS_type, FLAGS_interval);
	}

	
}

/**********************************************************************
* COMMAND:
* 1. print all type value: (all stat of grad and weight)
* -action=analyzer -src=*.info -all
* 2. print one type value:  // see analyzer.h
* -action=analyzer -src=*.info -type=max
* 3. other set
* -content=weight/grad (grad)
***********************************************************************/
void analyzer_stat() {
	CHECK_FLAGS_SRC;

	Infos info(FLAGS_src);

	if (FLAGS_all) {
		info.compute_stat_all(Infos::TYPE_CONTENT::GRAD);
		info.compute_stat_all(Infos::TYPE_CONTENT::WEIGHT);
		// info.print_total_info();
		if (FLAGS_db) {
			dbInstance->bindInfo(&info.get());
		}
	}
	else {
		CHECK_FLAGS_TYPE;
		CHECK_FLAGS_CONTENT;
		auto type = info.to_type<Infos::TYPE_STAT>(FLAGS_type);
		auto content = info.to_type<Infos::TYPE_CONTENT>(FLAGS_content);
		info.compute_stat(type, content);
		info.print_stat_info(content);
	}
}

void analyzer_seq() {

	CHECK_FLAGS_SRC;

	Infos info(FLAGS_src);

	if (FLAGS_all) {
		info.compute_seq_all(Infos::TYPE_CONTENT::GRAD);
		info.compute_seq_all(Infos::TYPE_CONTENT::WEIGHT);
		// info.print_seq_info(Infos::TYPE_CONTENT::GRAD);
		// info.print_seq_info(Infos::TYPE_CONTENT::WEIGHT);
		if (FLAGS_db) {

		}
	}
	else {
		CHECK_FLAGS_TYPE;
		CHECK_FLAGS_CONTENT;
		auto type = info.to_type<Infos::TYPE_SEQ>(FLAGS_type);
		auto content = info.to_type<Infos::TYPE_CONTENT>(FLAGS_content);
		info.compute_seq(type, content);
		info.print_seq_info(content);
	}
}


/**********************************************************************
* COMMAND:
***********************************************************************/
void analyzer_layerinfo() {
	CHECK_FLAGS_SRC;

	if (FLAGS_db) {
		Infos info(FLAGS_src);
		dbInstance->bindInfo(&info.get());
		dbInstance->importLayerAttrs();
	}
}

static inline void analyzer_batch_db(std::vector<Infos> &batch_infos) {
	int batch_size = batch_infos.size();
	for (int x = 0; x < batch_size - 1; x++) {
		batch_infos[x].get().set_worker_id(batch_infos[x].get().worker_id() + 1);
		dbInstance->bindInfo(&batch_infos[x].get());
		dbInstance->importAll();
		//COUT_CHEK << "work_id: " << (batch_infos[x].get().worker_id()) << std::endl;
	}
	batch_infos[batch_size - 1].get().set_worker_id(0);
	dbInstance->bindInfo(&batch_infos[batch_size - 1].get());
	dbInstance->importAll();
	//COUT_CHEK << "work_id: " << (batch_infos[batch_size - 1].get().worker_id()) << std::endl;
}

static inline void analyzer_batch_distance(std::vector<Infos> &batch_infos) {
	auto last_batch = batch_infos.size()-1;
	for (int idx = 0; idx < last_batch; idx++) {
		//auto content = batch_infos[idx].to_type<Infos::TYPE_CONTENT>(FLAGS_content);
		__FUNC_TIME_CALL(batch_infos[idx].compute_dist_all(Infos::TYPE_CONTENT::GRAD, batch_infos[last_batch]), "Process file with distance " + batch_infos[idx].get().filename());
		__FUNC_TIME_CALL(batch_infos[idx].compute_dist_all(Infos::TYPE_CONTENT::WEIGHT, batch_infos[last_batch]), "Process file with distance " + batch_infos[idx].get().filename());
	}
}

static inline void analyzer_batch(std::vector<Infos> &batch_infos) {
	int batch_size = batch_infos.size();
	for (int idx = 0; idx < batch_size; idx++) {
		auto &info = batch_infos[idx];
		__FUNC_TIME_CALL(info.compute_stat_all(Infos::TYPE_CONTENT::GRAD), "Process file with grad " + info.get().filename());
		
		if (idx == batch_size - 1){
			__FUNC_TIME_CALL(info.compute_stat_all(Infos::TYPE_CONTENT::WEIGHT), "Process file with weight " + info.get().filename());
			// copy weight statistic to all file?
			// compute all distance
			if (batch_size > 1)
				analyzer_batch_distance(batch_infos);
		}
	}
}

/**********************************************************************
* COMMAND:
***********************************************************************/
void analyzer_tools() {
	CHECK_FLAGS_SRC;
	
	if (!analyzer::filesystem::exist(FLAGS_src.c_str()))
		throw("Error: Missing folder path!");
	auto files = analyzer::filesystem::get_files(FLAGS_src.c_str(), "*.info", false);

	CHECK_FLAGS_BATCHSIZE;
	int batch_size = FLAGS_batchsize;

//#pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < files.size(); i += batch_size) {
		std::vector<Infos> batch_infos;
		COUT_CHEK << "Filename: " << files[i] << ", ratio:" << 100*i/float(files.size()) << std::endl;
		
		if (i + batch_size > files.size()) continue;

		for (int idx_batch = i; idx_batch < i + batch_size; idx_batch++)
			batch_infos.push_back(Infos(files[idx_batch], batch_size));
		
		analyzer_batch(batch_infos);

		if (FLAGS_db) {
			analyzer_batch_db(batch_infos);
		}

		batch_infos.clear();
	}
}

/**********************************************************************
* NEW:
***********************************************************************/
void adjacent_distance() {
	CHECK_FLAGS_SRC;

	if (!analyzer::filesystem::exist(FLAGS_src.c_str()))
		throw("Error: Missing folder path!");
	auto files = analyzer::filesystem::get_files(FLAGS_src.c_str(), "*.info", false);

	CHECK_FLAGS_BATCHSIZE;
	int batch_size = FLAGS_batchsize;

//#pragma omp parallel for schedule(dynamic)
	for (int i = 2 * batch_size - 1; i < files.size(); i += batch_size) {
		COUT_CHEK << "Filename: " << files[i-batch_size] << ", ratio:" << 100 * i / float(files.size()) / batch_size << std::endl;
		COUT_CHEK << "Filename: " << files[i] << ", ratio:" << 100 * i / float(files.size()) / batch_size << std::endl << std::endl;

		if (i + batch_size > files.size()) continue;

		auto left = Infos(files[i], batch_size);
		auto right = Infos(files[i + batch_size], batch_size);

		left.compute_dist_all(Infos::TYPE_CONTENT::WEIGHT, right);
		left.compute_dist_all(Infos::TYPE_CONTENT::GRAD, right);

		// left.print_distance_info(Infos::TYPE_CONTENT::WEIGHT);
		// left.print_distance_info(Infos::TYPE_CONTENT::GRAD);

		if (FLAGS_db) {
			dbInstance->bindInfo(&left.get());
			dbInstance->importDist(Infos::TYPE_DISTANCE::EUCLIDEAN, Infos::TYPE_CONTENT::GRAD, "GradDistAdjEuclidean");
			dbInstance->importDist(Infos::TYPE_DISTANCE::EUCLIDEAN, Infos::TYPE_CONTENT::WEIGHT, "WeightDistAdjEuclidean");
			dbInstance->importDist(Infos::TYPE_DISTANCE::COSINE, Infos::TYPE_CONTENT::GRAD, "GradDistAdjCosine");
			dbInstance->importDist(Infos::TYPE_DISTANCE::COSINE, Infos::TYPE_CONTENT::WEIGHT, "WeightDistAdjCosine");
		}
	}

}

int main(int argc, char *argv[]) {

	gflags::ParseCommandLineFlags(&argc, &argv, true);

	if (FLAGS_db) dbInstance = new db::DB(FLAGS_dbname);

	if (FLAGS_action == "stat") {
		analyzer_stat();
	}
	else if (FLAGS_action == "seq") {
		analyzer_seq();
	}
	else if (FLAGS_action == "batch") {
		analyzer_tools();
	}
	else if (FLAGS_action == "adjacent_distance") {
		adjacent_distance();
	}
	else if (FLAGS_action == "recorder") {
		analyzer_recorder();
	}
	else if (FLAGS_action == "layerinfo") {
		analyzer_layerinfo();
	}

	gflags::ShutDownCommandLineFlags();
	return 0;
}