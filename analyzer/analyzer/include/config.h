
#pragma once

#include <utils/color_print.h>
#include <utils/filesystem.h>

namespace analyzer {

#define ArrayToVector(x) (std::vector<data_type>{x.data(), x.data() + x.size()})

}