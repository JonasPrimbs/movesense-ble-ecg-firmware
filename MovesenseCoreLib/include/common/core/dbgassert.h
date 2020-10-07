#pragma once

#ifdef __cplusplus
extern "C" {
#else

#include <stdbool.h> // bool

#endif

#include "buildconfig/bsp/config.h"
#include <common/compiler/genDef.h>
#include <stddef.h>  // size_t

void systemAbort(void);

void handleStackCorruption(const char* taskName);
void kernelAssertFailed(const char* file, const size_t lineNumber);
void assertFailed(const char* file, const size_t lineNumber, bool systemEvent);
bool verifyFailed(const char* file, const size_t lineNumber);

#ifdef __cplusplus
}
#endif

/* Build environment might have passed a short version of the file, so use that
   if it's present, and if not defined, use the cpp/toolchain provided one */
#ifndef TRACE_FILENAME
#define TRACE_FILENAME __FILE__
#endif

// Undefine ASSERT here, e.g. nRF SDK defines ASSERT macro which is not what we want
#ifdef ASSERT
#undef ASSERT
#endif //ASSERT

#if defined(NDEBUG) && !defined(BUILD_BSP_ENABLE_ASSERT_IN_RELEASE)
// NDEBUG is from stdlib(C) and will disable stdlib assert which is used by
// KSDK and others. If it is passed as additional flag, disable ASSERTs as well
// NOTE: To test, set the CFLAGS and CXXFLAGS in cmake-gui (-DNDEBUG)

// No-op ASSERTs, but ones that "use" the result, to avoid extra compiler
// complaints about unused variables
#define ASSERT(cond)                                                                                                             \
    do                                                                                                                           \
    {                                                                                                                            \
        (void)(cond);                                                                                                            \
    } while (0);
#define ASSERT_NO_SYSTEMEVENT(cond)                                                                                              \
    do                                                                                                                           \
    {                                                                                                                            \
        (void)(cond);                                                                                                            \
    } while (0);

#else // no NDEBUG

#define ASSERT(cond)                                                                                                             \
    do                                                                                                                           \
    {                                                                                                                            \
        if (!(cond))                                                                                                             \
        {                                                                                                                        \
            assertFailed(TRACE_FILENAME, __LINE__, true);                                                                        \
        }                                                                                                                        \
    } while (0);

#define ASSERT_NO_SYSTEMEVENT(cond)                                                                                              \
    do                                                                                                                           \
    {                                                                                                                            \
        if (!(cond))                                                                                                             \
        {                                                                                                                        \
            assertFailed(TRACE_FILENAME, __LINE__, false);                                                                       \
        }                                                                                                                        \
    } while (0);

#endif // no NDEBUG

#ifdef BUILD_WITH_DEBUG
#define DEBUG_ASSERT(cond) ASSERT(cond)
#else
#define DEBUG_ASSERT(cond)
#endif

#define SYSTEM_VERIFY(cond) ((cond) ? true : (verifyFailed(TRACE_FILENAME, __LINE__), false))
#define VERIFY_WITH_RETVAL(cond) SYSTEM_VERIFY(cond)

#define VERIFY SYSTEM_VERIFY
