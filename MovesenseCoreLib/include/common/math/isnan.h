#pragma once
/******************************************************************************

        Copyright (c) Suunto Oy 2016.
        All rights reserved.

******************************************************************************/
/**
@file isnan.h
*/
#include <cmath>

// GCC ARM compiler uses std::isnan() rather than plain isnan().
#ifdef __GNUC__
using std::isnan;
#endif
