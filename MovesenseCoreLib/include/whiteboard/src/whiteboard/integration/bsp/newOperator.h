#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

#ifdef WB_HAVE_CPLUSPLUS_11
#include <type_traits>
#define TRIVIALLY_DESTRUCTIBLE_TYPE typename std::enable_if<std::is_trivially_destructible<T>::value, T>::type
#else
#define TRIVIALLY_DESTRUCTIBLE_TYPE T
#endif

namespace whiteboard {

/** Base class for all Whiteboard classes that can be allocated and deallocated dynamically */
class IDynamicallyAllocatable
{
public:
    /** Customized new operator implementation that redirects memory allocation
    * to WbMemAlloc functions. Library user might have its own pool for 
    * Whiteboard memory allocations and we want to direct all allocations and
    * deallocations there.
    *
    * @param size Number of bytes to allocate
    * @return Pointer to allocated memory block. Asserts if there is not enough memory.
    */
    static void* operator new(size_t size)
    {
        // Compiler handles class alignment
        void* pResult = WbMemAlloc(size);
        WB_ASSERT(pResult != NULL);
        return pResult;
    }

    /** Matching customized delete operator
    *
    * @param ptr Pointer to allocated memory block that should be freed
    */
    static void operator delete(void* ptr)
    {
        if (ptr)
        {
            WbMemFree(ptr);
        }
    }

    /** Customized new operator implementation that redirects memory allocation
    * to WbMemAlloc functions. Library user might have its own pool for
    * Whiteboard memory allocations and we want to direct all allocations and
    * deallocations there.
    *
    * @param size Number of bytes to allocate
    * @return Pointer to allocated memory block. Returns NULL if there is not enough memory.
    */
    static void* operator new(size_t size, const std::nothrow_t&)
    {
        // Compiler handles class alignment
        return WbMemAlloc(size);
    }

    /** Matching customized delete operator
    *
    * @param ptr Pointer to allocated memory block that should be freed
    */
    static void operator delete(void* ptr, const std::nothrow_t&)
    {
        if (ptr)
        {
            WbMemFree(ptr);
        }
    }

    /** Customized new operator implementation that redirects memory allocation
    * to WbMemAlloc functions. Library user might have its own pool for
    * Whiteboard memory allocations and we want to direct all allocations and
    * deallocations there.
    *
    * @param size Number of bytes to allocate
    * @return Pointer to allocated memory block. Asserts if there is not enough memory.
    */
    static void* operator new[](size_t size)
    {
        // Compiler handles class alignment
        void* pResult = WbMemAlloc(size);
        WB_ASSERT(pResult != NULL);
        return pResult;
    }

    /** Matching customized delete operator
    *
    * @param ptr Pointer to allocated memory block that should be freed
    */
    static void operator delete[](void* ptr)
    {
        if (ptr)
        {
            WbMemFree(ptr);
        }
    }

    /** Customized new operator implementation that redirects memory allocation
    * to WbMemAlloc functions. Library user might have its own pool for
    * Whiteboard memory allocations and we want to direct all allocations and
    * deallocations there.
    *
    * @param size Number of bytes to allocate
    * @return Pointer to allocated memory block. Returns NULL if there is not enough memory.
    */
    static void* operator new[](size_t size, const std::nothrow_t&)
    {
        // Compiler handles class alignment
        return WbMemAlloc(size);
    }

    /** Matching customized delete operator
    *
    * @param ptr Pointer to allocated memory block that should be freed
    */
    static void operator delete[](void* ptr, const std::nothrow_t&)
    {
        if (ptr)
        {
            WbMemFree(ptr);
        }
    }

    /** placement new operator */
    static void* operator new(size_t /*count*/, void* place)
    {
        return place;
    }
   
    /** placement new array operator */
    static void* operator new[](std::size_t /*count*/, void* place)
    {
        return place;
    }

    /** placement delete */
    static void operator delete(void* /*ptr*/, void* /*place*/)
    {}

