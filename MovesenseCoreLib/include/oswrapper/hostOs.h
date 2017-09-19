#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#if defined(ARCH_SDL) && (defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__))
#include "common/compiler/genType.h"

#ifdef __cplusplus
extern "C" {
#endif

// OsWrapper funktions re-declared here to avoid all possible dependencies
// in code that includes this file.
void threadEnterCritical(void);
void threadExitCritical(void);

#ifdef __cplusplus
}
#endif

// These macros are needed to prevent pre-emptive simulation OS
// to switch thread during host OS API calls. Otherwise deadlock can
// occur in following scenario:
// 1) Low prortiy thread executes a host OS call, that takes a lock on host OS API level (eg. writes to debug log)
// 2) FreeRTOS executes tick interrupt and switches running a thread with higher priority
// 3) Higher priority thread executes a host OS call that tries to take the same host OS lock
// 4) Locking doesn't succeed and higher priority thread starts a wait
// 5) FreeRTOS continues execution of high priority thread, because it doesn't know that the thread has blocked
// 6) In every context switch the higher priority thread is chosen over the lower priority thread and deadlock is reality
#define HOST_OS_API_ENTER() threadEnterCritical()
#define HOST_OS_API_EXIT() threadExitCritical()

#ifdef __cplusplus

/** Helper for wrapping Host OS API calls */
class HostOsApiScope
{
public:
    HostOsApiScope() { HOST_OS_API_ENTER(); }
    ~HostOsApiScope() { HOST_OS_API_EXIT(); }

private:
    HostOsApiScope(const HostOsApiScope&);
    HostOsApiScope& operator=(const HostOsApiScope&);
};

#define _HOST_OS_API_TOKENPASTE(x, y) _HOST_OS_API_TOKENPASTE2(x, y)
#define _HOST_OS_API_TOKENPASTE2(x, y) x##y

#define HOST_OS_API_CALL(call) (HostOsApiScope(), call)
#define HOST_OS_API_SCOPE() HostOsApiScope _HOST_OS_API_TOKENPASTE(hostOsScope, __LINE__)

#endif

#else

// When there is no host OS, we don't need any extra locking
#define HOST_OS_API_ENTER() (void)0
#define HOST_OS_API_EXIT() (void)0
#define HOST_OS_API_CALL(call) (call)
#define HOST_OS_API_SCOPE() (void)0

#endif

#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__)

#ifdef __cplusplus
extern "C" {
#endif

// Check whether the program is running in CI environment
bool isRunningInContinuousIntegration();

#ifdef __cplusplus
}
#endif

#endif

#if defined(ARCH_SDL) && defined(_MSC_VER) && defined(_DEBUG)

// At least in versions 10.0.10150.0 and 10.0.10240.0 of Unified C
// runtime (ucrt) sprintf & co implementations have dynamic buffer for
// formatting when the string length is more than 1024 bytes. If string is
// shorter than that static buffer is used. This would be fine, but on exit
// sprintf calls _free_dbg calls with null pointer. _free_dbg takes OS level
// lock without first checking if the pointer is null. Taking OS level lock
// without correct protection may deadlock FreeRTOS. Calling _free_dbg
// with null pointer is almost completely useless, because it is only
// making heap sanity checks if that is enabled.
//
// Because we have also 3rd party code that may be using sprintf, best
// option would be to patch the CRT library. But because patching existing
// runtime library on Windows / Visual C++ is quite impossible, here is
// macro workaround for code that we compile our selves.
//
// For patching I have tried following:
//   - Use mingw objcopy -> Does not understand MSVC libraries
//   - Search & replace symbols in the library -> Libary was corrupted
//     (there's checksum or something like that)
//   - Use module definition file to create a new import library, from
//     static crt lib -> Some functions like memset that are replaced
//     with compiler intrinsics cannot be exported anymore
//
// One working solution is to reimplement _free_dbg and its dependencies
// and then link that object file directly to application and use
// statically linked runtime lib. This causes nasty multiple definitions
// for symbol _free_dbg warning, but would be safe to ignore. I decided
// against this because of the warning message and because implementation
// would most certainly break when ever another CRT version is used. We
// could get rid of the warning by removing whole debug_heap.cpp, which
// isn't very feasible since it is 64kB code file.
//
// Another solution that I haven't tried would be to compile the ucrt
// library from sources. All sources of ucrt can be found from
// C:\Program Files (x86)\Windows Kits\10 and VCruntime code from
// C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\crt\src
//
// Third solution would be to use third party C library or at least
// sprintf implementation. C libary would be better, because existing
// code should not need modification (except includes & linking options).

