
#pragma once

#include <utils/color_print.h>
#include <utils/filesystem.h>
#include <time.h>

namespace analyzer {

#define ArrayToVector(x) (std::vector<data_type>{x.data(), x.data() + x.size()})
#define __DEBUG_INFO_OUTPUT


#define __FUNC_TIME_CALL(func, str) {auto start_t = clock(); (func); COUT_CHEK << str << ", spend time: " << clock()-start_t << std::endl; }

}