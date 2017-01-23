#include "type_map.h"
#include "entry.h"

#define CHECK_HAVE_WEIGHT {if (this->iData->worker_id() != 0) continue;}

using mongo::BSONArrayBuilder;
using mongo::BSONArray;
using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;
using mongo::BulkOperationBuilder;
using mongo::fromjson;

namespace db {

	DB::DB(std::string database, std::string dbName, std::string serverAddress) {
		mongo::client::initialize();
		try {
			connection.connect(serverAddress);
			std::cout << "mongoDB connect successfully" << std::endl;
			this->database = database;
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

	void DB::bindImgInfo(Images *data) {
		this->imgData = data;
	};

	void DB::bindRecorder(Recorder *data) {
		this->rData = data;
	}

	void DB::importGradient(std::string colName) {
		// std::cout << "Importing gradient data to collection \"" << colName << "\"." << std::endl;
		std::string col = this->database + "." + this->dbName + "_" + colName;
		Info *data = this->iData;
		const float *dt = NULL;
		int kernelSize = 0, kernelNum = 0, layerLength = 0, channelNum = 0, offset = 0;


		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm") continue;
			dt = data->layers(i).grad().data();
			kernelSize = data->layers(i).width() * data->layers(i).height();
			layerLength = data->layers(i).grad().size();
			channelNum = data->layers(i).channels();
			kernelNum = data->layers(i).num();
			offset = channelNum * kernelSize;

			BulkOperationBuilder bulk = this->connection.initializeUnorderedBulkOp(col);

			// save mongo document by cid
			for (int cid = 0; cid < channelNum; cid++) {
				// fetch kernels with cid j
				BSONObjBuilder bObj;
				bObj
					.append("iter", data->iteration())
					.append("wid", data->worker_id())
					.append("lid", i)
					.append("cid", cid);
				BSONObjBuilder valueObj;
				int kcount = 0;
				for (int k = cid * kernelSize; k < layerLength; k += offset) {
					std::vector<DType> kn(dt + k, dt + k + kernelSize);
					BSONArrayBuilder floatArrValue;
					for (auto &d : kn) { floatArrValue.append(d); }
					valueObj.append(std::to_string(kcount * channelNum + cid), floatArrValue.arr());
					kcount++;
				}
				bObj.append("value", valueObj.obj());
				bulk.insert(bObj.obj());
			}

			mongo::WriteResult rs;
			bulk.execute(0, &rs);
		}
	}

	void DB::importWeight(std::string colName) {
		// std::cout << "Importing weight data to collection \"" << colName << "\"." << std::endl;
		std::string col = this->database + "." + this->dbName + "_" + colName;
		Info *data = this->iData;
		const float *dt = NULL;
		int kernelSize = 0, kernelNum = 0, layerLength = 0, channelNum = 0, offset = 0;


		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm") continue;
			dt = data->layers(i).weight().data();
			kernelSize = data->layers(i).width() * data->layers(i).height();
			layerLength = data->layers(i).weight().size();
			channelNum = data->layers(i).channels();
			kernelNum = data->layers(i).num();
			offset = channelNum * kernelSize;

			BulkOperationBuilder bulk = this->connection.initializeUnorderedBulkOp(col);

			// save mongo document by cid
			for (int cid = 0; cid < channelNum; cid++) {
				// fetch kernels with cid j
				BSONObjBuilder bObj;
				bObj
					.append("iter", data->iteration())
					.append("wid", data->worker_id())
					.append("lid", i)
					.append("cid", cid);
				BSONObjBuilder valueObj;
				int kcount = 0;
				for (int k = cid * kernelSize; k < layerLength; k += offset) {
					std::vector<DType> kn(dt + k, dt + k + kernelSize);
					BSONArrayBuilder floatArrValue;
					for (auto &d : kn) { floatArrValue.append(d); }
					valueObj.append(std::to_string(kcount * channelNum  + cid), floatArrValue.arr());
					kcount++;
				}
				bObj.append("value", valueObj.obj());
				bulk.insert(bObj.obj());
			}

			mongo::WriteResult rs;
			bulk.execute(0, &rs);
		}
	}

