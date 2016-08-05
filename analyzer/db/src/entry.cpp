#undef UNICODE
#define UNICODE
#undef _WINSOCKAPI_
#define _WINSOCKAPI_
#include <windows.h>
#include <winsock2.h>

#include <cstdlib>
#include <iostream>
#include <mongo/client/dbclient.h>
#include <entry.h>

namespace db {

	int FuncPlus(int a, int b) {
		return a + b;
	};

	void connect() {
		mongo::DBClientConnection c;
		c.connect("localhost");
	};

	int run() {
		mongo::client::initialize();
		
		try {
			connect();
			std::cout << "connected ok" << std::endl;
		}
		catch (const mongo::DBException &e) {
			std::cout << "caught " << e.what() << std::endl;
		}

		mongo::client::shutdown();
		return EXIT_SUCCESS;
	}
}