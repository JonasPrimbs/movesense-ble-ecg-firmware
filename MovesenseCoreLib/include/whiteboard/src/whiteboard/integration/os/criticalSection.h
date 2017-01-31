#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

/** Acquires critical secction. On embedded systems this might be implemented by disabling interrupts
 * and therefore also disabling pre-emptive scheduling. On other systems this might be implemented
 * as a light weight mutex. */
WB_API void WbCriticalSectionEnter();

/** Releases critical secction. */
WB_API void WbCriticalSectionExit();
