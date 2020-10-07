#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/
/**
    @file macros.h
    Generic macro definitions.
*/
#include <stddef.h>

#if defined(_MSC_VER)
  #if (_MSC_VER < 1800)
    #error Only Visual Studio 2013 and later are supported
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

/**
Macro for getting an offset of a struct member.

\param type Struct type
\param member Member name
\hideinitializer
*/

#ifdef __GNUC__
  // GCC warns about non-POD type member offsetof and warnings are now errors
  // for now the warning is disabled in the GCC builds
  #pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif

#if defined(__builtin_offsetof)
  #define WB_OFFSETOF(type, member) __builtin_offsetof (type, member)
#elif defined(offsetof)
  #define WB_OFFSETOF(type, member) offsetof(type, member)
#else
  #define WB_OFFSETOF(type, member) (size_t)(&(((type*)0)->member))
#endif

/**
Returns the size of a member of a struct.
\hideinitializer
*/
#define WB_SIZEOF_MEMBER(type, member) (size_t)(sizeof(((type*)0)->member))

/**
* Macro for defining variables and types without causing xxx defined but not used warning
*/
#if __GNUC__
  #define WB_ATTRIBUTE_UNUSED __attribute__((unused))
#else
  #define WB_ATTRIBUTE_UNUSED
#endif

/**
* Macro for defining global variables that should not be initialized
*/
#ifdef __ICCARM__
  #define WB_NO_INIT __no_init
#else
  #define WB_NO_INIT
#endif

/** @def WB_STATIC_VERIFY(cond, tag)
Compile time check.
Creates a compile error when the condition is false.

@param cond A compile time condition.
@param tag An unique tag for test, syntactically must be a variable name.

Example:

@code

WB_STATIC_VERIFY(sizeof(uint32) == 4, sizeof_uint32_must_be_4);

@endcode
*/

#define WB_STATIC_VERIFY(cond, tag) typedef char tag##__static_verify__helper[(cond) ? 1 : -1] WB_ATTRIBUTE_UNUSED

/** @def WB_NOT_USED(param)
Turn off param not used warning.

@param param A param.

Example:

@code

static void foo(int bar)
{
    WB_NOT_USED(bar);
}

@endcode
*/
#define WB_NOT_USED(param) ((void)param)

/** @def WB_CONCATENATE(part1, part2)
Forms identifier by concatenating two parts.

@param part1 First part
@param part2 Second part

Example:

@code

static void foo(int bar)
{
    int WB_CONCATENATE(aaa, bbb) = 1;
}

@endcode
*/
#define WB_CONCATENATE(a, b) a##b

/** @def WB_EVAL_AND_CONCATENATE(part1, part2)
Forms identifier by first evaluating parts and then concatenating the results.

@param part1 First part
@param part2 Second part

Example:

@code

static void foo(int bar)
{
    int WB_EVAL_AND_CONCATENATE(aaa, __LINE__) = 1;
}

@endcode
*/
#define WB_EVAL_AND_CONCATENATE(a, b) WB_CONCATENATE(a, b)

/** @def WB_MIN(a, b)
Returns smaller item

@param a item
@param b item

@return smaller item
*/
#define WB_MIN(a, b) ((a) < (b) ? (a) : (b))

/** @def WB_MAX(a, b)
Returns larger item

@param a item
@param b item

@return larger item
*/
#define WB_MAX(a, b) ((a) > (b) ? (a) : (b))

/** Converts non negative number at compile time to next largest
 *  multiple
 *
 * @param number Number to convert
 * @param multiple Factor which multiple will be returned
 */
#define WB_ROUND_UP(number, multiple) \
    (((number) + ((multiple) - 1)) / (multiple) * (multiple))

/** Checks whether given number is power of two
http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
*/
#define WB_IS_POWER_OF_TWO(x)  (((x) != 0) && (((x) & (~(x) + 1)) == (x)))

/** Macro for converting macro parameter to string */
#define WB_STRINGIFY_X(x) #x
#define WB_STRINGIFY(x) WB_STRINGIFY_X(x)

/** Helper for passing comma in macro parameters without causing number of arguments error */
#define WB_COMMA ,

/** Macro for forcing inline expansion of a following function. Use together with WB_FORCE_INLINE_ATTRIBUTE. */
#if defined(__IAR_SYSTEMS_ICC__) && defined(NDEBUG)
#define WB_FORCE_INLINE _Pragma("inline=forced")
#else
#define WB_FORCE_INLINE
#endif