	void DB::importRaw() {
		this->importGradient();
		if (this->iData->worker_id() == 0)
			this->importWeight();
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
		std::string col = this->database + "." + this->dbName + "_" + colName;
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

	void DB::importSeq(TYPE_SEQ seqName, TYPE_CONTENT contentName, std::string colName) {
		MAP_TYPE_STATSEQ::iterator iterStatSeq;
		iterStatSeq = mapTypeSeq.find(seqName);
		if (iterStatSeq == mapTypeSeq.end()) {
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
		std::string col = this->database + "." + this->dbName + "_" + colName;
		Info *data = this->iData;
		BSONObjBuilder bObj;
		//BSONArrayBuilder floatArrValue, floatArrLayerId;

		bObj.append("iter", data->iteration())
			.append("wid", data->worker_id());

		BSONObjBuilder valueObj;
		for (int i = 0; i < data->layers_size(); i++) {

			if (data->layers(i).type() == "batch_norm") continue;
			BSONArrayBuilder floatArrValue;
			for (auto v : data->layers(i).seq(STATSEQ_INDEX(seqName, contentName)).data()) {
				floatArrValue.append(v);
			}
			valueObj.append(std::to_string(i), floatArrValue.arr());
		}
		bObj.append("value", valueObj.obj());
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
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
		std::string col = this->database + "." + this->dbName + "_" + colName;
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

	void DB::importAllStats() {
		for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
			this->importStat(it->first, TYPE_CONTENT::GRAD);
			CHECK_HAVE_WEIGHT
			this->importStat(it->first, TYPE_CONTENT::WEIGHT);
		}
	}

	void DB::importAllDists() {
		for (auto it = mapTypeDist.begin(); it != mapTypeDist.end(); ++it) {
			if (this->iData->worker_id() != 0) 
				this->importDist(it->first, TYPE_CONTENT::GRAD);
			// CHECK_HAVE_WEIGHT
			// this->importDist(it->first, TYPE_CONTENT::WEIGHT);
		}
	}

	void DB::importAllSeqs() {
		for (auto it = mapTypeSeq.begin(); it != mapTypeSeq.end(); ++it) {
			if (it->first != TYPE_SEQ::CHANGERATIO)
				this->importSeq(it->first, TYPE_CONTENT::GRAD);
			CHECK_HAVE_WEIGHT
			this->importSeq(it->first, TYPE_CONTENT::WEIGHT);
		}
	}

	void DB::importImgInfo(std::string colName) {

		std::string col = this->database + "." + this->dbName + "_" + colName;
		Info *data = this->iData;
		analyzer::Images imgs = data->images();

		BSONObjBuilder bObj;
		bObj.append("iter", data->iteration());
			//.append("wid", data->worker_id());
		
		BSONArrayBuilder cls;
		BSONArrayBuilder file;
		BSONArrayBuilder label;

		for (int i = 0; i < imgs.images_size(); i++) {
			cls.append(imgs.images(i).class_name());
			file.append(imgs.images(i).file_name());
			label.append(imgs.images(i).label_id());
		}
		bObj.append("cls", cls.arr());
		bObj.append("file", file.arr());
		bObj.append("label", label.arr());
		
		BSONObj o = bObj.obj();
		this->connection.insert(col, o);
	}

	void DB::importTestImgInfo(int batchsize, std::string colName) {
		// do something
		std::string col = this->database + "." + this->dbName + "_" + colName;
		Images *data = this->imgData;
		for (int bs = 0, i = 0; i < data->images_size(); i+=batchsize) {
			BSONObjBuilder bObj;
			bObj.append("iter", data->iteration())
				.append("batch", bs);
			BSONArrayBuilder cls;
			BSONArrayBuilder file;
			BSONArrayBuilder label;
			BSONArrayBuilder answer;
			BSONArrayBuilder prob;

			for (int j = i; j < i + batchsize; j++) {
				cls.append(data->images(j).class_name());
				label.append(data->images(j).label_id());
				file.append(data->images(j).file_name());
				answer.append(data->images(j).answer());
				BSONArrayBuilder probArr;
				for (int k = 0; k < data->images(j).prob_size(); k++) { probArr.append(data->images(j).prob(k)); }
				prob.append(probArr.arr());
			}
			bObj.append("cls", cls.arr());
			bObj.append("file", file.arr());
			bObj.append("label", label.arr());
			bObj.append("answer", answer.arr());
			bObj.append("prob", prob.arr());
			bs++;

			BSONObj o = bObj.obj();
			this->connection.insert(col, o);
		}
	}

	void DB::importStatKernel(analyzer::Infos::TYPE_STAT_KERNEL statName, analyzer::Infos::TYPE_CONTENT contentName, std::string colName) {
		MAP_TYPE_STAT_KERNEL::iterator iterStat;
		iterStat = mapTypeStatKernel.find(statName);
		if (iterStat == mapTypeStatKernel.end()) {
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
		std::string col = this->database + "." + this->dbName + "_" + colName;
		std::cout << "Importing data to \""<< col << "\"." << std::endl;

		Info *data = this->iData;
		BulkOperationBuilder bulk = this->connection.initializeUnorderedBulkOp(col);

		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm") continue;
			BSONObjBuilder bObj;
			bObj
				.append("iter", data->iteration())
				.append("lid", i)
				.append("name", data->layers(i).name());
			BSONArrayBuilder floatArrValue;
			auto st = data->layers(i).stat_kernel(STAT_KERNEL_INDEX(statName, contentName));
			for (int j = 0; j < st.data_size(); j++) {
				floatArrValue.append(st.data(j));
			}
			bObj.append("value", floatArrValue.arr());
			bulk.insert(bObj.obj());
		}
		mongo::WriteResult rs;
		bulk.execute(0, &rs);

	}

	void DB::importAllStatsKernel() {
		for (auto it = mapTypeStatKernel.begin(); it != mapTypeStatKernel.end(); ++it) {
			this->importStatKernel(it->first, TYPE_CONTENT::WEIGHT);
		}
	}

	void DB::importAll() {
		this->importAllStats();
		this->importAllSeqs();
		// this->importAllDists();
		this->importImgInfo();
		// this->importAllStatsKernel();
	}

	void DB::importLayerAttrs(std::string colName) {
		std::cout << this->database << std::endl;
		std::string col = this->database + "." + this->dbName + "_" + colName;
		std::cout << "Importing layer attrs to collection \"" << col << "\"." << std::endl;
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
				std::cout << data->tuple(i).type() << " Wrong TYPE_RECORD_MAP" << std::endl;
				// return;
			}
			else {
				col = this->database + "." + this->dbName + "_" + iterRecord->second;

				BSONObjBuilder bObj;
				bObj.append("iter", data->tuple(i).iteration())
					.append("value", data->tuple(i).value());
				BSONObj o = bObj.obj();
				if ((i + 1) % 10000 == 0) {
					std::cout << i + 1 << "tuples have been inserted successfully" << std::endl;
				}
				this->connection.insert(col, o);
			}
			
		}
		std::cout << data->tuple_size() << "tuples have been inserted successfully" << std::endl;
	}

