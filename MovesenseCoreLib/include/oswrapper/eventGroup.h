#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/

#include "oswrapper/debugAndTrace.h"
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* EventGroupHandle;
typedef uint32_t EventFlagMask;

EventGroupHandle eventGroupCreate(WAITOBJECT_DEBUGNAME_PARAM_DECL_VOID);
void eventGroupDelete(EventGroupHandle group);
EventFlagMask
eventGroupWaitFlags(EventGroupHandle group, EventFlagMask flags, bool clearOnExit, bool waitForAllFlags, size_t timeoutMs);
EventFlagMask eventGroupClearFlags(EventGroupHandle eventGroup, EventFlagMask flagsToClear);
bool eventGroupClearFlagsFromISR(EventGroupHandle eventGroup, EventFlagMask flagsToClear);
EventFlagMask eventGroupSetFlags(EventGroupHandle eventGroup, EventFlagMask flagsToSet);
bool eventGroupSetFlagsFromISR(EventGroupHandle eventGroup, EventFlagMask flagsToSet);
EventFlagMask
eventGroupSync(EventGroupHandle eventGroup, EventFlagMask flagsToSet, EventFlagMask flagsToWaitFor, size_t timeoutMs);
EventFlagMask eventGroupGetFlags(EventGroupHandle eventGroup);
EventFlagMask eventGroupGetFlagsFromISR(EventGroupHandle eventGroup);

#ifdef CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES

#define eventGroupCreate() eventGroupCreate(EVENTGROUP_NAME)

#endif

#if defined(__cplusplus)
} // extern "C" {
#endif
