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

namespace db {

	using Info = analyzer::Info;

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
		Bind data to private variable data for further operation
		*/
		void bindData(Info *d);

		/**
		Import a selected stat into DB
		*/
		void importStat(analyzer::Infos::TYPE_STAT statName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName = "");

		/**
		Import all stat information to DB
		*/
		void importAllStats();

		/**
		Import layer attrs
		*/
		void importLayerAttrs(std::string colName = "LayerTable");

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
		
	// private data
	private:
		mongo::DBClientConnection connection;
		std::string dbName;
		Info *data;
	};

}
