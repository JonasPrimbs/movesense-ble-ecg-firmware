#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
// Copyright (c) Suunto Oy 2016. All rights reserved

/* Minimum set of features to compile Whiteboard against FreeRTOS headers
 *
 * Check that these definitions match in your REAL FreeRTOSConfig.h file
 */

 /* Constants related to the behaviour or the scheduler. */
#define configUSE_PREEMPTION					1
#define configUSE_TIME_SLICING					1
#define configMAX_PRIORITIES					( 5 )

/* Constants that describe the hardware and memory usage. */

/* Code using Whiteboard needs to declare this symbol, for example in code that initializes the Whiteboard. 
   This has to be done in order for the port to use same tick rate as the host application */
extern const uint32 gWbPortFreeRtosTickRate_Hz;
#define configTICK_RATE_HZ						gWbPortFreeRtosTickRate_Hz

/* Constants that build features in or out. */
#define configUSE_MUTEXES						1
#define configUSE_COUNTING_SEMAPHORES 			1
#define configUSE_RECURSIVE_MUTEXES				1
#define configUSE_TASK_NOTIFICATIONS			1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 1 /* At least one for Whiteboard. Rest can be used as you wish. */

/* Software timer definitions. */
#define configUSE_TIMERS						1
#define configTIMER_TASK_PRIORITY				( 4 ) /* 'Tmr Svc' -thread priority should be higher than application */
#define configTIMER_QUEUE_LENGTH				10 /* For example. May be more or less.  */
#define configTIMER_TASK_STACK_DEPTH			90 /* For example. May be more or less. */

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.  NOTE:  Setting an INCLUDE_ parameter to 0 is only
necessary if the linker does not automatically remove functions that are not
referenced anyway. */
#define INCLUDE_vTaskPrioritySet				0
#define INCLUDE_uxTaskPriorityGet				0
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskCleanUpResources			0
#define INCLUDE_vTaskSuspend					0
#define INCLUDE_vTaskDelayUntil					0
#define INCLUDE_vTaskDelay						1
#define INCLUDE_uxTaskGetStackHighWaterMark		0
#define INCLUDE_pcTaskGetTaskName				0
#define INCLUDE_xTaskGetIdleTaskHandle			1
#define INCLUDE_eTaskGetState					0
#define INCLUDE_xTaskResumeFromISR				0
#define INCLUDE_xTaskGetCurrentTaskHandle		1
#define INCLUDE_xTaskGetSchedulerState			0
#define INCLUDE_xSemaphoreGetMutexHolder		0
#define INCLUDE_xTimerPendFunctionCall			0

/////////////////////////////////////////////////////////////////////////////////////////
// Runtime statistics.
//
// Add following code segment to your FreeRTOSConfig.h file
// and implement freertosInitialiseRunTimeStats & freertosGetRunTimeStatsCounterValue
// functions.
//
// For more information see http://www.freertos.org/rtos-run-time-stats.html
/////////////////////////////////////////////////////////////////////////////////////////
#if 0
#define configGENERATE_RUN_TIME_STATS	1

/* This file is included also by assembler, but these functions are only for C/C++ */
#if defined(__STDC__) || defined(__cplusplus) || defined (_MSC_VER)

#ifdef __cplusplus
extern "C" {
#endif

    // Runtime timer interface. These functions are implemented
    // in the platform code. Actual location is implementation
    // dependant, but rtc.cpp would be good place to look for.
    void WbBspInitialiseRunTimeStatsTimer(void);
    unsigned long WbBspGetRunTimeStatsTimerValue(void);

    // Additional trace function to capture number of context switches
    void WbThreadRuntimeStatisticsContextSwitch(unsigned long threadNumber);

#ifdef __cplusplus
}
#endif
#endif

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() WbBspInitialiseRunTimeStatsTimer()
#define portGET_RUN_TIME_COUNTER_VALUE() WbBspGetRunTimeStatsTimerValue()
#define traceTASK_SWITCHED_IN() WbThreadRuntimeStatisticsContextSwitch((unsigned long)pxCurrentTCB->uxTCBNumber)
#endif

/** Whiteboard doesn't need these and they have only dummy value. These are only here because FreeRTOS would
 * otherwise issue errors for missing configuration values 
 */
#define configMINIMAL_STACK_SIZE				0
#define configUSE_IDLE_HOOK						0
#define configUSE_TICK_HOOK						0
#define configUSE_QUEUE_SETS					0
#define configUSE_16_BIT_TICKS					0

 /* Some Cortex-M ports require this define for following macros which are not used by Whiteboard.
 *
 * portSET_INTERRUPT_MASK_FROM_ISR()
 * portCLEAR_INTERRUPT_MASK_FROM_ISR(x)
 * portDISABLE_INTERRUPTS()
 * portENABLE_INTERRUPTS()
 *
 * Here the value is defined to something that should be easily detectable if used. So don't use this,
 * but consult examples and documentation for proper value.
 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 0xACDCBEE5

/* Include only small subset of Windows if needed. Otherwise we get identifier conflicts when using non-Windows BSP. */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#endif /* FREERTOS_CONFIG_H */

