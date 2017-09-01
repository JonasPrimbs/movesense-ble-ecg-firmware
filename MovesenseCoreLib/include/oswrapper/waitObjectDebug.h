#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/

#include "common/compiler/genDef.h"

#ifdef CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES

#define EVENTGROUP_NAME "E/" __FILE__ ":" STRINGIFY(__LINE__)
#define MUTEX_NAME "M/" __FILE__ ":" STRINGIFY(__LINE__)
#define SEMAPHORE_NAME "S/" __FILE__ ":" STRINGIFY(__LINE__)

#define WAITOBJECT_DEBUGNAME_PARAM_DECL_VOID const char* name
#define WAITOBJECT_DEBUGNAME_PARAM_DECL , WAITOBJECT_DEBUGNAME_PARAM_DECL_VOID
#define WAITOBJECT_DEBUGNAME_PARAM name

#ifdef CMAKE_NEA_HAVE_HEAP_TRACE
#error Heap tracing is not supported simultanously with object debug names
#endif

#else

#define WAITOBJECT_DEBUGNAME_PARAM_DECL
#define WAITOBJECT_DEBUGNAME_PARAM_DECL_VOID void
#define WAITOBJECT_DEBUGNAME_PARAM

#endif
