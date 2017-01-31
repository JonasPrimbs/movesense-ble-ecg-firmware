#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard {

/** Custom new operator implementation that redirects memory allocation
 * to WbMemAlloc functions. Library user might have its own pool for 
 * Whiteboard memory allocations and we want to direct all allocations and
 * deallocations there.
 *
 * @tparam T Type of object to allocate
 * @return Pointer to allocated object
 */
template <typename T>
static inline T* New()
{
    T* ptr = static_cast<T*>(WbMemAllocAligned(sizeof(T), WB_TYPE_ALIGNMENT(T)));
    WB_ASSERT(ptr != NULL);
    new (ptr) T();
    return ptr;
}

/** Custom new operator implementation that redirects memory allocation
 * to WbMemAlloc functions. Library user might have its own pool for 
 * Whiteboard memory allocations and we want to direct all allocations and
 * deallocations there.
 *
 * @tparam T Type of object to allocate
 * @tparam P1 Type of first parameter to constructor
 * @param param1 First parameter to constructor
 * @return Pointer to allocated object
 */
template <typename T, typename P1>
static inline T* New(P1 param1)
{
    T* ptr = static_cast<T*>(WbMemAllocAligned(sizeof(T), WB_TYPE_ALIGNMENT(T)));
    WB_ASSERT(ptr != NULL);
    new (ptr) T(param1);
    return ptr;
}

/** Custom new operator implementation that redirects memory allocation
 * to WbMemAlloc functions. Library user might have its own pool for 
 * Whiteboard memory allocations and we want to direct all allocations and
 * deallocations there.
 *
 * @tparam T Type of object to allocate
 * @tparam P1 Type of first parameter to constructor
 * @tparam P2 Type of second parameter to constructor
 * @param param1 First parameter to constructor
 * @param param2 Second parameter to constructor
 * @return Pointer to allocated object
 */
template <typename T, typename P1, typename P2>
static inline T* New(P1 param1, P2 param2)
{
    T* ptr = static_cast<T*>(WbMemAllocAligned(sizeof(T), WB_TYPE_ALIGNMENT(T)));
    WB_ASSERT(ptr != NULL);
    new (ptr) T(param1, param2);
    return ptr;
}

/** Custom new operator implementation that redirects memory allocation
 * to WbMemAlloc functions. Library user might have its own pool for 
 * Whiteboard memory allocations and we want to direct all allocations and
 * deallocations there.
 *
 * @tparam T Type of object to allocate
 * @tparam P1 Type of first parameter to constructor
 * @tparam P2 Type of second parameter to constructor
 * @tparam P3 Type of third parameter to constructor
 * @param param1 First parameter to constructor
 * @param param2 Second parameter to constructor
 * @param param3 Third parameter to constructor
 * @return Pointer to allocated object
 */
template <typename T, typename P1, typename P2, typename P3>
static inline T* New(P1 param1, P2 param2, P3 param3)
{
    T* ptr = static_cast<T*>(WbMemAllocAligned(sizeof(T), WB_TYPE_ALIGNMENT(T)));
    WB_ASSERT(ptr != NULL);
    new (ptr) T(param1, param2, param3);
    return ptr;
}

/** Custom new operator implementation that redirects memory allocation
* to WbMemAlloc functions. Library user might have its own pool for
* Whiteboard memory allocations and we want to direct all allocations and
* deallocations there.
*
* @tparam T Type of object to allocate
* @tparam P1 Type of first parameter to constructor
* @tparam P2 Type of second parameter to constructor
* @tparam P3 Type of third parameter to constructor
* @tparam P4 Type of fourth parameter to constructor
* @param param1 First parameter to constructor
* @param param2 Second parameter to constructor
* @param param3 Third parameter to constructor
* @param param4 Fourth parameter to constructor
* @return Pointer to allocated object
*/
template <typename T, typename P1, typename P2, typename P3, typename P4>
static inline T* New(P1 param1, P2 param2, P3 param3, P4 param4)
{
    T* ptr = static_cast<T*>(WbMemAllocAligned(sizeof(T), WB_TYPE_ALIGNMENT(T)));
    WB_ASSERT(ptr != NULL);
    new (ptr) T(param1, param2, param3, param4);
    return ptr;
}

/** Matching custom delete operator implementation that redirects memory allocation
 * to WbMemFree functions.
 *
 * @tparam Type of object to allocate
 * @param pointer Pointer to allocated object
 */
template <typename T>
static inline void Delete(T* pointer)
{
    if (pointer)
    {
        pointer->T::~T();
        WbMemFreeAligned(pointer);
    }
}

/** Custom array new operator implementation that redirects memory allocation
* to WbMemAlloc functions. Library user might have its own pool for
* Whiteboard memory allocations and we want to direct all allocations and
* deallocations there.
*
* @tparam T Type of object to allocate
* @param itemCount Number of items to allocate
* @return Pointer to allocated object array
*/
template <typename T>
static inline T* NewArray(size_t itemCount)
{
    T* ptr = static_cast<T*>(WbMemAllocAligned(sizeof(T) * itemCount, WB_TYPE_ALIGNMENT(T)));
    WB_ASSERT(ptr != NULL);
    new (ptr) T[itemCount];
    return ptr;
}

/** Matching custom delete array operator implementation that redirects memory allocation
* to WbMemFree functions.
*
* @tparam Type of object to allocate
* @param pointer Pointer to allocated object array
* @param itemCount Number of items in the array (number of items for which placement destruct is called)
*/
template <typename T>
static inline void DeleteArray(T* pointer, size_t itemCount)
{
    if (pointer)
    {
        for (T* pCurrent = pointer; itemCount > 0; --itemCount)
        {
            pCurrent->T::~T();
            ++pCurrent;
        }

        WbMemFreeAligned(pointer);
    }
}

}
