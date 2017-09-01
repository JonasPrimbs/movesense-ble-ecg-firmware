#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/

#include "common/core/HeapTrace.h"
#include "oswrapper/waitObjectDebug.h"

#if defined(CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES) && defined(CMAKE_NEA_HAVE_HEAP_TRACE)
#error Heap tracing is not supported simultanously with object debug names
#endif

#define OS_API_DEBUG_PARAMS_DECL WAITOBJECT_DEBUGNAME_PARAM_DECL HEAP_TRACE_PARAMS_DECL
#if defined(CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES)
#define OS_API_DEBUG_PARAMS_DECL_VOID WAITOBJECT_DEBUGNAME_PARAM_DECL_VOID
#elif defined(CMAKE_NEA_HAVE_HEAP_TRACE)
#define OS_API_DEBUG_PARAMS_DECL_VOID HEAP_TRACE_PARAMS_DECL_VOID
#else
#define OS_API_DEBUG_PARAMS_DECL_VOID void
#endif
