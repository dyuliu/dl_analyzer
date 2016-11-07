#include "contenttype_map.h"
#include "statname_map.h"
#include "statseqname_map.h"
#include "distname_map.h"
#include "recordername_map.h"
#include "cluster_map.h"
#include "entry.h"

using mongo::BSONArrayBuilder;
using mongo::BSONArray;
using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;
using mongo::BulkOperationBuilder;
using mongo::fromjson;

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
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
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
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
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
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
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

	void DB::importStatSeq(TYPE_SEQ statSeqName, TYPE_CONTENT contentName, std::string colName) {
		MAP_TYPE_STATSEQ::iterator iterStatSeq;
		iterStatSeq = mapTypeStatSeq.find(statSeqName);
		if (iterStatSeq == mapTypeStatSeq.end()) {
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
			colName = iterContent->second + iterStatSeq->second;
		}

		//std::cout << "Importing data to \""<< colName << "\"." << std::endl;
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
		Info *data = this->iData;
		BSONObjBuilder bObj;
		//BSONArrayBuilder floatArrValue, floatArrLayerId;

		bObj.append("iter", data->iteration())
			.append("wid", data->worker_id());

		BSONObjBuilder valueObj;
		for (int i = 0; i < data->layers_size(); i++) {

			if (data->layers(i).type() == "batch_norm") continue;
			BSONArrayBuilder floatArrValue;
			for (auto v : data->layers(i).seq(STATSEQ_INDEX(statSeqName, contentName)).data()) {
				floatArrValue.append(v);
			}
			valueObj.append(std::to_string(i), floatArrValue.arr());
		}
		bObj.append("value", valueObj.obj());
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
	}

	void DB::importAllStatSeqs() {
		//std::cout << "Importing all stats" << std::endl;

		for (auto it = mapTypeStatSeq.begin(); it != mapTypeStatSeq.end(); ++it) {
			this->importStatSeq(it->first, TYPE_CONTENT::GRAD);
		}

		Info *data = this->iData;
		if (data->worker_id() == 0) {
			for (auto it = mapTypeStatSeq.begin(); it != mapTypeStatSeq.end(); ++it) {
				this->importStatSeq(it->first, TYPE_CONTENT::WEIGHT);
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
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
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
		this->importAllStatSeqs();
		this->importAllDists();
	}

	void DB::importLayerAttrs(std::string colName) {
		std::cout << "Importing layer attrs to collection \"" << colName << "\"." << std::endl;
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
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
		this->connection.createIndex(col, fromjson("{lid:1}"));
		this->connection.createIndex(col, fromjson("{type:1}"));
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
			col = "cnnvis-cifar." + this->dbName + "_" + iterRecord->second;

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

	void DB::importClusterInfo(TYPE_CLUSTER clusterName, TYPE_CONTENT contentName, std::string colName) {

		MAP_TYPE_CLUSTER::iterator iterCluster;
		iterCluster = mapTypeCluster.find(clusterName);
		if (iterCluster == mapTypeCluster.end()) {
			std::cout << "Wrong TYPE_Cluster" << std::endl;
			return;
		}
		MAP_TYPE_CONTENT::iterator iterContent;
		iterContent = mapTypeContent.find(contentName);
		if (iterContent == mapTypeContent.end()) {
			std::cout << "Wrong TYPE_CONTENT" << std::endl;
			return;
		}
		if (colName == "") {
			colName = iterContent->second + iterCluster->second;
		}
		//std::cout << "Importing data to \"" << colName << "\"." << std::endl;
		std::string col = "cnnvis-cifar." + this->dbName + "_" + colName;
		Info *data = this->iData;
		
		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm") continue;

			auto ptr = data->layers(i).cluster(CLUSTER_INDEX(TYPE_CLUSTER::KMEANS, TYPE_CONTENT::WEIGHT));

			BSONObjBuilder centersObj;
			for (int ic = 0; ic < ptr.centre_size(); ic++) {
				BSONObjBuilder centerObj;
				centerObj.append("group_id", ptr.centre(ic).group_id());
				centerObj.append("averageDistance", ptr.centre(ic).value());
				BSONArrayBuilder floatArrValue;
				for (auto v : ptr.centre(ic).data()) {
					floatArrValue.append(v);
				}
				centerObj.append("coord", floatArrValue.arr());
				centersObj.append(std::to_string(ptr.centre(ic).index()), centerObj.obj());
			}
			BSONObj centers = centersObj.obj();

			BSONObjBuilder *pointsObjs = new BSONObjBuilder[data->layers(i).channels()];
			for (int ip = 0; ip < ptr.points_size(); ip++) {
				BSONObjBuilder pointObj;
				pointObj.append("group_id", ptr.points(ip).group_id());
				pointObj.append("distance", ptr.points(ip).value());
				BSONArrayBuilder floatArrValue;
				for (auto v : ptr.points(ip).data()) {
					floatArrValue.append(v);
				}
				pointObj.append("coord", floatArrValue.arr());
				pointsObjs[ptr.points(ip).channel_id()].append(std::to_string(ptr.points(ip).index()), pointObj.obj());
			}

			BulkOperationBuilder bulk = this->connection.initializeUnorderedBulkOp(col);
			for (int fc = 0; fc < data->layers(i).channels(); fc++) {
				BSONObjBuilder bObj;
				bObj
					.append("iter", data->iteration())
					.append("wid", data->worker_id())
					.append("lid", i)
					.append("cid", fc)
					.append("clusterNumbers", ptr.num())
					.append("centers", centers)
					.append("points", pointsObjs[fc].obj());
				bulk.insert(bObj.obj());
			}
			mongo::WriteResult rs;
			bulk.execute(0, &rs);
		}
	}

	void DB::createIndexes() {
		
		std::cout << "Creating Indexes " << this->dbName << std::endl;
		std::string col;
		for (auto it = mapTypeRecord.begin(); it != mapTypeRecord.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
		}

		for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Grad" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));

			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));
		}

		for (auto it = mapTypeStatSeq.begin(); it != mapTypeStatSeq.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Grad" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));

			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));
		}

		for (auto it = mapTypeDist.begin(); it != mapTypeDist.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Grad" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));

			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));
		}

		for (auto it = mapTypeCluster.begin(); it != mapTypeCluster.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{lid:1}"));
			this->connection.createIndex(col, fromjson("{iter:1, wid: 1, lid: 1}"));
		}
	}

	void DB::deleteDB() {
		std::cout << "Deleting database with name " << this->dbName << std::endl;
		std::string col;
		for (auto it = mapTypeRecord.begin(); it != mapTypeRecord.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Grad" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);

			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeStatSeq.begin(); it != mapTypeStatSeq.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Grad" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);

			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeDist.begin(); it != mapTypeDist.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Grad" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);

			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeCluster.begin(); it != mapTypeCluster.end(); ++it) {
			col = "cnnvis-cifar." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}
	}

}
