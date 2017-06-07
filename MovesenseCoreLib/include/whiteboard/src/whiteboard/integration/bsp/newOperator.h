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
    * @return Pointer to allocated memory block
    */
    static void* operator new(size_t size)
    {
        // Compiler handles class alignment
        return WbMemAlloc(size);
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
    * @return Pointer to allocated memory block
    */
    static void* operator new[](size_t size)
    {
        // Compiler handles class alignment
        return WbMemAlloc(size);
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
template <typename T, typename T1>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* New(T1 P1)
{
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewMustBeUsed);

    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAlloc(sizeof(T)));

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
* @tparam T Type of object to allocate
* @param itemCount Number of items to allocate
* @param pointer Pointer to allocated object
*/
template <typename T>
static inline void DeleteArray(T* pointer, size_t itemCount, TRIVIALLY_DESTRUCTIBLE_TYPE* = 0)
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
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewAlignedMustBeUsed);

    // No assert here, because memory allocation failures are already asserted in WbMemAlloc
    T* ptr = static_cast<T*>(WbMemAllocAlignedAlways(sizeof(T), WB_TYPE_ALIGNMENT(T)));

    // Clear memory to its default values by using compiler generated default constructor
    new (ptr) T();
    return ptr;
}
template <typename T, typename T1>
static inline TRIVIALLY_DESTRUCTIBLE_TYPE* NewAligned(T1 P1)
{
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewAlignedMustBeUsed);

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
    WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(T) <= WB_MALLOC_ALIGNMENT, AlignedNewAlignedArrayMustBeUsed);

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
* @param itemCount Number of items to allocate
* @param pointer Pointer to allocated object
*/
template <typename T>
static inline void DeleteAlignedArray(T* pointer, size_t itemCount, TRIVIALLY_DESTRUCTIBLE_TYPE* = 0)
{
    if (pointer)
    {
        // Trivially destructible don't need destructor call
        WbMemFreeAlignedAlways(pointer);
    }
}

}