// Undefine existing sprintf macro definitions. MSVC_SDL.cmake contains
// definitions that could be used to detect code that is not using
// properly workaround code of this file.
#undef vsprintf
#undef vsprintf_s
#undef vsnprintf
#undef vsnprintf_s
#undef sprintf
#undef sprintf_s
#undef snprintf
#undef snprintf_s

#include <stdio.h>
#include <stdarg.h>

#ifdef __cplusplus

inline int hostOsApiWrapped_vsprintf(char* const buffer, char const* const format, va_list argList)
{
    int result;
    HOST_OS_API_ENTER();
#pragma warning(push)
#pragma warning(disable : 4996)
    result = vsprintf(buffer, format, argList);
#pragma warning(pop)
    HOST_OS_API_EXIT();
    return result;
}

inline int hostOsApiWrapped_vsprintf_s(char* const buffer, size_t const bufferCount, char const* const format, va_list argList)
{
    int result;
    HOST_OS_API_ENTER();
    result = vsprintf_s(buffer, bufferCount, format, argList);
    HOST_OS_API_EXIT();
    return result;
}

inline int hostOsApiWrapped_vsnprintf(char* const buffer, size_t const bufferCount, char const* const format, va_list argList)
{
    int result;
    HOST_OS_API_ENTER();
    result = vsnprintf(buffer, bufferCount, format, argList);
    HOST_OS_API_EXIT();
    return result;
}

inline int hostOsApiWrapped_vsnprintf_s(
    char* const buffer, size_t const bufferCount, size_t const maxCount, char const* const format, va_list argList)
{
    int result;
    HOST_OS_API_ENTER();
    result = vsnprintf_s(buffer, bufferCount, maxCount, format, argList);
    HOST_OS_API_EXIT();
    return result;
}

inline int hostOsApiWrapped_sprintf(char* const buffer, char const* const format, ...)
{
    int result;
    HOST_OS_API_ENTER();
    va_list argList;
    va_start(argList, format);
#pragma warning(push)
#pragma warning(disable : 4996)
    result = vsprintf(buffer, format, argList);
#pragma warning(pop)
    va_end(argList);
    HOST_OS_API_EXIT();
    return result;
}

inline int hostOsApiWrapped_sprintf_s(char* const buffer, size_t const bufferCount, char const* const format, ...)
{
    int result;
    HOST_OS_API_ENTER();
    va_list argList;
    va_start(argList, format);
    result = vsprintf_s(buffer, bufferCount, format, argList);
    va_end(argList);
    HOST_OS_API_EXIT();
    return result;
}

extern "C++" template <size_t SIZE> inline int hostOsApiWrapped_sprintf_s(char (&buffer)[SIZE], char const* const format, ...)
{
    int result;
    HOST_OS_API_ENTER();
    va_list argList;
    va_start(argList, format);
    result = vsprintf_s(buffer, format, argList);
    va_end(argList);
    HOST_OS_API_EXIT();
    return result;
}

inline int hostOsApiWrapped_snprintf(char* const buffer, size_t const bufferCount, char const* const format, ...)
{
    int result;
    HOST_OS_API_ENTER();
    va_list argList;
    va_start(argList, format);
    result = vsnprintf(buffer, bufferCount, format, argList);
    va_end(argList);
    HOST_OS_API_EXIT();
    return result;
}

inline int
hostOsApiWrapped_snprintf_s(char* const buffer, size_t const bufferCount, size_t const maxCount, char const* const format, ...)
{
    int result;
    HOST_OS_API_ENTER();
    va_list argList;
    va_start(argList, format);
    result = vsnprintf_s(buffer, bufferCount, maxCount, format, argList);
    va_end(argList);
    HOST_OS_API_EXIT();
    return result;
}

#define vsprintf hostOsApiWrapped_vsprintf
#define vsprintf_s hostOsApiWrapped_vsprintf_s
#define vsnprintf hostOsApiWrapped_vsnprintf
#define vsnprintf_s hostOsApiWrapped_vsnprintf_s
#define sprintf hostOsApiWrapped_sprintf
#define sprintf_s hostOsApiWrapped_sprintf_s
#define snprintf hostOsApiWrapped_snprintf
#define snprintf_s hostOsApiWrapped_snprintf_s

#endif

#else

// Same includes so that we don't make configurations any more complex than they now are
#include <stdio.h>
#include <stdarg.h>

#endif
