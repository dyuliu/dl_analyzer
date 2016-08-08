#include "statname_map.h"
#include "entry.h"

using mongo::BSONArrayBuilder;
using mongo::BSONArray;
using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;

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
		this->data = data;
	}

	void DB::importStat(unsigned int statName, std::string colName) {
		MAP_TYPE::iterator iter;
		iter = nameMap.find(statName);
		if (iter == nameMap.end()) {
			std::cout << "No such stat" << std::endl;
			return;
		}
		if (colName == "") {
			colName = iter->second;
		}
		std::cout << "I am importing " << iter->second << 
			" into the collection with name \"" << colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->data;
		BSONObjBuilder bObj;
		BSONArrayBuilder floatArrValue, floatArrLayerId;

		bObj.append("it", data->iteration())
			.append("wid", data->sim_id());

		for (int i = 0; i < data->layers_size(); i++) {

			if (data->layers(i).type() == "batch_norm") continue;

			floatArrValue.append(data->layers(i).stat(statName));
			floatArrLayerId.append(i);
		}
		bObj.append("v", floatArrValue.arr());
		bObj.append("l", floatArrLayerId.arr());
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
	}

	void DB::importAllStats() {
		std::cout << "I am importing all stats" << std::endl;

		for (auto it = nameMap.begin(); it != nameMap.end(); ++it) {
			this->importStat(it->first);
		}
	}

	void DB::importLayerAttrs(std::string colName) {
		std::cout << "I am importing layer attrs into the collection with name \"" << colName << "\"." << std::endl;
		std::string col = this->dbName + '.' + colName;
		Info *data = this->data;
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
		std::cout << "I am importing all data into db" << std::endl;
		this->importLayerAttrs();
		this->importAllStats();
	}

}
