#pragma once
// Copyright (c) Suunto Oy 2016-2018. All rights reserved.

#include "whiteboard/integration/port.h"

/** Start a critical section.
 *
 * Depending on the underlying system, this might disable interrupts, disable
 * underlying RTOS API access (completely), support recursion or use a light
 * weight mutex.
 *
 * For best portability, assume that no Whiteboard functions may be used within
 * the critical section and keep the critical section as short as possible.
 *
 * NOTE: DO NOT CALL FROM ISR CONTEXT!
 */
WB_API void WbCriticalSectionEnter();

/** End a previously started critical secction. */
WB_API void WbCriticalSectionExit();
