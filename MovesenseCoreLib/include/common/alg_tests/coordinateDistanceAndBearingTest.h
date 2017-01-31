#pragma once
#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"

const size_t INPUT_SIZE = 2403 + 2 + 6; // 1865
const size_t TEST_SIZE = INPUT_SIZE;

#ifdef MATLAB
extern int32 temp[TEST_SIZE];
#endif

extern const int32 input_lat_coordinates[INPUT_SIZE];
extern const int32 input_long_coordinates_begin[INPUT_SIZE];
extern const uint32 output_distance[INPUT_SIZE];
extern const int32 output_bearing[INPUT_SIZE];