/** Macro for forcing inline expansion of a function. Use together with WB_FORCE_INLINE. */
#ifdef _MSC_VER
#define WB_FORCE_INLINE_ATTRIBUTE __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define WB_FORCE_INLINE_ATTRIBUTE __attribute__((always_inline))
#else
#define WB_FORCE_INLINE_ATTRIBUTE inline
#endif

#if (__cplusplus >= 201103L) || _MSC_VER >= 1900
  #define WB_HAVE_CPLUSPLUS_11
#endif

#if (__cplusplus >= 201402L) || _MSC_VER >= 1910
  #define WB_HAVE_CPLUSPLUS_14
#endif

#if defined(WB_HAVE_CPLUSPLUS_11)

  /** C++11 override specifier that enforces virtual function override. */
  #ifndef OVERRIDE
    #define OVERRIDE override
  #endif

  /** C++11 delete specifier to specify explicitly unimplemented methods. */
  #ifndef DELETED
    #define DELETED = delete
  #endif

  /** C++11 final specifier to specify class & virtual functions that cannot be inherited. */
  #ifndef FINAL
    #define FINAL final
  #endif

  /** C++11 final specifier to specify that only explicit conversion / constructions 
      are allowed. */
  #ifndef EXPLICIT
    #define EXPLICIT explicit
  #endif

  /** C++11 constexpr specifier to specify possible compile time function / variable */
  #ifndef CONSTEXPR
    #define CONSTEXPR constexpr
  #endif

  /** C++11 constexpr specifier to specify possible compile time data */
  #ifndef CONSTEXPR_DATA
    #define CONSTEXPR_DATA constexpr
  #endif

#else // WB_HAVE_CPLUSPLUS_11

  #ifndef OVERRIDE
    #define OVERRIDE /**/
  #endif

  #ifndef DELETED
    #define DELETED /**/
  #endif

  #ifndef FINAL
    #define FINAL /**/
  #endif

  #ifndef EXPLICIT
    #define EXPLICIT /**/
  #endif

  #ifndef CONSTEXPR
    #define CONSTEXPR /**/
  #endif

  #ifndef CONSTEXPR_DATA
    #define CONSTEXPR_DATA const
  #endif
#endif // WB_HAVE_CPLUSPLUS_11

#if defined(WB_HAVE_CPLUSPLUS_14)

  /** C++14 extended constexpr specifier */
  #ifndef CONSTEXPR_14
    #define CONSTEXPR_14 constexpr
  #endif

#else // WB_HAVE_CPLUSPLUS_14

  #ifndef CONSTEXPR_14
    #define CONSTEXPR_14 /**/
  #endif

#endif // WB_HAVE_CPLUSPLUS_11

/** Macro for creating compiler friend multichar literals in big endian format
 */
#define WB_BE_TAG(a,b,c,d) (((a) << 0) | ((b) << 8) | ((c) << 16) | ((d) << 24))

#ifdef WB_HAVE_DEBUG_NAMES
  #define WBDEBUG_NAME(name) name
#else
  #define WBDEBUG_NAME(name) NULL
#endif

// If build environment passed a short name to use for tracing, use that, and
// otherwise use the regular cpp/toolchain provided one
#ifdef TRACE_FILENAME
  #define WB_SOURCE_FILENAME TRACE_FILENAME
#else
  #define WB_SOURCE_FILENAME __FILE__
#endif

#if WB_UNITTEST_BUILD
  #define WB_PUBLIC_IN_UNITTESTS(visibility) public
#else
  #define WB_PUBLIC_IN_UNITTESTS(visibility) visibility
#endif

/** Include header checks */
#include "whiteboard/integration/shared/headerCheck.h"

#if defined(_MSC_VER)
  #if defined(WB_DECLARE_API_EXPORT)
    #define WB_API __declspec(dllexport)
    #define WB_INTERNAL
  #elif defined(WB_DECLARE_API_IMPORT)
    #define WB_API __declspec(dllimport)
    #define WB_INTERNAL
  #endif
#elif defined(__GNUC__) || defined(__clang__) || defined(__hexagon__)
  #define WB_API __attribute__((visibility("default")))
  #define WB_INTERNAL __attribute__((visibility("hidden")))
#endif

#ifndef WB_API
  #define WB_API
  #define WB_INTERNAL
#endif

// Used to force link static GTest symbols from libraries to unit test executables
#define WB_FORCE_LINK_THIS(x) int force_link_##x = 0
#define WB_FORCE_LINK_THAT(x)                                                                         \
    do {                                                                                           \
        extern int force_link_##x;                                                                 \
        force_link_##x = 1;                                                                        \
    } while (0)

