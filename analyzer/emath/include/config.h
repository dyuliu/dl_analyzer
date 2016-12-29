
#pragma once

#include <cassert>

namespace emath {

	#define DType double

	typedef struct NumScaleBin{
		int count;
		DType ls;
	}STNumScaleBin;

	#define NOT_REALIZE throw("Error: This function has not been realized")
	#define DIVIDE_ZERO throw("Error: Divide Zero")

	#define CHECK_EQ(x, y) {assert( x == y );}
	#define CHECK_NE(x, y) {assert( x != y );}

	#define CHECK_GE(x, y) {assert( x >= y );}
	#define CHECK_GT(x, y) {assert( x > y);}

}