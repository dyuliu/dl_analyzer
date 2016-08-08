
#pragma once

// externel
#include <emath/include/config.h>

// internel
#include <utils/color_print.h>
#include <utils/filesystem.h>
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

// constant
#define MAX_PROTOFILE_SIZE 5368709120

// fast operation
#define ArrayToVector(x) (std::vector<DType>{x.data(), x.data() + x.size()})
#define __FUNC_TIME_CALL(func, str) {auto start_t = clock(); (func); COUT_CHEK << str << ", spend time: " << clock()-start_t << std::endl; }

}