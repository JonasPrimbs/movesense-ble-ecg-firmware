#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/os/debugLogging.h"
#include "whiteboard/integration/os/eventGroup.h"
#include "whiteboard/integration/os/interrupt.h"
#include "whiteboard/integration/os/java.h"
#include "whiteboard/integration/os/mutex.h"
#include "whiteboard/integration/os/recursiveMutex.h"
#include "whiteboard/integration/os/semaphore.h"
#include "whiteboard/integration/os/thread.h"
#include "whiteboard/integration/os/timer.h"
#include "whiteboard/integration/os/criticalSection.h"
#include "whiteboard/integration/os/threadNotification.h"
#include "whiteboard/integration/os/threadStatistics.h"
#include "whiteboard/integration/os/dynamicLinkLibrary.h"

/** Initialize port */
WB_API void WbPortInit();

/** Shutdown port */
WB_API void WbPortShutdown();