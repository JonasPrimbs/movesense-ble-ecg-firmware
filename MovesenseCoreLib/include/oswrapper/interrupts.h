#pragma once
/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__)

/*
* Install an interrupt handler to be called by the simulated interrupt handler
* thread.
*
* @note For FreeRTOS the interrupt number must be in range 3 to 31.
*
* @param interruptNumber Number of the simulated interrupt
* @param interruptHandler Interrupt handler function
*/
void kernelSetSimulatedInterruptHandler(uint32_t interruptNumber, void (*interruptHandler)(void));

/*
* Raise a simulated interrupt
*
* @param interruptNumber Number of the simulated interrupt
*/
void kernelGenerateSimulatedInterrupt(uint32_t interruptNumber);

#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */
