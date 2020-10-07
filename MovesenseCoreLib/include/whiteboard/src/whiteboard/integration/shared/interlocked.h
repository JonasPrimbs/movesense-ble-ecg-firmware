#pragma once
// Copyright (c) Suunto Oy 2015.

#include "whiteboard/integration/port.h"

#ifdef _MSC_VER

///////////////////////////////////
// Microsoft Visual C/C++ Compiler
//////////////////////////////////

#include <intrin.h>

/** Increments 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @return The resulting incremented value
*/
inline int32 WbInterlockedIncrement32(volatile int32* pAddend)
{
    return _InterlockedIncrement((volatile long*)pAddend);
}

/** Decrements 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be decremented.
* @return The resulting decremented value
*/
inline int32 WbInterlockedDecrement32(volatile int32* pAddend)
{
    return _InterlockedDecrement((volatile long*)pAddend);
}

/** Adds two 32-bit integers in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @param value Incrementation value
* @return The resulting incremented value
*/
inline int32 WbInterlockedAdd32(volatile int32* pAddend, int32 value)
{
    return _InterlockedExchangeAdd((volatile long*)pAddend, value) + value;
}

/** Exchanges 32-bit integer in atomic fashion
*
* @param pTarget A pointer to the variable to be exchanged.
* @param value The value to be exchanged with the value pointed to by Target.
* @return The initial value pointed by pTarget pointer
*/
inline int32 WbInterlockedExchange32(volatile int32* pTarget, int32 value)
{
    return _InterlockedExchange((volatile long*)pTarget, value);
}

/** Exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @return Initial value of pointer pointed by ppTarget
*/
inline void* WbInterlockedExchangePointer(void* volatile* ppTarget, void* pExchange)
{
//VS2013 compatibility fix
#if defined(_MSC_VER) && _MSC_VER < 1900
    return _Intrin_h_InterlockedExchangePointer(ppTarget, pExchange);
#else
    return _InterlockedExchangePointer(ppTarget, pExchange);
#endif
}

/** Compares and exchanges 32-bit integer in atomic fashion
*
* @param pTarget Pointer to the target value.
* @param exchange Exchange value
* @param comparand Value to compare to target. Value pointed by pTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
inline bool WbInterlockedCompareAndExchange32(volatile int32* pTarget, int32 exchange, int32 comparand)
{
    return _InterlockedCompareExchange(
               reinterpret_cast<volatile long*>(pTarget), static_cast<long>(exchange), static_cast<long>(comparand)) == comparand;
}

/** Compares and exchanges pointer value in atomic fashion
 *
 * @param ppTarget Pointer to the pointer which value should be exchanged.
 * @param pExchange Exchange value
 * @param pComparand Value to compare to target. Value pointed by ppTarget pointer is changed only if it equals compare value.
 * @return A value indicating wether the value was exchanged
 */
inline bool WbInterlockedCompareAndExchangePointer(void* volatile* ppTarget, void* pExchange, void* pComparand)
{
    return _InterlockedCompareExchangePointer(ppTarget, pExchange, pComparand) == pComparand;
}

#elif defined(__GNUC__) && !defined(__hexagon__)

///////////////////////////////////
// GNU C/C++ Compiler
//////////////////////////////////

/** Increments 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @return The resulting incremented value
*/
inline int32 WbInterlockedIncrement32(volatile int32* pAddend)
{
    return __sync_add_and_fetch(pAddend, 1);
}

/** Decrements 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be decremented.
* @return The resulting decremented value
*/
inline int32 WbInterlockedDecrement32(volatile int32* pAddend)
{
    return __sync_sub_and_fetch(pAddend, 1);
}

/** Adds two 32-bit integers in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @param value Incrementation value
* @return The resulting incremented value
*/
inline int32 WbInterlockedAdd32(volatile int32* pAddend, int32 value)
{
    return __sync_add_and_fetch(pAddend, value);
}

/** Exchanges 32-bit integer in atomic fashion
*
* @param pTarget A pointer to the variable to be exchanged.
* @param value The value to be exchanged with the value pointed to by Target.
* @return The initial value pointed by pTarget pointer
*/
inline int32 WbInterlockedExchange32(volatile int32* pTarget, int32 value)
{
    // NOTE: __sync_lock_test_and_set would be an acquire barrier, so we force a full barrier
    __sync_synchronize();
    return __sync_lock_test_and_set(pTarget, value);
}

/** Exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @return Initial value of pointer pointed by ppTarget
*/
inline void* WbInterlockedExchangePointer(void* volatile* ppTarget, void* pExchange)
{
    // NOTE: __sync_lock_test_and_set would be an acquire barrier, so we force a full barrier
    __sync_synchronize();
    return __sync_lock_test_and_set(ppTarget, pExchange);
}

/** Compares and exchanges 32-bit integer in atomic fashion
*
* @param pTarget Pointer to the target value.
* @param exchange Exchange value
* @param comparand Value to compare to target. Value pointed by pTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
inline bool WbInterlockedCompareAndExchange32(volatile int32* pTarget, int32 exchange, int32 comparand)
{
    return __sync_bool_compare_and_swap(pTarget, comparand, exchange);
}

/** Compares and exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @param pComparand Value to compare to target. Value pointed by ppTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
inline bool WbInterlockedCompareAndExchangePointer(void* volatile* ppTarget, void* pExchange, void* pComparand)
{
    return __sync_bool_compare_and_swap(ppTarget, pComparand, pExchange);
}

#elif defined(__clang__) && (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_ATOMICS__)

///////////////////////////////////
// C11 stdatomic
//   - Clang C/C++ Compiler
//////////////////////////////////

#include <stdatomic.h>

/** Increments 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @return The resulting incremented value
*/
inline int32 WbInterlockedIncrement32(volatile int32* pAddend)
{
    return atomic_fetch_add(pAddend, 1) + 1;
}

