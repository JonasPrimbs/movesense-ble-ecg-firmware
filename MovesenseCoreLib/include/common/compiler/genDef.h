#pragma once
/**
    @file genDef.h
    Common definitions, perhaps defined already in some other file, but still...
*/

#include <string.h>
#include <limits.h>
#include <stdint.h>

#ifdef __cplusplus
// Flag that enables C++ API section of C/C++ headers
#define CPP_API
// For API definitions
#define C_API extern "C"
// For data definitions
#define EXTERN_C extern "C"
#else
// For API definitions
#define C_API
// For data definitions
#define EXTERN_C extern
#endif

#ifndef OFFSETOF
/**
    Macro for getting an offset of a struct member.

    \param type Struct type
    \param member Member name
    \hideinitializer
*/
#ifdef offsetof
#define OFFSETOF offsetof
#else
#define OFFSETOF(type, member) (size_t)(&(((type*)0)->member))
#endif
#endif

#ifndef ELEMENTS
/**
    Returns the number of elements in an array.
    \param	array	A symbol name of an array.
    \hideinitializer
*/
#define ELEMENTS(array) (sizeof(array) / sizeof(array[0]))
#endif

#ifndef BITSIZEOF
/**
    Returns the number of bits in a variable.
    \param	var	name of a variable.
    \hideinitializer
*/
#define BITSIZEOF(var) (sizeof(var) * CHAR_BIT)
#endif

#ifndef SIZEOF_MEMBER

/**
   Returns the size of a member of a struct.
   \hideinitializer
*/
#define SIZEOF_MEMBER(type, member) (size_t)(sizeof(((type*)0)->member))

#endif

#ifndef OFFSETOF_OBJ
/** @def OFFSETOF_OBJ(Object, Field)
    Calculate offset of a member field of an object.

    @param Object An object,
    @param Field A field.

    @return offset.
*/
#define OFFSETOF_OBJ(Object, Field) ((size_t) & ((Object).Field) - ((size_t) & (Object)))

#endif

/** Macro for converting macro parameter to string */
#ifndef STRINGIFY
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#endif

/* Performs macro expansion of given argument */
#define EXPAND(x) x

/*
Macro for calculating argument count. Example:
ARGCOUNT( a, b, 1, 2, 3 ) evaluates to 5
*/
//#define ARGCOUNT(...) (sizeof((int[]){__VA_ARGS__})/sizeof(int))
#define ARGCOUNT_N(_1,                                                                                                           \
                   _2,                                                                                                           \
                   _3,                                                                                                           \
                   _4,                                                                                                           \
                   _5,                                                                                                           \
                   _6,                                                                                                           \
                   _7,                                                                                                           \
                   _8,                                                                                                           \
                   _9,                                                                                                           \
                   _10,                                                                                                          \
                   _11,                                                                                                          \
                   _12,                                                                                                          \
                   _13,                                                                                                          \
                   _14,                                                                                                          \
                   _15,                                                                                                          \
                   _16,                                                                                                          \
                   _17,                                                                                                          \
                   _18,                                                                                                          \
                   _19,                                                                                                          \
                   _20,                                                                                                          \
                   _21,                                                                                                          \
                   _22,                                                                                                          \
                   _23,                                                                                                          \
                   _24,                                                                                                          \
                   _25,                                                                                                          \
                   _26,                                                                                                          \
                   _27,                                                                                                          \
                   _28,                                                                                                          \
                   _29,                                                                                                          \
                   _30,                                                                                                          \
                   _31,                                                                                                          \
                   _32,                                                                                                          \
                   _33,                                                                                                          \
                   _34,                                                                                                          \
                   _35,                                                                                                          \
                   _36,                                                                                                          \
                   _37,                                                                                                          \
                   _38,                                                                                                          \
                   _39,                                                                                                          \
                   _40,                                                                                                          \
                   _41,                                                                                                          \
                   _42,                                                                                                          \
                   _43,                                                                                                          \
                   _44,                                                                                                          \
                   _45,                                                                                                          \
                   _46,                                                                                                          \
                   _47,                                                                                                          \
                   _48,                                                                                                          \
                   _49,                                                                                                          \
                   _50,                                                                                                          \
                   _51,                                                                                                          \
                   _52,                                                                                                          \
                   _53,                                                                                                          \
                   _54,                                                                                                          \
                   _55,                                                                                                          \
                   _56,                                                                                                          \
                   _57,                                                                                                          \
                   _58,                                                                                                          \
                   _59,                                                                                                          \
                   _60,                                                                                                          \
                   _61,                                                                                                          \
                   _62,                                                                                                          \
                   _63,                                                                                                          \
                   N,                                                                                                            \
                   ...)                                                                                                          \
    N
#define ARGCOUNT_SEQ                                                                                                             \
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33,  \
        32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define ARGCOUNT_(...) EXPAND(ARGCOUNT_N(__VA_ARGS__))
#define ARGCOUNT(...) EXPAND(ARGCOUNT_(__VA_ARGS__, ARGCOUNT_SEQ))

/**
   General values for unknown or error states.
*/
#define ERROR_UINT8 255
#define ERROR_INT8 -128
#define ERROR_UINT16 65535
#define ERROR_INT16 -32768
#define ERROR_UINT32 4294967295
#define ERROR_INT32 -2147483648

