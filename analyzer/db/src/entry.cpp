#include "contenttype_map.h"
#include "statname_map.h"
#include "distname_map.h"
#include "recordername_map.h"
#include "entry.h"

using mongo::BSONArrayBuilder;
using mongo::BSONArray;
using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;
using mongo::BulkOperationBuilder;

namespace db {

	DB::DB(std::string dbName, std::string serverAddress) {
		mongo::client::initialize();
		try {
			connection.connect(serverAddress);
			std::cout << "mongoDB connect successfully" << std::endl;
			this->dbName = dbName;
		}
		catch (const mongo::DBException &e) {
			std::cout << "caught " << e.what() << std::endl;
		}
	}

	DB::~DB() {
		mongo::client::shutdown();
	}

	void DB::bindInfo(Info *data) {
		this->iData = data;
	}

	void DB::bindRecorder(Recorder *data) {
		this->rData = data;
	}

	void DB::importGradient(std::string colName) {
		//std::cout << "Importing gradient data to collection \"" << colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->iData;
		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm") continue;
			BSONObjBuilder bObj;
			BSONArrayBuilder floatArrValue;
			bObj.append("iter", data->iteration())
				.append("wid", data->worker_id())
				.append("lid", i)
				.appendArray("v", BSON_ARRAY(1.0 << 2.0 << 3.0 << 4.0 << 5.0));
			BSONObj o = bObj.obj();
			this->connection.insert(col, o);
		}
	}

	void DB::importWeight(std::string colName) {
		//std::cout << "Importing weight data to collection \"" << colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->iData;
		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm") continue;
			BSONObjBuilder bObj;
			BSONArrayBuilder floatArrValue;
			bObj.append("iter", data->iteration())
				.append("wid", data->worker_id())
				.append("lid", i)
				.appendArray("v", BSON_ARRAY(1.0 << 2.0 << 3.0 << 4.0 << 5.0));
			BSONObj o = bObj.obj();
			this->connection.insert(col, o);
		}
	}

	void DB::importStat(TYPE_STAT statName, TYPE_CONTENT contentName, std::string colName) {
		MAP_TYPE_STAT::iterator iterStat;
		iterStat = mapTypeStat.find(statName);
		if (iterStat == mapTypeStat.end()) {
			std::cout << "Wrong TYPE_STAT" << std::endl;
			return;
		}
		MAP_TYPE_CONTENT::iterator iterContent;
		iterContent = mapTypeContent.find(contentName);
		if (iterContent == mapTypeContent.end()) {
			std::cout << "Wrong TYPE_CONTENT" << std::endl;
			return;
		}
		if (colName == "") {
			colName = iterContent->second + iterStat->second;
		}
		//std::cout << "Importing data to \""<< colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->iData;
		BSONObjBuilder bObj;
		//BSONArrayBuilder floatArrValue, floatArrLayerId;

		bObj.append("iter", data->iteration())
			.append("wid", data->worker_id());

		BSONObjBuilder valueObj;
		for (int i = 0; i < data->layers_size(); i++) {

			if (data->layers(i).type() == "batch_norm") continue;
			//floatArrValue.append(data->layers(i).stat(STAT_INDEX(statName, contentName)).value());
			//floatArrLayerId.append(i);
			valueObj.append(std::to_string(i), data->layers(i).stat(STAT_INDEX(statName, contentName)).value());
		}
		//bObj.append("value", floatArrValue.arr());
		//bObj.append("l_ids", floatArrLayerId.arr());
		bObj.append("value", valueObj.obj());
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
	}

	void DB::importAllStats() {
		//std::cout << "Importing all stats" << std::endl;

		for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
			this->importStat(it->first, TYPE_CONTENT::GRAD);
		}

		Info *data = this->iData;
		if (data->worker_id() == 0) {
			for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
				this->importStat(it->first, TYPE_CONTENT::WEIGHT);
			}
		}
	}

	void DB::importDist(TYPE_DISTANCE distName, TYPE_CONTENT contentName, std::string colName) {
		MAP_TYPE_DIST::iterator iterDist;
		iterDist = mapTypeDist.find(distName);
		if (iterDist == mapTypeDist.end()) {
			std::cout << "Wrong TYPE_DIST" << std::endl;
			return;
		}
		MAP_TYPE_CONTENT::iterator iterContent;
		iterContent = mapTypeContent.find(contentName);
		if (iterContent == mapTypeContent.end()) {
			std::cout << "Wrong TYPE_CONTENT" << std::endl;
			return;
		}
		if (colName == "") {
			colName = iterContent->second + iterDist->second;
		}
		//std::cout << "Importing data to \"" << colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->iData;
		BSONObjBuilder bObj;
		//BSONArrayBuilder floatArrValue, floatArrLayerId;

		bObj.append("iter", data->iteration())
			.append("wid", data->worker_id());

		BSONObjBuilder valueObj;
		for (int i = 0; i < data->layers_size(); i++) {

			if (data->layers(i).type() == "batch_norm") continue;
			//floatArrValue.append(data->layers(i).stat(INDEX(statName, contentName)).value());
			//floatArrLayerId.append(i);
			valueObj.append(std::to_string(i), data->layers(i).distance(DIST_INDEX(distName, contentName)).value());
		}
		//bObj.append("value", floatArrValue.arr());
		//bObj.append("l_ids", floatArrLayerId.arr());
		bObj.append("value", valueObj.obj());
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
	}

	void DB::importAllDists() {
		//std::cout << "Importing all Dists" << std::endl;

		for (auto it = mapTypeDist.begin(); it != mapTypeDist.end(); ++it) {
			this->importDist(it->first, TYPE_CONTENT::GRAD);
			this->importDist(it->first, TYPE_CONTENT::WEIGHT);
		}
	}

	void DB::importAll() {
		//std::cout << "Begin to import all data to " << this->dbName << std::endl;
		//this->importLayerAttrs();
		this->importAllStats();
		this->importAllDists();
	}

	void DB::importLayerAttrs(std::string colName) {
		std::cout << "Importing layer attrs to collection \"" << colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->iData;
		for (int i = 0; i < data->layers_size(); i++) {
			BSONObjBuilder bObj;
			bObj.append("lid", i)
				.append("name", data->layers(i).name())
				.append("type", data->layers(i).type())
				.append("channels", data->layers(i).channels())
				.append("kernelNum", data->layers(i).num())
				.append("kernelWidth", data->layers(i).width())
				.append("kernelHeight", data->layers(i).height());
			this->connection.insert(col, bObj.obj());
		}
	}

	void DB::importRecorderInfo() {
		std::cout << "Importing records to " << this->dbName << std::endl;
		std::string col;
		Recorder *data = this->rData;

		for (int i = 0; i < data->tuple_size(); i++) {
			MAP_TYPE_RECORD::iterator iterRecord = mapTypeRecord.find(data->tuple(i).type());
			if (iterRecord == mapTypeRecord.end()) {
				std::cout << "Wrong TYPE_RECORD_MAP" << std::endl;
				return;
			}
			col = this->dbName + '.' + iterRecord->second;

			BSONObjBuilder bObj;
			bObj.append("iter", data->tuple(i).iteration())
				.append("value", data->tuple(i).value());
			BSONObj o = bObj.obj();
			if ( (i + 1) % 1000 == 0) {
				std::cout << i + 1 << "tuples have been inserted successfully" << std::endl;
			}
			this->connection.insert(col, o);
		}

		std::cout << data->tuple_size() << "tuples have been inserted successfully" << std::endl;

	}

}
