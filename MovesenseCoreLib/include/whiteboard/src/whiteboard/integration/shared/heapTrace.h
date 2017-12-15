#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/integration/shared/types.h"
#include "whiteboard/integration/shared/macros.h"

#ifdef WB_HAVE_HEAP_TRACE

#ifdef __cplusplus
extern "C" {
#endif

/** Logs memory allocation message */
void heapTraceAlloc(const char* callerFile, uint32 callerLine, const char* callerFunc, const char* funcName, uint32 size, const void* pointer);

/** Logs memory deallocation message */
void heapTraceFree(const char* callerFile, uint32 callerLine, const char* callerFunc, const char* funcName, const void* pointer);

/** Logs entry to trace related function */
void heapTraceEnter(const char* callerFile, uint32 callerLine, const char* callerFunc, const char* funcName);

/** Logs exit from trace related function */
void heapTraceExit(const char* callerFile, uint32 callerLine, const char* callerFunc, const char* funcName);

#ifdef __cplusplus
}
#endif

#define WB_HEAP_TRACE_DECLARE_WRAPPER(...) \
    WB_HEAP_TRACE_DECLARE_WRAPPER_N(ARGCOUNT(__VA_ARGS__), __VA_ARGS__)

#ifdef _MSC_VER
#define WB_HEAP_TRACE_DECLARE_WRAPPER_N(n, ...) WB_HEAP_TRACE_DECLARE_WRAPPER_I(n, __VA_ARGS__)
#define WB_HEAP_TRACE_DECLARE_WRAPPER_I(n, ...) WB_CONCATENATE(WB_CONCATENATE(WB_HEAP_TRACE_DECLARE_WRAPPER_, n)(__VA_ARGS__,),)
#else
#define WB_HEAP_TRACE_DECLARE_WRAPPER_N(n, ...) WB_CONCATENATE(WB_HEAP_TRACE_DECLARE_WRAPPER_, n)(__VA_ARGS__,)
#endif

#define WB_HEAP_TRACE_DECLARE_VOID_WRAPPER(...) \
    WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_N(ARGCOUNT(__VA_ARGS__), __VA_ARGS__)

#ifdef _MSC_VER
#define WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_N(n, ...) WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_I(n, __VA_ARGS__)
#define WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_I(n, ...) WB_CONCATENATE(WB_CONCATENATE(WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_, n)(__VA_ARGS__,),)
#else
#define WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_N(n, ...) WB_CONCATENATE(WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_, n)(__VA_ARGS__,)
#endif

#define WB_HEAP_TRACE_DEFINE_WRAPPER(...) \
    WB_HEAP_TRACE_DEFINE_WRAPPER_N(ARGCOUNT(__VA_ARGS__), __VA_ARGS__)

#ifdef _MSC_VER
#define WB_HEAP_TRACE_DEFINE_WRAPPER_N(n, ...) WB_HEAP_TRACE_DEFINE_WRAPPER_I(n, __VA_ARGS__)
#define WB_HEAP_TRACE_DEFINE_WRAPPER_I(n, ...) WB_CONCATENATE(WB_CONCATENATE(WB_HEAP_TRACE_DEFINE_WRAPPER_, n)(__VA_ARGS__,),)
#else
#define WB_HEAP_TRACE_DEFINE_WRAPPER_N(n, ...) WB_CONCATENATE(WB_HEAP_TRACE_DEFINE_WRAPPER_, n)(__VA_ARGS__,)
#endif

#define WB_HEAP_TRACE_DEFINE_VOID_WRAPPER(...) \
    WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_N(ARGCOUNT(__VA_ARGS__), __VA_ARGS__)

#ifdef _MSC_VER
#define WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_N(n, ...) WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_I(n, __VA_ARGS__)
#define WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_I(n, ...) WB_CONCATENATE(WB_CONCATENATE(WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_, n)(__VA_ARGS__,),)
#else
#define WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_N(n, ...) WB_CONCATENATE(WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_, n)(__VA_ARGS__,)
#endif

#define WB_HEAP_TRACE_WRAPPER(...) \
    WB_HEAP_TRACE_WRAPPER_N(ARGCOUNT(__VA_ARGS__), __VA_ARGS__)

