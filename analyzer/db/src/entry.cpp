#include "entry.h"

using mongo::BSONElement;
using mongo::BSONObj;
using mongo::BSONObjBuilder;

namespace db {

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

}