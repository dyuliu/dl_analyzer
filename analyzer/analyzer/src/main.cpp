
#include <gflags.h>
#include <assert.h>

// DEFINE_string(action, "analyzer", "record, stat, distance, analyzer");
// DEFINE_string(src, "records", "the specify file/folder path");

DEFINE_string(action, "recorder", "recorder, stat, distance, batch");
DEFINE_string(src, "running_info_0.log", "the specify file/folder path");

DEFINE_string(type, "", "specify output type");
DEFINE_string(content, "grad", "grad or weight");

DEFINE_uint64(batchsize, 3, "batch size of records");
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

#include <analyzer/analyzer.h>
#include <recorder/recorder.h>
#include <db/include/entry.h>

#include <omp.h>

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
		//recorder.print_total_info();
		if (FLAGS_db) {
			dbInstance->bindRecorder(recorder.getRecorder());
			dbInstance->importRecorderInfo();
		}
	}
	else {
		CHECK_FLAGS_TYPE;
		CHECK_FLAGS_INTERVAL;
		recorder.print_specify_type(FLAGS_type, FLAGS_interval);
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
		info.compute_all(Infos::TYPE_CONTENT::GRAD);
		info.compute_all(Infos::TYPE_CONTENT::WEIGHT);
		//info.print_total_info();
		if (FLAGS_db) {
			dbInstance->bindInfo(info.getInfo());
		}
	}
	else {
		CHECK_FLAGS_TYPE;
		CHECK_FLAGS_CONTENT;
		auto type = info.to_type<Infos::TYPE_STAT>(FLAGS_type);
		auto content = info.to_type<Infos::TYPE_CONTENT>(FLAGS_content);
		info.compute(type, content);
		info.print_stat_info(content);
	}
}


/**********************************************************************
* COMMAND:
***********************************************************************/
void analyzer_layerinfo() {
	CHECK_FLAGS_SRC;

	if (FLAGS_db) {
		Infos info(FLAGS_src);
		dbInstance->bindInfo(info.getInfo());
		dbInstance->importLayerAttrs();
	}
}

static inline void analyzer_batch_db(std::vector<Infos> &batch_infos) {
	int batch_size = batch_infos.size();
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
}

static inline void analyzer_batch_distance(std::vector<Infos> &batch_infos) {
	auto last_batch = batch_infos.size()-1;
	for (int idx = 0; idx < last_batch; idx++) {
		//auto content = batch_infos[idx].to_type<Infos::TYPE_CONTENT>(FLAGS_content);
		__FUNC_TIME_CALL(batch_infos[idx].compute_all(Infos::TYPE_CONTENT::GRAD, batch_infos[last_batch]), "Process file with distance " + batch_infos[idx].get().filename());
		__FUNC_TIME_CALL(batch_infos[idx].compute_all(Infos::TYPE_CONTENT::WEIGHT, batch_infos[last_batch]), "Process file with distance " + batch_infos[idx].get().filename());
	}
}

static inline void analyzer_batch(std::vector<Infos> &batch_infos) {
	int batch_size = batch_infos.size();
	for (int idx = 0; idx < batch_size; idx++) {
		auto &info = batch_infos[idx];
		__FUNC_TIME_CALL(info.compute_all(Infos::TYPE_CONTENT::GRAD), "Process file with grad " + info.get().filename());
		
		if (idx == batch_size - 1){
			__FUNC_TIME_CALL(info.compute_all(Infos::TYPE_CONTENT::WEIGHT), "Process file with weight " + info.get().filename());
			// copy weight statistic to all file?
			// compute all distance
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

int main(int argc, char *argv[]) {

	gflags::ParseCommandLineFlags(&argc, &argv, true);

	dbInstance = new db::DB(FLAGS_dbname);

	if (FLAGS_action == "stat") {
		analyzer_stat();
	}
	else if (FLAGS_action == "batch") {
		analyzer_tools();
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