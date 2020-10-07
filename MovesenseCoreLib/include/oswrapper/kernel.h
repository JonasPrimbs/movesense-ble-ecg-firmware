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

void kernelRun(void);
void kernelKill(void); // kill scheduler for debugging purposes. not recoverable

// should be called after HW init and OS init to indicate kernel calls can be made
void kernelReadyToOperate(void);
bool isKernelReady(void);

/// By default the FreeRTOS sleeps 1 ms in prvSimulatedPeripheralTimer
/// This function can be used to add multiplier
void kernelSetTickrate(uint32_t tickRate);

/// In simulator, returns the tick rate set by \ref kernelSetTickrate
/// and on target builds returns portTICK_RATE_MS
uint32_t kernelGetTickrate(void);

/**
 * Convert kernel ticks to milliseconds
 *
 * @param ticks		kernel ticks
 * @return milliseconds
 */
uint32_t kernelConvertTicksToMs(uint32_t ticks);

/**
 * Convert milliseconds to kernel ticks
 *
 * @param milliSeconds		milliseconds
 * @return kernel ticks
 */
uint32_t kernelConvertMsToTicks(uint32_t milliSeconds);

uint32_t kernelGetTickCount(void);
uint32_t kernelGetTickCountMs(void);
uint32_t kernelGetHeapSize(void);
uint32_t kernelGetFreeHeapSize(void);
uint32_t kernelGetMinimumEverFreeHeapSize(void);

/// Convenience function for displaying information about ticks per second in the Simulator
uint32_t kernelGetTicksPerSecond(void);

// Choose something between 1-100 to make ticks run faster.
void setTickMultiplier(int multiplier);
int getTickMultiplier(void);

#if defined(_WIN32) || (defined(__APPLE__) && defined(__MACH__)) || defined(__linux__)

bool kernelHasMemoryAllocationError(void);

/*
* Install an interrupt handler to be called by the simulated interrupt handler
* thread.  The interrupt number must be in range 3 to 31.
*
* Interrupt handler functions must return a non-zero value if executing the
* handler resulted in a task switch being required.
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
