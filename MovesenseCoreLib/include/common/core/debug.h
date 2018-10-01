#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "common/compiler/genDef.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Don't allow direct use of these functions

/** Writes printf-like message with line feed to debug out.
 *
 * @note Not to be called directly.
 */
void debugOut(bool appendLineFeed, const char* format, ...);

/** Writes unformatted message with line feed to debug out.
* Function takes variable number of extra uint32 parameters that are
* appended to output as 32-bit hexadecimal values.
*
* This function needs less stack than the full blown printf version.
*
* @param paramCount Number of parameters after 'message'
* @param firstParamIsString A value indicating whether the first parameter is a string
* @param message String message to output
* @note Not to be called directly.
*/
void debugOutRaw(uint8_t paramCount, bool firstParamIsString, const char* message, ...);

/** Configurable hook that allows debug logs to be passed to external debugging interfaces.
 * It is declared as 'weak' function and can be overriden, depending on use case.
 *
 * @param str Formatted debug message
 * @note No DEBUGLOGs or debugOut(...) calls should be used in this function's implementation,
 * in order to prevent infinite recursion.
 */
void WEAK debugOutHook(const char* str);

#ifdef DISABLE_DEBUGLOG
#define DEBUGLOG(...)                                                                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
    } while (0)
#define DEBUGLOGNOENDL(...)                                                                                                      \
    do                                                                                                                           \
    {                                                                                                                            \
    } while (0)
#define DEBUGLOG_SIZE()
#elif defined(__IAR_SYSTEMS_ICC__) || defined(__GNUC__)
#define DEBUGLOG(...) debugOut(true, __VA_ARGS__)
#define DEBUGLOGNOENDL(...) debugOut(false, __VA_ARGS__)
#define DEBUGLOG_SIZE() DEBUGLOG("[this]:%s: %u B", __FUNCTION__, sizeof(*this))
#elif defined(__clang__)
#define DEBUGLOG(format, ...) debugOut(true, format, ##__VA_ARGS__)
#define DEBUGLOGNOENDL(format, ...) debugOut(false, format, ##__VA_ARGS__)
#define DEBUGLOG_SIZE() DEBUGLOG("[this]:%s: %uB", __FUNCTION__, sizeof(*this))
#else
#define DEBUGLOG(...) debugOut(true, ##__VA_ARGS__)
#define DEBUGLOGNOENDL(...) debugOut(false, ##__VA_ARGS__)
#define DEBUGLOG_SIZE() DEBUGLOG("[this]:%s: %uB", __FUNCTION__, sizeof(*this))
#endif

/** Sets name of current thread to attached debugger
*/
#if (defined(_DEBUG) && defined(_MSC_VER)) || defined(__clang__)
void debugSetThreadName(const char* name);
#else
#define debugSetThreadName(name) ((void)0)
#endif

#ifdef __cplusplus
} // extern "C"
#endif
