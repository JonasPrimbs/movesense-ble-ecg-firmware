#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

/** Whiteboard debug out message hook prototype */
#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#include "whiteboard/integration/shared/macros.h"
#endif

typedef void(*WbDebugOutHookFunction)(bool fatal, const char* message);

/** C function that can be used to redirect Whiteboard debug out messages
 *
 * @param hookFunction Function that should handle debug messages
 */
WB_API void WbDebugOutSetHookC(WbDebugOutHookFunction hookFunction);

#ifdef __cplusplus
}
#include "whiteboard/integration/port.h"

/** Debug out function prototype. Works just like printf. */
WB_API void WbDebugOut(bool fatal, const char* format, ...);

/** Function that issues a breakpoint or terminates execution to assertion */
WB_API void WbAssertBreak(void);

/** Function that can be used to redirect Whiteboard debug out messages 
 *
 * @param hookFunction Function that should handle debug messages
 */
WB_API void WbDebugOutSetHook(WbDebugOutHookFunction hookFunction);

/** Set name of current thread to debugger */
#if !defined(NDEBUG)
WB_API void WbDebugSetThreadName(const char* name);
#else
inline void WbDebugSetThreadName(const char*) {}
#endif

#if !defined(NDEBUG) || defined(WB_HAVE_DEBUGLOG_IN_RELEASE)
#define WB_DEBUGLOG(format, ...) WbDebugOut(false, format, ##__VA_ARGS__)
#define WB_DEBUGLOG_SIZE() WB_DEBUGLOG("[this]:%s: %zuB", __FUNCTION__, sizeof(*this))

#else
#define WB_DEBUGLOG(format, ...) ((void)0)
#define WB_DEBUGLOG_SIZE()
#endif

// Assert implementation. Do not use this directly.
#define _WB_ASSERT_IMPL(cond)                                                           \
    if (!(cond))                                                                        \
    {                                                                                   \
        WbDebugOut(true, "%s(%i): ASSERT FAILED! [%s]", WB_SOURCE_FILENAME, __LINE__, #cond); \
        WbAssertBreak();                                                                \
    }

// WB_DEBUG_ASSERTs will be removed from release builds
#if !defined(NDEBUG)
#define WB_DEBUG_ASSERT(cond) _WB_ASSERT_IMPL(cond)
#else
#define WB_DEBUG_ASSERT(cond) ((void)0) 
#endif

// WB_ASSERTs will be removed from release builds unless WB_ASSERT_IN_RELEASE flag is set
#if !defined(NDEBUG) || defined(WB_ASSERT_IN_RELEASE)
#define WB_ASSERT(cond) _WB_ASSERT_IMPL(cond)
#else
#define WB_ASSERT(cond) ((void)0) 
#endif

#define WB_VERIFY(cond) ((cond) ? true : (WbDebugOut(false, "%s(%i): VERIFY FAILED! [%s]", WB_SOURCE_FILENAME, __LINE__, #cond), false))

#endif // __cplusplus
