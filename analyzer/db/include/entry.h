#pragma once
#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_

#include <winsock2.h>

#include <vector>
#include <mongo/client/dbclient.h>
#include <analyzer/include/proto/analyzer.pb.h>
#include <analyzer/include/analyzer/analyzer.h>
#include <analyzer/include/recorder/recorder.h>

namespace db {

	using Info = analyzer::Info;
	using Recorder = analyzer::Recorder;

	class DB {

	// public API
	public:

		/**
		Construction Function
		@param serverAddress e.g., "192.168.1.10", 
		the default value is "localhost".
		*/
		DB(std::string dbName = "DeepLearning", std::string serverAddress = "localhost");

		/**
		Deconstruction Function 
		*/
		~DB();

		/**
		Bind info to private variable iData for further operation
		*/
		void bindInfo(Info *d);

		/**
		Bind recorder to private variable rData for further operation
		*/
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
		Import a selected distance info into DB
		*/
		void importDist(analyzer::Infos::TYPE_DISTANCE distName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName = "");

		/**
		Import all distance information to DB
		*/
		void importAllDists();

		/**
		Import layer attrs
		*/
		void importLayerAttrs(std::string colName = "LayerInfo");

		/**
		Import all stats and layer attrs
		*/
		void importAll();

		/**
		Import fine-grained gradient data, optional
		*/
		void importGradient(std::string colName = "Gradient");

		/**
		Import fine-grained weight data, optional
		*/
		void importWeight(std::string colName = "Weight");

		/**
		Import recorder info, e.g., error rate.
		*/
		void importRecorderInfo();
		
	// private data
	private:
		mongo::DBClientConnection connection;
		std::string dbName;
		Info *iData;
		Recorder *rData;
	};

}
