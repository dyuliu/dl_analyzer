#pragma once
#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_

#include <winsock2.h>

#include <vector>
#include <mongo/client/dbclient.h>
#include <libanalyzer/include/proto/analyzer.pb.h>
#include <libanalyzer/include/info/info.h>
#include <libanalyzer/include/recorder/recorder.h>

namespace db {

	using Info = analyzer::Info;
	using Recorder = analyzer::Recorder;
	using Images = analyzer::Images;

	class DB {

	// public API
	public:

		/**
		Construction Function
		@param serverAddress e.g., "192.168.1.10", 
		the default value is "localhost".
		*/
		DB(std::string database = "cnnvis", std::string dbName = "DeepLearning", std::string serverAddress = "localhost:27017");

		/**
		Deconstruction Function 
		*/
		~DB();

		/**
		Bind data to private members
		*/
		void bindInfo(Info *d);

		void bindImgInfo(Images *d);

		void bindRecorder(Recorder *d);

		/**
		Import a selected stat into DB
		*/
		void importStat(analyzer::Infos::TYPE_STAT statName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName = "");

		/**
		Import all stat information to DB
		*/
		void importAllStats();

		/**
		Import a selected seq into DB
		*/
		void importSeq(analyzer::Infos::TYPE_SEQ seqName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName = "");

		/**
		Import all seq information to DB
		*/
		void importAllSeqs();

		/**
		Import a selected distance info into DB
		*/
		void importDist(analyzer::Infos::TYPE_DISTANCE distName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName = "");

		/**
		Import all distance information to DB
		*/
		void importAllDists();

		/**
		Import all kernel ralted stats
		*/
		void importStatKernel(analyzer::Infos::TYPE_STAT_KERNEL statName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName = "");

		void importAllStatsKernel();

		/**
		Import layer attrs
		*/
		void importLayerAttrs(std::string colName = "LayerInfo");

		/*8
		Import img info including class_name & data (file) name
		*/
		void importImgInfo(std::string colName = "TrainImgInfo");

		void importTestImgInfo(int batchsize = 1000, std::string colName = "TestImgInfo");

		/**
		Import all stats and layer attrs
		*/
		void importAll();

		/**
		Import fine-grained gradient data, optional
		*/
		void importGradient(std::string colName = "GradRaw");

		/**
		Import fine-grained weight data, optional
		*/
		void importWeight(std::string colName = "WeightRaw");

		/**
		Import raw gradient and weight data
		*/
		void importRaw();

		/**
		Import recorder info, e.g., error rate.
		*/
		void importRecorderInfo();

		/**
		Import cluster info
		*/
		void importClusterInfo(analyzer::Infos::TYPE_CLUSTER clusterName, analyzer::Infos::TYPE_CONTENT contentName, unsigned int maxlayer = 2, std::string colName = "ClusterInfo");

		/**
		Create Indexes for dist, stat, statseq, and recorder collections
		*/
		void createIndexes();
		
		void deleteDB();

	// private data
	private:
		mongo::DBClientConnection connection;
		std::string dbName;
		std::string database;
		Info *iData;
		Images *imgData;
		Recorder *rData;
	};

}
