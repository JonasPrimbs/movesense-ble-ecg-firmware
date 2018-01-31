#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/integration/port.h"

// Interrupt handler prototype
typedef void (*WbInterruptHandler)(void);

#ifdef MIM_HAVE_INTERRUPT_API

/** During ISR checks whether higher priority task has been woken during any of the 
* Whiteboard API calls.
*
* @note Should be called only once in a interrupt
*/
WB_API bool WbHigherPriorityTaskWokenISR();

/** Installs interrupt handler for given interrupt.
 *
 * @param interruptNumber Index of the interrupt which handler should be set.
 * @param interruptHandler Interrupt handler or NULL if interrupt handler should be removed.
 */
WB_API void WbSetInterruptHandler(uint32 interruptNumber, WbInterruptHandler interruptHandler);

/** Raises a software interrupt
 *
 * @param interruptNumber Index of the interrupt which should be raised
 */
WB_API void WbRaiseInterrupt(uint32 interruptNumber);

#endif
