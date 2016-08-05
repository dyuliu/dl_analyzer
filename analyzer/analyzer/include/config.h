
#pragma once

#include <utils/color_print.h>
#include <utils/filesystem.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <time.h>

namespace analyzer {

// pre-compile choose
// for debug
#define __DEBUG_INFO_OUTPUT

// constant
#define MAX_PROTOFILE_SIZE 5368709120

// fast operation
#define ArrayToVector(x) (std::vector<data_type>{x.data(), x.data() + x.size()})
#define __FUNC_TIME_CALL(func, str) {auto start_t = clock(); (func); COUT_CHEK << str << ", spend time: " << clock()-start_t << std::endl; }

}