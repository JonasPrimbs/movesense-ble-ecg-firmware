#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/
#include <stdint.h>
#include "common/compiler/genDef.h"

#ifndef CMAKE_NEA_HAVE_HEAP_TRACE

#define HEAP_TRACE_ENABLE(code)
#define HEAP_TRACE_FUNC(func) func
#define HEAP_TRACE_PARAMS_DECL
#define HEAP_TRACE_PARAMS_DECL_VOID void
#define HEAP_TRACE_PARAMS
#define HEAP_TRACE_PARAMS_VOID

#else

#define HEAP_TRACE_FUNC(func) trace_##func

#if defined(__IAR_SYSTEMS_ICC__) || defined(__GNUC__)
// IAR and GCC expand __FUNCTION__ macro at compile time and cannot use string concateation done by preprocessor / lexer
#define HEAP_TRACE_FUNCTION_NAME_NOT_CONCATENATED
#define HEAP_TRACE_MESSAGE(extra) __FILE__ "(" STRINGIFY(__LINE__) "): HEAP " extra, __FUNCTION__
#define HEAP_TRACE_PARAMS_DECL_VOID const char *message, const char *callerFuncName
#define HEAP_TRACE_PARAMS_VOID message, callerFuncName
#else
#define HEAP_TRACE_MESSAGE(extra) __FILE__ "(" STRINGIFY(__LINE__) "): HEAP " extra __FUNCTION__
#define HEAP_TRACE_PARAMS_DECL_VOID const char* message
#define HEAP_TRACE_PARAMS_VOID message
#endif

#define HEAP_TRACE_PARAMS_DECL , HEAP_TRACE_PARAMS_DECL_VOID
#define HEAP_TRACE_PARAMS , HEAP_TRACE_PARAMS_VOID

#define HEAP_TRACE_ALLOC HEAP_TRACE_MESSAGE("")
#define HEAP_TRACE_FREE HEAP_TRACE_MESSAGE("")
#define HEAP_TRACE_ENTER(funcName) HEAP_TRACE_MESSAGE("FUNC " funcName " ")

#ifdef __cplusplus
extern "C" {
#endif

void heapTraceAlloc(uint32_t size, const void* pointer HEAP_TRACE_PARAMS_DECL);
void heapTraceFree(const void* pointer HEAP_TRACE_PARAMS_DECL);
void heapTraceEnter(HEAP_TRACE_PARAMS_DECL_VOID);
void heapTraceExit(HEAP_TRACE_PARAMS_DECL_VOID);

#ifdef __cplusplus
}
#endif

#endif // CMAKE_NEA_HAVE_HEAP_TRACE
