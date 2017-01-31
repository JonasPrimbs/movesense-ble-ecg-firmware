#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

#ifndef LOG_TAG
#define LOG_TAG "Whiteboard_engine"
#endif

#include <android/log.h>
#include <assert.h>

#if! defined(NDEBUG) || defined(WB_HAVE_DEBUGLOG_IN_RELEASE)
#define WB_DEBUGLOG(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#else
#define WB_DEBUGLOG(...) ((void)0)
#endif

// Assert implementation. Do not use this directly.
#define _WB_ASSERT_IMPL(cond)                                                                                                    \
    ((cond) ? (void)0 : __android_log_assert(0, LOG_TAG, "%s(%d): ASSERT FAILED! [%s]", __FILE__, __LINE__, #cond))

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

#define WB_VERIFY(cond)                                                                                                          \
    ((cond) ? true : (__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "%s(%i): VERIFY FAILED! [%s]", __FILE__, __LINE__, #cond), \
                      false))

/** Sets name of the current thread to debugger */
inline void WbDebugSetThreadName(const char*) {}

/** Whiteboard debug out message hook prototype */
extern "C"
{
    typedef void(*WbDebugOutHookFunction)(const char* message);
}

/** Function that can be used to redirect Whiteboard debug out messages 
 *
 * @param hookFunction Function that should handle debug messages
 */
void WbDebugOutSetHook(WbDebugOutHookFunction hookFunction);
