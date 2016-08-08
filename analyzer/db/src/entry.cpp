#include "entry.h"

using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;

namespace db {
	
	
	
	DB::DB(std::string serverAddress) {
		mongo::client::initialize();
		try {
			connection.connect(serverAddress);
			std::cout << "mongoDB connect successfully" << std::endl;

		}
		catch (const mongo::DBException &e) {
			std::cout << "caught " << e.what() << std::endl;
		}
	}
	
	DB::~DB() {
		mongo::client::shutdown();
	}

	void DB::bindData(Info *d) {
		this->data = d;
	}

	void DB::importAllStats() {
		std::cout << "I am importing all stats" << std::endl;
		Info *data = this->data;

		size_t weight_size = 0, grad_size = 0;
		for (int i = 0; i < data->layers_size(); i++) {

			weight_size += data->layers(i).weight_size();
			grad_size += data->layers(i).grad_size();

			if (data->layers(i).type() == "batch_norm") continue;

			COUT_CHEK << std::setw(3) << i << ", " << std::setw(30) << data->layers(i).name()
				<< std::setw(10) << "min: " << std::setw(10) << data->layers(i).stat(STAT_TYPE::LAYER_STAT_MIN_C)
				<< std::setw(10) << "max: " << std::setw(10) << data->layers(i).stat(STAT_TYPE::LAYER_STAT_MAX_C)
				<< std::setw(10) << "sum: " << std::setw(10) << data->layers(i).stat(STAT_TYPE::LAYER_STAT_SUM_C)
				<< std::setw(10) << "mean: " << std::setw(10) << data->layers(i).stat(STAT_TYPE::LAYER_STAT_MEAN_C)
				<< std::setw(10) << "norm-1: " << std::setw(10) << data->layers(i).stat(STAT_TYPE::LAYER_STAT_NORM_1_C)
				<< std::setw(10) << "norm-2: " << std::setw(10) << data->layers(i).stat(STAT_TYPE::LAYER_STAT_NORM_2_C) << std::endl;
		}
	}

	void DB::importLayerNameTable() {
		std::cout << "I am importing layer name table" << std::endl;
	}

}


void iter(BSONObj o) {
	/* iterator example */
	std::cout << "\niter()\n";
	for (BSONObj::iterator i(o); i.more();) {
		std::cout << ' ' << i.next().toString() << '\n';
	}
}

void bsonOperationExample() {

	/* a simple way of making a bson */
	{
		const mongo::OID generated = mongo::OID::gen();
		std::cout << generated << std::endl;

		BSONObjBuilder b;
		b.append("_id", generated);
		b.append("when", mongo::jsTime());
		b.append("name", "joe");
		b.append("age", 32.5);
		BSONObj result = b.obj();

		std::cout << "json for object with _id: " << result << std::endl;
	}

	/* a more compact manner */
	BSONObj x = BSONObjBuilder().append("name", "jack").append("age", 38).obj();
	std::string json_str = x.toString();
	std::cout << "json for x: " << json_str << std::endl;
	std::cout << "Attr in x: " << x["name"] << ' ' << x["age"].Number() << ' ' << x.isEmpty() << std::endl;

	///* nest */
	BSONObj y = BSON("x" << "asdf" <<
		"y" << true <<
		"subobj" << BSON("z" << 3 << "q" << 4));
	std::cout << "y: " << y << std::endl;
	std::cout << "subobj.z : " << y.getFieldDotted("subobj.z").Number() << std::endl;
	std::cout << "subobj.z : " << y["subobj"]["z"].Number() << std::endl;

	std::cout << "\nstd::vector iter()\n";
	std::vector<BSONElement> t_v;
	y.elems(t_v);
	for (BSONElement &v_it : t_v) {
		std::cout << v_it.toString() << std::endl;
	}

	std::cout << "\nstd::list iter()\n";
	std::list<BSONElement> t_l;
	y.elems(t_l);
	for (BSONElement &l_it : t_l) {
		std::cout << l_it.toString() << std::endl;
	}


	BSONObj sub = y["subobj"].Obj();

	iter(y);

}