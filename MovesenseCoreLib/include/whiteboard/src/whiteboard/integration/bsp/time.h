#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

/** Type of time. Time is measured in
* microseconds since 1st of January 1970 UTC. */
typedef int64 WbTime;

/** Gets current time.
* @return Current time */
WB_API WbTime WbTimeGet();