	void DB::importClusterInfo(TYPE_CLUSTER clusterName, TYPE_CONTENT contentName, unsigned int maxlayer, std::string colName) {

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
		std::string col = this->database + "." + this->dbName + "_" + colName;
		Info *data = this->iData;
		
		int count = 0;
		for (int i = 0; i < data->layers_size(); i++) {
			if (data->layers(i).type() == "batch_norm")
			{
				continue;
			}
			else
			{
				count++;
			}

			if (count > maxlayer) return;

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
			col = this->database + "." + this->dbName + "_" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
		}
		
		for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
			col = this->database + "." + this->dbName + "_Grad" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));

			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));
		}

		for (auto it = mapTypeSeq.begin(); it != mapTypeSeq.end(); ++it) {
			col = this->database + "." + this->dbName + "_Grad" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));

			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));
		}

		for (auto it = mapTypeStatKernel.begin(); it != mapTypeStatKernel.end(); ++it) {
			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{lid:1}"));
			this->connection.createIndex(col, fromjson("{iter: 1, lid:1}"));
		}


		std::vector<std::string> names = { "WeightRaw", "GradRaw" };
		for (auto &it : names) {
			col = this->database + "." + this->dbName + "_" + it;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{lid:1}"));
			this->connection.createIndex(col, fromjson("{wid: 1, lid: 1, iter:1}"));
		}


		col = this->database + "." + this->dbName + "_TrainImgInfo";
		std::cout << "Creating Index on " << col << std::endl;
		this->connection.createIndex(col, fromjson("{iter:1}"));

		col = this->database + "." + this->dbName + "_TestImgInfo";
		std::cout << "Creating Index on " << col << std::endl;
		this->connection.createIndex(col, fromjson("{iter:1}"));
		this->connection.createIndex(col, fromjson("{iter:1, batch:1}"));

		/* // deprecated types
		for (auto it = mapTypeDist.begin(); it != mapTypeDist.end(); ++it) {
			col = this->database + "." + this->dbName + "_Grad" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));

			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{wid:1, iter:1}"));
		}
		
		for (auto it = mapTypeCluster.begin(); it != mapTypeCluster.end(); ++it) {
			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Creating Index on " << col << std::endl;
			this->connection.createIndex(col, fromjson("{iter:1}"));
			this->connection.createIndex(col, fromjson("{wid:1}"));
			this->connection.createIndex(col, fromjson("{lid:1}"));
			this->connection.createIndex(col, fromjson("{wid: 1, lid: 1, iter:1}"));
		}
		*/


	}

	void DB::deleteDB() {
		std::cout << "Deleting database with name " << this->dbName << std::endl;
		std::string col;
		for (auto it = mapTypeRecord.begin(); it != mapTypeRecord.end(); ++it) {
			col = this->database + "." + this->dbName + "_" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeStat.begin(); it != mapTypeStat.end(); ++it) {
			col = this->database + "." + this->dbName + "_Grad" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);

			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeSeq.begin(); it != mapTypeSeq.end(); ++it) {
			col = this->database + "." + this->dbName + "_Grad" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);

			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeDist.begin(); it != mapTypeDist.end(); ++it) {
			col = this->database + "." + this->dbName + "_Grad" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);

			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		for (auto it = mapTypeCluster.begin(); it != mapTypeCluster.end(); ++it) {
			col = this->database + "." + this->dbName + "_Weight" + it->second;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		std::vector<std::string> names = { "WeightRaw", "GradRaw" };
		for (auto it : names) {
			col = this->database + "." + this->dbName + "_" + it;
			std::cout << "Deleting collection " << col << std::endl;
			this->connection.dropCollection(col);
		}

		col = this->database + "." + this->dbName = "_TrainImgInfo";
		this->connection.dropCollection(col);
		col = this->database + "." + this->dbName = "_TestImgInfo";
		this->connection.dropCollection(col);
	}

}