    /** placement array delete */
    static void operator delete[](void* /*ptr*/, void* /*place*/)
    {}
};

/** Custom POD type new operator implementation that redirects memory allocation
 * to WbMemAlloc functions. Library user might have its own pool for 
 * Whiteboard memory allocations and we want to direct all allocations and
 * deallocations there.
 *
 * @tparam T Type of object to allocate
 * @return Pointer to allocated object
 */
template <typename T>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* New()
{
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewMustBeUsed);

    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAlloc(sizeof(T)));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T();
    return ptr;
}
template <typename T, typename P1>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* New(P1 p1)
{
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewMustBeUsed);

    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAlloc(sizeof(T)));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T(p1);
    return ptr;
}

/** Matching custom delete operator implementation that redirects memory allocation
 * to WbMemFree functions.
 *
 * @tparam Type of object to allocate
 * @param pointer Pointer to allocated object
 */
template <typename T>
static inline void Delete(T* pointer, TRIVIALLY_DESTRUCTIBLE_TYPE* = 0)
{
    if (pointer)
    {
        // Trivially destructible don't need destructor call
        WbMemFree(pointer);
    }
}

/** Custom POD type new operator implementation that redirects memory allocation
 * to WbMemAlloc functions. Library user might have its own pool for 
 * Whiteboard memory allocations and we want to direct all allocations and
 * deallocations there.
 *
 * @tparam T Type of object to allocate
 * @param itemCount Number of items to allocate
 * @return Pointer to allocated object
 */
template <typename T>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* NewArray(size_t itemCount)
{
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewArrayMustBeUsed);

    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAlloc(sizeof(T) * itemCount));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T[itemCount];
    return ptr;
}

/** Matching custom delete operator implementation that redirects memory allocation
* to WbMemFree functions.
*
* @tparam T Type of object to deallocate
* @param pointer Pointer to allocated object
*/
template <typename T>
static inline void DeleteArray(T* pointer, TRIVIALLY_DESTRUCTIBLE_TYPE* = 0)
{
    if (pointer)
    {
        // Trivially destructible don't need destructor call
        WbMemFree(pointer);
    }
}

/** Custom POD type new operator implementation that redirects memory allocation
* to WbMemAlloc functions. Library user might have its own pool for
* Whiteboard memory allocations and we want to direct all allocations and
* deallocations there.
*
* @tparam T Type of object to allocate
* @return Pointer to allocated object
*/
template <typename T>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* NewAligned()
{
    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAllocAlignedAlways(sizeof(T), WB_TYPE_ALIGNMENT(T)));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T();
    return ptr;
}
template <typename T, typename T1>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* NewAligned(T1 P1)
{
    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAllocAlignedAlways(sizeof(T), WB_TYPE_ALIGNMENT(T)));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T(P1);
    return ptr;
}

/** Matching custom delete operator implementation that redirects memory allocation
* to WbMemFree functions.
*
* @tparam Type of object to allocate
* @param pointer Pointer to allocated object
*/
template <typename T>
static inline void DeleteAligned(T* pointer, TRIVIALLY_DESTRUCTIBLE_TYPE* = 0)
{
    if (pointer)
    {
        // Trivially destructible don't need destructor call
        WbMemFreeAlignedAlways(pointer);
    }
}

/** Custom POD type new operator implementation that redirects memory allocation
* to WbMemAlloc functions. Library user might have its own pool for
* Whiteboard memory allocations and we want to direct all allocations and
* deallocations there.
*
* @tparam T Type of object to allocate
* @param itemCount Number of items to allocate
* @return Pointer to allocated object
*/
template <typename T>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* NewAlignedArray(size_t itemCount)
{
    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAllocAlignedAlways(sizeof(T) * itemCount, WB_TYPE_ALIGNMENT(T)));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T[itemCount];
    return ptr;
}

/** Matching custom delete operator implementation that redirects memory allocation
* to WbMemFree functions.
*
* @tparam T Type of object to allocate
* @param itemCount Number of items to deallocate
* @param pointer Pointer to allocated object
*/
template <typename T>
static inline void DeleteAlignedArray(T* pointer, TRIVIALLY_DESTRUCTIBLE_TYPE* = 0)
{
    if (pointer)
    {
        // Trivially destructible don't need destructor call
        WbMemFreeAlignedAlways(pointer);
    }
}

#ifdef WB_HAVE_HEAP_TRACE

class ht_New
{
public:
    const char* mCallerFile;
    size_t mCallerLine;
    const char* mCallerFunc;

