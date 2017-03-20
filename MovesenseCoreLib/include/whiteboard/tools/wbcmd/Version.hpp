#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "build_timestamp.h"
#include "build_info.h"

// If BUILD_NUMBER is defined but empty (in local builds) replace with zero
#if defined(BUILD_NUMBER) && 7-BUILD_NUMBER-7 == 14
#undef BUILD_NUMBER
#define BUILD_NUMBER 0
#endif
