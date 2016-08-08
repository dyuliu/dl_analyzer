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

	using STAT_TYPE = analyzer::Infos::STAT_TYPE_CLASS;
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
		Import a selected stat into DB
		*/
		void importStat(unsigned int statName, std::string colName = "");

		/**
		Import all stat information to DB
		*/
		void importAllStats();

		void importAll();

		void importLayerAttrs(std::string colName = "LayerTable");
		
		void bindData(Info *d);
	// private data
	private:
		mongo::DBClientConnection connection;
		std::string dbName;
		Info *data;
	};

}