    ht_New(const char* callerFile, size_t callerLine, const char* callerFunc)
        : mCallerFile(callerFile), mCallerLine(callerLine), mCallerFunc(callerFunc)
    {
    }

    template <typename T>
    inline TRIVIALLY_DESTRUCTIBLE_TYPE* call()
    {
        heapTraceEnter(mCallerFile, mCallerLine, mCallerFunc, "New");
        TRIVIALLY_DESTRUCTIBLE_TYPE* result = New<T>();
        heapTraceExit(mCallerFile, mCallerLine, mCallerFunc, "New");
        return result;
    }

    template <typename T, typename P1>
    inline TRIVIALLY_DESTRUCTIBLE_TYPE* call(P1 p1)
    {
        heapTraceEnter(mCallerFile, mCallerLine, mCallerFunc, "New");
        TRIVIALLY_DESTRUCTIBLE_TYPE* result = New<T, P1>(p1);
        heapTraceExit(mCallerFile, mCallerLine, mCallerFunc, "New");
        return result;
    }
};

#define New ht_New(__FILE__, __LINE__, __FUNCTION__).call

class ht_NewArray
{
public:
    const char* mCallerFile;
    size_t mCallerLine;
    const char* mCallerFunc;

    ht_NewArray(const char* callerFile, size_t callerLine, const char* callerFunc)
        : mCallerFile(callerFile), mCallerLine(callerLine), mCallerFunc(callerFunc)
    {
    }

    template <typename T>
    inline TRIVIALLY_DESTRUCTIBLE_TYPE* call(size_t itemCount)
    {
        heapTraceEnter(mCallerFile, mCallerLine, mCallerFunc, "NewArray");
        TRIVIALLY_DESTRUCTIBLE_TYPE* result = NewArray<T>(itemCount);
        heapTraceExit(mCallerFile, mCallerLine, mCallerFunc, "NewArray");
        return result;
    }
};

#define NewArray ht_NewArray(__FILE__, __LINE__, __FUNCTION__).call

class ht_NewAligned
{
public:
    const char* mCallerFile;
    size_t mCallerLine;
    const char* mCallerFunc;

    ht_NewAligned(const char* callerFile, size_t callerLine, const char* callerFunc)
        : mCallerFile(callerFile), mCallerLine(callerLine), mCallerFunc(callerFunc)
    {
    }

    template <typename T>
    inline TRIVIALLY_DESTRUCTIBLE_TYPE* call()
    {
        heapTraceEnter(mCallerFile, mCallerLine, mCallerFunc, "NewAligned");
        TRIVIALLY_DESTRUCTIBLE_TYPE* result = NewAligned<T>();
        heapTraceExit(mCallerFile, mCallerLine, mCallerFunc, "NewAligned");
        return result;
    }

    template <typename T, typename P1>
    inline TRIVIALLY_DESTRUCTIBLE_TYPE* call(P1 p1)
    {
        heapTraceEnter(mCallerFile, mCallerLine, mCallerFunc, "NewAligned");
        TRIVIALLY_DESTRUCTIBLE_TYPE* result = NewAligned<T, P1>(p1);
        heapTraceExit(mCallerFile, mCallerLine, mCallerFunc, "NewAligned");
        return result;
    }
};

#define NewAligned ht_NewAligned(__FILE__, __LINE__, __FUNCTION__).call

class ht_NewAlignedArray
{
public:
    const char* mCallerFile;
    size_t mCallerLine;
    const char* mCallerFunc;

    ht_NewAlignedArray(const char* callerFile, size_t callerLine, const char* callerFunc)
        : mCallerFile(callerFile), mCallerLine(callerLine), mCallerFunc(callerFunc)
    {
    }

    template <typename T>
    inline TRIVIALLY_DESTRUCTIBLE_TYPE* call(size_t itemCount)
    {
        heapTraceEnter(mCallerFile, mCallerLine, mCallerFunc, "NewAlignedArray");
        TRIVIALLY_DESTRUCTIBLE_TYPE* result = NewAlignedArray<T>(itemCount);
        heapTraceExit(mCallerFile, mCallerLine, mCallerFunc, "NewAlignedArray");
        return result;
    }
};

#define NewAlignedArray ht_NewAlignedArray(__FILE__, __LINE__, __FUNCTION__).call

#endif

}
