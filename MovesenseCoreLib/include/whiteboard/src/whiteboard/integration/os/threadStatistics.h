#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

// Thread info structure
namespace WB_RES
{
    struct ThreadInfo;
};

/** Gets thread runtime statistics
*
* @param pThreadStatistics Array of thread statistics structures to fill
* @param maxCount Maximum count of structures to fill
* @return Number of thread statistics structures filled
*/
WB_API size_t WbThreadGetRuntimeStatistics(WB_RES::ThreadInfo* pThreadStatistics, size_t maxCount);
