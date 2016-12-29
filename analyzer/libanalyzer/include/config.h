
#pragma once

// externel
#include <emath/include/config.h>

// internel
#include <utils/color_print.h>
#include <utils/filesystem.h>
#include <utils/filestream.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <time.h>

namespace analyzer {

#ifndef DType
	#define DType float
#endif

// pre-compile choose
// for debug
// #define __DEBUG_INFO_OUTPUT

// google repeated type -> vector
#define ArrayToVector(x) (std::vector<DType>{x.data(), x.data() + x.size()})

#ifdef __DEBUG_INFO_OUTPUT
	#define __FUNC_TIME_CALL(func, str) {auto start_t = clock(); (func); COUT_CHEK << str << ", spend time: " << clock()-start_t << std::endl; }
#else
	#define __FUNC_TIME_CALL(func, str) {(func);}
#endif

#define THROW(str) {std::cout << str << std::endl; throw(str);}


// constant
#define MAX_PROTOFILE_SIZE 5368709120
#define NUM_HISTOGRAM_BINS 64
#define NUM_CHANGERATIO_BINS 56
#define	NUM_CHANGERATIO_SCALES 6

}