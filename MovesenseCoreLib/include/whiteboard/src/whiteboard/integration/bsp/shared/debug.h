#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

/** Debug out function prototype. Works just like printf. */
WB_API void WbDebugOut(const char* format, ...);

/** Function that issues a breakpoint or terminates execution to assertion */
WB_API void WbAssertBreak(void);

/** Whiteboard debug out message hook prototype */
extern "C"
{
    typedef void(*WbDebugOutHookFunction)(const char* message);
}

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
#define WB_DEBUGLOG(format, ...) WbDebugOut(format, ##__VA_ARGS__)
#else
#define WB_DEBUGLOG(format, ...) ((void)0)
#endif

// Assert implementation. Do not use this directly.
#define _WB_ASSERT_IMPL(cond)                                                 \
    if (!(cond))                                                              \
    {                                                                         \
        WbDebugOut("%s(%i): ASSERT FAILED! [%s]", __FILE__, __LINE__, #cond); \
        WbAssertBreak();                                                      \
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

#define WB_VERIFY(cond) ((cond) ? true : (WbDebugOut("%s(%i): VERIFY FAILED! [%s]", __FILE__, __LINE__, #cond), false))
