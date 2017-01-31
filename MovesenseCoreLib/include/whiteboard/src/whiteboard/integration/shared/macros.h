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

#ifndef ELEMENTS
/**
Returns the number of elements in an array.
\param	array	A symbol name of an array.
\hideinitializer
*/
#define ELEMENTS(array) (sizeof(array) / sizeof(array[0]))
#endif

#ifndef WB_OFFSETOF
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
#endif  // #ifndef WB_OFFSETOF

#ifndef WB_SIZEOF_MEMBER

/**
Returns the size of a member of a struct.
\hideinitializer
*/
#define WB_SIZEOF_MEMBER(type, member) (size_t)(sizeof(((type*)0)->member))

#endif

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

#ifndef WB_STATIC_VERIFY
#define WB_STATIC_VERIFY(cond, tag) typedef char tag##__static_verify__helper[(cond) ? 1 : -1] WB_ATTRIBUTE_UNUSED
#endif

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
#ifndef WB_NOT_USED
#define WB_NOT_USED(param) ((void)param)
#endif

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
#ifndef WB_CONCATENATE
#define WB_CONCATENATE(a, b) a##b
#endif

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
#ifndef WB_EVAL_AND_CONCATENATE
#define WB_EVAL_AND_CONCATENATE(a, b) WB_CONCATENATE(a, b)
#endif

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

/** Macro for converting macro parameter to string */
#ifndef WB_STRINGIFY
#define WB_STRINGIFY_X(x) #x
#define WB_STRINGIFY(x) WB_STRINGIFY_X(x)
#endif

/** Helper for passing comma in macro parameters without causing number of arguments error */
#ifndef WB_COMMA
#define WB_COMMA ,
#endif

/** Include header checks */
#include "whiteboard/integration/shared/headerCheck.h"
