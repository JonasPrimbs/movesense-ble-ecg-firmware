#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#ifdef WB_DECLARE_API_EXPORT
#define WB_API __declspec(dllexport)
#elif defined(WB_DECLARE_API_IMPORT)
#define WB_API __declspec(dllimport)
#else
#define WB_API
#endif

#include "whiteboard/integration/shared/types.h"

// Define these in your make files / build system
#include WB_BSP

#include "whiteboard/integration/bsp/bsp.h"

#include WB_PORT

#include "whiteboard/integration/os/os.h"

#if defined(_MSC_VER)
#if (_MSC_VER < 1800)
#error Only Visual Studio 2013 and later are supported
#endif
#endif

#if (__cplusplus >= 201103L) || _MSC_VER >= 1900
#define WB_HAVE_CPLUSPLUS_11
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

#endif // WB_HAVE_CPLUSPLUS_11

#ifdef WB_HAVE_DEBUG_NAMES
#define WBDEBUG_NAME(name) name
#else
#define WBDEBUG_NAME(name) NULL
#endif

#if UNITTEST_BUILD
#define WB_PUBLIC_IN_UNITTESTS(visibility) public
#else
#define WB_PUBLIC_IN_UNITTESTS(visibility) visibility
#endif