#ifdef _MSC_VER
#define WB_HEAP_TRACE_WRAPPER_N(n, ...) WB_HEAP_TRACE_WRAPPER_I(n, __VA_ARGS__)
#define WB_HEAP_TRACE_WRAPPER_I(n, ...) WB_CONCATENATE(WB_CONCATENATE(WB_HEAP_TRACE_WRAPPER_, n)(__VA_ARGS__,),)
#else
#define WB_HEAP_TRACE_WRAPPER_N(n, ...) WB_CONCATENATE(WB_HEAP_TRACE_WRAPPER_, n)(__VA_ARGS__,)
#endif


#define WB_HEAP_TRACE_DECLARE_WRAPPER_2(retType, func, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_3(retType, func, p1Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_4(retType, func, p1Type, p2Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_5(retType, func, p1Type, p2Type, p3Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_6(retType, func, p1Type, p2Type, p3Type, p4Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_7(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_8(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, p6Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5, p6Type p6)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_9(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, p6Type, p7Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5, p6Type p6, p7Type p7)

#define WB_HEAP_TRACE_DECLARE_WRAPPER_10(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, p6Type, p7Type, p8Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5, p6Type p6, p7Type p7, p8Type p8)

#define WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_1(func, ...) \
    void wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc)

#define WB_HEAP_TRACE_DECLARE_VOID_WRAPPER_2(func, p1Type, ...) \
    void wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1)


#define WB_HEAP_TRACE_DEFINE_WRAPPER_2(retType, func, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_3(retType, func, p1Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_4(retType, func, p1Type, p2Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_5(retType, func, p1Type, p2Type, p3Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2, p3); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_6(retType, func, p1Type, p2Type, p3Type, p4Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2, p3, p4); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_7(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2, p3, p4, p5); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_8(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, p6Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5, p6Type p6) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2, p3, p4, p5, p6); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_9(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, p6Type, p7Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5, p6Type p6, p7Type p7) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2, p3, p4, p5, p6, p7); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_WRAPPER_10(retType, func, p1Type, p2Type, p3Type, p4Type, p5Type, p6Type, p7Type, p8Type, ...) \
    retType wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1, p2Type p2, p3Type p3, p4Type p4, p5Type p5, p6Type p6, p7Type p7, p8Type p8) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        retType result = func(p1, p2, p3, p4, p5, p6, p7, p8); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        return result; \
    }

#define WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_1(func, ...) \
    void wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
        func(); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
    }

#define WB_HEAP_TRACE_DEFINE_VOID_WRAPPER_2(func, p1Type, ...) \
    void wbht_ ## func(const char* callerFile, size_t callerLine, const char* callerFunc, p1Type p1) \
    { \
        heapTraceEnter(callerFile, callerLine, callerFile, STRINGIFY(func)); \
        func(p1); \
        heapTraceExit(callerFile, callerLine, callerFunc, STRINGIFY(func)); \
    }

#define WB_HEAP_TRACE_WRAPPER_1(func, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__)
#define WB_HEAP_TRACE_WRAPPER_2(func, p1, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1)
#define WB_HEAP_TRACE_WRAPPER_3(func, p1, p2, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2)
#define WB_HEAP_TRACE_WRAPPER_4(func, p1, p2, p3, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2, p3)
#define WB_HEAP_TRACE_WRAPPER_5(func, p1, p2, p3, p4, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2, p3, p4)
#define WB_HEAP_TRACE_WRAPPER_6(func, p1, p2, p3, p4, p5, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2, p3, p4, p5)
#define WB_HEAP_TRACE_WRAPPER_7(func, p1, p2, p3, p4, p5, p6, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2, p3, p4, p5, p6)
#define WB_HEAP_TRACE_WRAPPER_8(func, p1, p2, p3, p4, p5, p6, p7, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2, p3, p4, p5, p6, p7)
#define WB_HEAP_TRACE_WRAPPER_9(func, p1, p2, p3, p4, p5, p6, p7, p8, ...) wbht_ ## func(__FILE__, __LINE__, __FUNCTION__, p1, p2, p3, p4, p5, p6, p7, p8)

#endif // WB_HAVE_HEAP_TRACE
