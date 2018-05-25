#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

/** Type of timestamp */
typedef uint32_t WbTimestamp;

/** Gets current timestamp
*
* @note: Timestamp is not in any specific units, but should not wrap for at least a day
*
* @return Current timestamp */
WB_API WbTimestamp WbTimestampGet();

/** Increments timestamp value by given amount of milliseconds
*
* @param timestamp Base time stamp to increment.
* @param milliseconds Number of milliseconds to increment
* @return Incremented timestamp
*/
WB_API WbTimestamp WbTimestampIncrementMs(WbTimestamp timestamp, size_t milliseconds);

/** Gets difference of two timestamps in milliseconds
*
* @param start First timestamp
* @param end Second timestamp
* @return Timestamp difference (end - start) in milliseconds
*/
WB_API int WbTimestampDifferenceMs(WbTimestamp start, WbTimestamp end);
