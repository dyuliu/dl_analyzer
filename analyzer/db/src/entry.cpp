#include "statname_map.h"
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

	void DB::bindData(Info *data) {
		this->iData = data;
	}

	void DB::importGradient(std::string colName) {
		std::cout << "Importing gradient data to collection \"" << colName << "\"." << std::endl;
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
		std::cout << "Importing weight data to collection \"" << colName << "\"." << std::endl;
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
			std::cout << "Wrong TYPE_STAT" << std::endl;
			return;
		}
		if (colName == "") {
			colName = iterContent->second + iterStat->second;
		}
		std::cout << "Importing data to \""<< colName << "\"." << std::endl;
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
			valueObj.append(std::to_string(i), data->layers(i).stat(INDEX(statName, contentName)).value());
		}
		//bObj.append("value", floatArrValue.arr());
		//bObj.append("l_ids", floatArrLayerId.arr());
		bObj.append("o_value", valueObj.obj());
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
	}

	void DB::importAllStats() {
		std::cout << "I am importing all stats" << std::endl;

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

	void DB::importAll() {
		std::cout << "Begin to import all data into " << this->dbName << std::endl;
		this->importLayerAttrs();
		this->importAllStats();
	}

	void DB::importRecorderInfo(std::string colName) {
		std::cout << "Importing recorder info to collection \"" << colName << "\"." << std::endl;

	}

}
