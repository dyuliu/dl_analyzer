

#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <recorder/recorder.h>
#include <analyzer/analyzer.h>

using namespace boost::python;

// for recorders

boost::python::list get_data(analyzer::Recorders &self, std::string record_type, bool all, int iterval) {
	
	std::vector<std::tuple<int, std::string, float>> res;

	if (all) {
		res = self.get_total_info(iterval);
	}
	else {
		res = self.get_specify_type(record_type, iterval);
	}

	// res is (int, string, float)
	boost::python::list list_;
	for (auto item : res) {
		boost::python::list list_tup;
		list_tup.append(std::get<0>(item));
		list_tup.append(std::get<1>(item));
		list_tup.append(std::get<2>(item));
		list_.append(list_tup);
	}
	return list_;
}

void print_specify_type(analyzer::Recorders &self, std::string record_type, int iterval) {
	self.print_specify_type(record_type, iterval);
}

BOOST_PYTHON_MODULE(_analyzer) {

	class_<analyzer::Recorders>("Recorders")
		.def("load_from_file", &analyzer::Recorders::load_from_file)
		.def("save_to_file", &analyzer::Recorders::save_to_file)
		.def("print_total_info", &analyzer::Recorders::print_total_info)
		.def("print_specify_type", print_specify_type)
		.def("get_data", get_data)
	;

	class_<analyzer::Infos>("Infos")
	;
}