#ifndef WHIBYTE_IMPLEMENTATION
#define WHIBYTE_IMPLEMENTATION(var16) (var16 >> 8)
#endif

/**
    A macro to get the high byte of a word.

    \param	var16	16 bit variable (word) to get the high byte of.

    \return	High byte of the given word.
*/
#define WHIBYTE(var16) WHIBYTE_IMPLEMENTATION(var16)

/** @def HIGH_BYTE(val)
    Extract high byte from word size value;
    in general second lowest byte from an integral value.

    @param val An integral value.

    @return second lowest byte value.
*/
#define HIGH_BYTE(val) (((val) >> 8) & 0xff)

/** @def LOW_BYTE(val)
    Extract the least significant byte from an integral value.

    @param val An integral value.

    @return low byte value.
*/
#define LOW_BYTE(var) ((var)&0xff)

/** @def HIGH_WORD(val)
    Return high order word from an integral value.

    @param val A integral value.

    @return high word.
*/
#define HIGH_WORD(val) (((val) >> 16) & 0xffff)

/** @def LOW_WORD(val)
    Return order order word from an integral value.

    @param val A integral value.

    @return low word.
*/
#define LOW_WORD(val) ((val)&0xffff)

/** @def MAKE_WORD(msb, lsb)
    Build a word from two integral values.

    @param msb A most significant byte.
    @param lsb A least significant.

    @return a word.
*/
#define MAKE_WORD(msb, lsb) (((((msb) + 0) & 0xff) << 8) | ((lsb)&0xff))

/** @def MAKE_DWORD(msw, lsw)
    Build a double word from two integral values.

    @param msw A most significant word.
    @param lsw A least significant word.

    @return a double word.
*/
#define MAKE_DWORD(msw, lsw) (((((msw) + 0L) & 0xffff) << 16) | ((lsw)&0xffff))

/** @def MIN(a, b)
    Returns smaller item

    @param a item
    @param b item

    @return smaller item
*/
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

/** @def MAX(a, b)
    Returns larger item

    @param a item
    @param b item

    @return larger item
*/
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

/** @def STATIC_VERIFY(cond, tag)
    Compile time check.
    Creates a compile error when the condition is false.

    @param cond A compile time condition.
    @param tag An unique tag for test, syntactically must be a variable name.

    Example:

    @code
    #include "common/compiler/genDef.h"

    STATIC_VERIFY(sizeof(uint32) == 4, sizeof_uint32_must_be_4);

    @endcode
*/
#ifndef STATIC_VERIFY
#ifdef __cplusplus
#ifdef __IAR_SYSTEMS_ICC__ // IAR 8.x support (C++14)
#define STATIC_VERIFY(cond, tag) static_assert((cond), #tag)
#else
/* In C11, static_assert is a macro that does not work together with ours.
   Using the keyword directly instead. */
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define STATIC_VERIFY(cond, tag) _Static_assert((cond), #tag)
#else
/* For G++, MSVC and IAR 7.x compilers */
#define STATIC_VERIFY(cond, tag) static_assert((cond), #tag)
#endif // defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#endif // __IAR_SYSTEMS_ICC__
#else
#define STATIC_VERIFY(cond, tag) typedef char tag##__static_verify__helper[(cond) ? 1 : -1]
#endif // __cplusplus
#endif // STATIC_VERIFY

/** @def PARAM_NOTUSED(param)
    Turn off param not used warning.

    @param param A param.

    Example:

    @code
    #include "common/compiler/genDef.h"

    static void foo(int bar)
    {
        PARAM_NOTUSED(bar);
    }

    @endcode
*/
#define PARAM_NOTUSED(param) ((void)param)

#ifdef ARCH_CORTEX
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 10000
#endif

#define FOURCC(a, b, c, d) ((((uint32)(a)) << 24) | (((uint32)(b)) << 16) | (((uint32)(c)) << 8) | (d)) // Little Endian
#define FOURCC_BE(a, b, c, d) (((uint32)(a)) | (((uint32)(b)) << 8) | (((uint32)(c) << 16)) | ((uint32)(d) << 24)) // Big Endian

#include "cpp11port.hpp"

/// Mark variables that might be unused (due to a codepath being ifdef'd out)
/// so that GCC won't warn about them.
#ifdef __GNUC__
#define ATTRIBUTE_UNUSED __attribute__((unused))
#else
#define ATTRIBUTE_UNUSED
#endif

/// Mark variables that might be unused (due to a codepath being ifdef'd out)
/// so that GCC won't warn about them.
#ifdef __GNUC__
       #define WEAK __attribute__((weak))
#else
       #define WEAK
#endif

// Detect whether doing a debug or release version
// With msbuild, we want to use the per build setting, while on others we'll
// want to obey the CMAKE_BUILD_TYPE setting (indirectly)
#if defined(_MSC_VER)
// If _DEBUG is set with MSC, then debugs are enabled (don't use NDEBUG,
// which is used to control asserts and is portable).
#if defined(_DEBUG)
#ifndef BUILD_WITH_DEBUG
#define BUILD_WITH_DEBUG
#endif
#elif defined(BUILD_WITH_DEBUG)
// Undo the define from cmake
#undef BUILD_WITH_DEBUG
#endif
#else
// BUILD_WITH_DEBUG set by cmake is obeyed on other build systems
#endif

// Include wrappers for host OS API functions
#include "oswrapper/hostOs.h"
