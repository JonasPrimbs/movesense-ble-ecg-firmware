#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/shared/pack.h"

#include "whiteboard/integration/shared/types.h"

#include "whiteboard/integration/shared/crc32.h"
#include "whiteboard/integration/shared/headerCheck.h"
#include "whiteboard/integration/shared/interlocked.h"
#include "whiteboard/integration/shared/macros.h"
#include "whiteboard/integration/shared/safeString.h"
#include "whiteboard/integration/shared/templateMetaprogramming.h"

#include "whiteboard/integration/bsp/stackAlloc.h"
#include "whiteboard/integration/bsp/heapAlloc.h"
#include "whiteboard/integration/bsp/alignedAlloc.h"
#include "whiteboard/integration/bsp/newOperator.h"

#include "whiteboard/integration/bsp/crashDump.h"
#include "whiteboard/integration/bsp/ipstack.h"
#include "whiteboard/integration/bsp/serial.h"
#include "whiteboard/integration/bsp/time.h"
#include "whiteboard/integration/bsp/timestamp.h"

/** Initialize BSP part of Whiteboard port */
WB_API void WbBspInit();

/** Shutdown BSP part of Whiteboard port */
WB_API void WbBspShutdown();
