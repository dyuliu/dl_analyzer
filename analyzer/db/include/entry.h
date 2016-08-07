#pragma once
#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>

#include <vector>
#include <mongo/client/dbclient.h>
#include <analyzer/include/proto/analyzer.pb.h>
#include <analyzer/include/analyzer/analyzer.h>

namespace db {

	using STAT_TYPE = analyzer::Infos::STAT_TYPE;
	using Info = analyzer::Info;

	class DB {

	// public API
	public:

		/**
		Construction Function
		@param serverAddress e.g., "192.168.1.10", 
		the default value is "localhost".
		*/
		DB(std::string serverAddress);

		/**
		Deconstruction Function 
		*/
		~DB();

		/**
		Import a selected stat into DB
		@param statName, the name of the stattistic,
		type: STAT_TYPE.
		*/
		void importStat(analyzer::Infos::STAT_TYPE statName) {}

		/**
		Import all stat information to DB
		*/
		void importAllStat() {}

	// private data
	private:
		mongo::DBClientConnection connection;

	};

}