/** Decrements 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be decremented.
* @return The resulting decremented value
*/
inline int32 WbInterlockedDecrement32(volatile int32* pAddend)
{
    return atomic_fetch_sub(pAddend, 1) - 1;
}

/** Adds two 32-bit integers in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @param value Incrementation value
* @return The resulting incremented value
*/
inline int32 WbInterlockedAdd32(volatile int32* pAddend, int32 value)
{
    return atomic_fetch_add(pAddend, value) + value;
}

/** Exchanges 32-bit integer in atomic fashion
*
* @param pTarget A pointer to the variable to be exchanged.
* @param value The value to be exchanged with the value pointed to by Target.
* @return The initial value pointed by pTarget pointer
*/
inline int32 WbInterlockedExchange32(volatile int32* pTarget, int32 value)
{
    return atomic_exchange(pTarget, value);
}

/** Exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @return Initial value of pointer pointed by ppTarget
*/
inline void* WbInterlockedExchangePointer(void* volatile* ppTarget, void* pExchange)
{
    return atomic_exchange(ppTarget, pExchange);
}

/** Compares and exchanges 32-bit integer in atomic fashion
*
* @param pTarget Pointer to the target value.
* @param exchange Exchange value
* @param comparand Value to compare to target. Value pointed by pTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
inline bool WbInterlockedCompareAndExchange32(volatile int32* pTarget, int32 exchange, int32 comparand)
{
    return atomic_compare_exchange_strong(pTarget, echange, comparand);
}

/** Compares and exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @param pComparand Value to compare to target. Value pointed by ppTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
inline bool WbInterlockedCompareAndExchangePointer(void* volatile* ppTarget, void* pExchange, void* pComparand)
{
    return atomic_compare_exchange_strong(ppTarget, pExchange, pComparand);
}

#elif defined(__ICCARM__) || defined(__hexagon__)

////////////////////////////////////////////////////////////////////
// IAR C/C++ Compiler - ARM Architecture
//
////////////////////////////////////////////////////////////////////

/** Increments 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @return The resulting incremented value
*/
int32 WbInterlockedIncrement32(volatile int32* pAddend);

/** Decrements 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be decremented.
* @return The resulting decremented value
*/
int32 WbInterlockedDecrement32(volatile int32* pAddend);

/** Adds two 32-bit integers in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @param value Incrementation value
* @return The resulting incremented value
*/
int32 WbInterlockedAdd32(volatile int32* pAddend, int32 value);

/** Exchanges 32-bit integer in atomic fashion
*
* @param pTarget A pointer to the variable to be exchanged.
* @param value The value to be exchanged with the value pointed to by Target.
* @return The initial value pointed by pTarget pointer
*/
int32 WbInterlockedExchange32(volatile int32* pTarget, int32 value);

/** Exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @return Initial value of pointer pointed by ppTarget
*/
void* WbInterlockedExchangePointer(void* volatile* ppTarget, void* pExchange);

/** Compares and exchanges 32-bit integer in atomic fashion
*
* @param pTarget Pointer to the target value.
* @param exchange Exchange value
* @param comparand Value to compare to target. Value pointed by pTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
bool WbInterlockedCompareAndExchange32(volatile int32* pTarget, int32 exchange, int32 comparand);

/** Compares and exchanges pointer value in atomic fashion
*
* @param ppTarget Pointer to the pointer which value should be exchanged.
* @param pExchange Exchange value
* @param pComparand Value to compare to target. Value pointed by ppTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
bool WbInterlockedCompareAndExchangePointer(void* volatile* ppTarget, void* pExchange, void* pComparand);

#else

#error Interlocked functions have not beed defined for current compiler

#endif

/** Increments 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @return The resulting incremented value
*/
inline uint32 WbInterlockedIncrement32(volatile uint32* pAddend)
{
    return WbInterlockedIncrement32(reinterpret_cast<volatile int32*>(pAddend));
}

/** Decrements 32-bit integer in atomic fashion
*
* @param pAddend A pointer to the variable to be decremented.
* @return The resulting decremented value
*/
inline uint32 WbInterlockedDecrement32(volatile uint32* pAddend)
{
    return WbInterlockedDecrement32(reinterpret_cast<volatile int32*>(pAddend));
}

/** Adds two 32-bit integers in atomic fashion
*
* @param pAddend A pointer to the variable to be incremented.
* @param value Incrementation value
* @return The resulting incremented value
*/
inline uint32 WbInterlockedAdd32(volatile uint32* pAddend, uint32 value)
{
    return WbInterlockedAdd32(reinterpret_cast<volatile int32*>(pAddend), value) + value;
}

/** Exchanges 32-bit integer in atomic fashion
*
* @param pTarget A pointer to the variable to be exchanged.
* @param value The value to be exchanged with the value pointed to by Target.
* @return The initial value pointed by pTarget pointer
*/
inline uint32 WbInterlockedExchange32(volatile uint32* pTarget, uint32 value)
{
    return WbInterlockedExchange32(reinterpret_cast<volatile int32*>(pTarget), value);
}

/** Compares and exchanges 32-bit integer in atomic fashion
*
* @param pTarget Pointer to the target value.
* @param exchange Exchange value
* @param comparand Value to compare to target. Value pointed by pTarget pointer is changed only if it equals compare value.
* @return A value indicating wether the value was exchanged
*/
inline bool WbInterlockedCompareAndExchange32(volatile uint32* pTarget, uint32 exchange, uint32 comparand)
{
    return WbInterlockedCompareAndExchange32(reinterpret_cast<volatile int32*>(pTarget), exchange, comparand);
}
