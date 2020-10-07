/*
    FreeRTOS V8.1.2 - Copyright (C) 2014 Real Time Engineers Ltd.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/******************************************************************************
    See http://www.freertos.org/a00110.html for an explanation of the
    definitions contained in this file.
******************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H


/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 * http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#include "platform/bsp/maxfreq.h"
#if defined(ARCH_KINETIS_K) || defined(ARCH_NRF52)
#include "buildconfig/bsp/config.h"
#endif

 // nRF5 SDK specific includes (needed for succesful linking of sd_app_wait_evt)
#ifdef ARCH_NRF52
#include "nrf_soc.h"
#endif

/* Constants related to the behaviour or the scheduler. */
#ifdef ARCH_NRF52
// Bug: https://devzone.nordicsemi.com/question/80413/freertos-tick-issue-on-nrf52-can-lead-to-lockups-sdk11/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	0
#define configUSE_TICKLESS_IDLE_SIMPLE_DEBUG    1
/*
     * Implementation note:
     *
     * To help debugging the option configUSE_TICKLESS_IDLE_SIMPLE_DEBUG was presented.
     * This option would make sure that even if program execution was stopped inside
     * this function no more than expected number of ticks would be skipped.
     *
     * Normally RTC works all the time even if firmware execution was stopped
     * and that may lead to skipping too much of ticks.
     */
#else
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1
#endif

#define configUSE_PREEMPTION					1
#define configUSE_TIME_SLICING					1
#define configMAX_PRIORITIES					( 5 )
#define configIDLE_SHOULD_YIELD					0
#define configUSE_16_BIT_TICKS					0 /* Only for 8 and 16-bit hardware. */

/* Despite the name this defines stack size of IDLE thread */
#if defined(CMAKE_NEA_HAVE_HEAP_TRACE)
// IDLE thread frees terminated threads, which needs more stack because tracing
#define configMINIMAL_STACK_SIZE ((uint16_t)130)
#elif defined(BUILD_BSP_REFLECT_ANALYSIS)
// callstack is deeper when reflect analysis is enabled
#define configMINIMAL_STACK_SIZE				( ( uint16_t ) 80 )
#elif defined(SOFTDEVICE_PRESENT)
// callstack is deeper using softdevice
#define configMINIMAL_STACK_SIZE				( ( uint16_t ) 100 )
#else
#define configMINIMAL_STACK_SIZE ((uint16_t)46)
#endif

#define configMAX_TASK_NAME_LEN (10)

#ifdef UNITTEST_KERNEL_HEAP_SIZE
#define configTOTAL_HEAP_SIZE ((size_t)(UNITTEST_KERNEL_HEAP_SIZE))
#else
#define configTOTAL_HEAP_SIZE ((size_t)(KERNEL_HEAP_SIZE))
#endif

/**
 * Tick/tickless configuration
 */
// Frequency of the scheduling tick when it's active
#define configTICK_RATE_HZ ((TickType_t)KERNEL_TICK_RATE_HZ)
/*
#if defined(BUILD_BSP_TICKLESS_AGGRESSIVE)
// Use custom tickless implementation (LPTMR based)
// Must define this since regular systick is still being compiled and needs this
#define configSYSTICK_CLOCK_HZ MAXFREQ_ACTUAL_HZ
#define configUSE_TICKLESS_IDLE 2
#define xPortSysTickHandler LPTMR_Systick
#elif defined(BUILD_BSP_TICKLESS_SYSTICK)
// Use upstream CM4F SysTick-based tickless implementation
#define configUSE_TICKLESS_IDLE 1
#define configCPU_CLOCK_HZ (MAXFREQ_ACTUAL_HZ)
#define xPortSysTickHandler SysTick_Handler
#else
// Tick is always running (from SysTick)
#define configUSE_TICKLESS_IDLE 0
#define configCPU_CLOCK_HZ (MAXFREQ_ACTUAL_HZ)
#define xPortSysTickHandler SysTick_Handler
#endif
*/
// Additional debugging facility for task switching using pinreflect
#ifdef ARCH_KINETIS_K
#if defined(__STDC__)
extern void ticklessTraceSwitch(uint8_t enabled);
#ifdef BUILD_BSP_ENABLE_WATCHDOG
extern void WD_feed(void);
#define traceTASK_SWITCHED_OUT()                                                                                                 \
    if (pxCurrentTCB == xTaskGetIdleTaskHandle()) WD_feed();                                                                     \
    const TCB_t* _preSwitchTCB = (const TCB_t*)pxCurrentTCB;                                                                     \
    ticklessTraceSwitch(1)
#else
#define traceTASK_SWITCHED_OUT()                                                                                                 \
    const TCB_t* _preSwitchTCB = (const TCB_t*)pxCurrentTCB;                                                                     \
    ticklessTraceSwitch(1)
#endif
#define traceTASK_SWITCHED_IN() ticklessTraceSwitch((_preSwitchTCB == (const TCB_t*)pxCurrentTCB) ? 0 : 2)
#endif // STDC
#endif // ARCH_KINETIS_K

#if defined(ARCH_NRF52) && defined(BUILD_BSP_ENABLE_WATCHDOG)
extern void WD_feed(void);
#define traceTASK_SWITCHED_OUT()                                                                                                 \
    if (pxCurrentTCB == xTaskGetIdleTaskHandle()) WD_feed();
#endif // BUILD_BSP_ENABLE_WATCHDOG && ARCH_NRF52

/* Constants that build features in or out. */
#define configUSE_MUTEXES 1
#define configUSE_APPLICATION_TASK_TAG 0
#define configUSE_NEWLIB_REENTRANT 0
#define configUSE_CO_ROUTINES 0
#define configMAX_CO_ROUTINE_PRIORITIES (2)

#define configUSE_COUNTING_SEMAPHORES 1
#define configUSE_RECURSIVE_MUTEXES 1
#define configUSE_QUEUE_SETS 0
#define configUSE_TASK_NOTIFICATIONS 1
#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 1

/* Constants that define which hook (callback) functions should be used. */
#define configUSE_IDLE_HOOK 1
#define configUSE_TICK_HOOK 0
#define configUSE_MALLOC_FAILED_HOOK 1

#define FREERTOS_USE_RTC      0 /**< Use real time clock for the system */
#define FREERTOS_USE_SYSTICK  1 /**< Use SysTick timer for system */


/* Constants provided for debugging and optimisation assistance. */
#define configCHECK_FOR_STACK_OVERFLOW 2

#ifdef CMAKE_NEA_HAVE_WAITOBJECT_DEBUGNAMES
/* All wait objects have names */
#define configQUEUE_REGISTRY_SIZE 100
#elif !defined(NDEBUG)
/* Only queues have names */
#define configQUEUE_REGISTRY_SIZE 8
#else
/* No names */
#define configQUEUE_REGISTRY_SIZE 0
#endif
#ifdef ARCH_NRF52
#define configTICK_SOURCE						FREERTOS_USE_RTC
#define configSYSTICK_CLOCK_HZ  ( 32768UL )
#define xPortSysTickHandler     RTC1_IRQHandler
#define configUSE_TICKLESS_IDLE 1
#else
#define configTICK_SOURCE						FREERTOS_USE_SYSTICK
#endif

// This file is included from assembler and C functions must be guarded
#if defined(__STDC__) || defined(_MSC_VER)


// Prefer to use TRACE_FILENAME if given by build system, default to __FILE__
#ifdef TRACE_FILENAME
    #define FREERTOS_ASSERT_FILE TRACE_FILENAME
#else
    #define FREERTOS_ASSERT_FILE __FILE__
#endif

void kernelAssert(const char* file, int line);
#define configASSERT(x)                                                                                                          \
    if ((x) == 0)                                                                                                                \
    {                                                                                                                            \
        kernelAssert(FREERTOS_ASSERT_FILE, __LINE__);                                                                            \
    }
#endif

/* Software timer definitions. */
#define configUSE_TIMERS 1
#define configTIMER_TASK_PRIORITY (4) // 'Tmr Svc' -thread priority should be higher than application
#define configTIMER_QUEUE_LENGTH 10

#ifndef CMAKE_NEA_HAVE_HEAP_TRACE
#define configTIMER_TASK_STACK_DEPTH (90)
#else
#define configTIMER_TASK_STACK_DEPTH (120)
#endif

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.  NOTE:  Setting an INCLUDE_ parameter to 0 is only
necessary if the linker does not automatically remove functions that are not
referenced anyway. */
#define INCLUDE_vTaskPrioritySet 1
#define INCLUDE_uxTaskPriorityGet 1
#define INCLUDE_vTaskDelete 1
#define INCLUDE_vTaskCleanUpResources 0
#define INCLUDE_vTaskSuspend 1
#define INCLUDE_vTaskDelayUntil 1
#define INCLUDE_vTaskDelay 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define INCLUDE_pcTaskGetTaskName 1
#define INCLUDE_xTaskGetIdleTaskHandle 1
#define INCLUDE_eTaskGetState 1
#define INCLUDE_xTaskResumeFromISR 1
#define INCLUDE_xTaskGetCurrentTaskHandle 1
#define INCLUDE_xTaskGetSchedulerState 0
#define INCLUDE_xSemaphoreGetMutexHolder 1
#define INCLUDE_xTimerPendFunctionCall 1
#define INCLUDE_xTimerGetTimerDaemonTaskHandle 1

#ifdef BUILD_BSP_THREAD_RUN_TIME_STATS_FULL

#define configGENERATE_RUN_TIME_STATS 1

/* This file is included also by assembler, but these functions are only for C/C++ */
#if defined(__STDC__) || defined(__cplusplus) || defined(_MSC_VER)

#ifdef __cplusplus
extern "C" {
#endif

// Runtime timer interface. These functions are implemented
// in the platform code. Actual location is implementation
// dependant, but rtc.cpp would be good place to look for.
void freertosInitialiseRunTimeStats(void);
unsigned long freertosGetRunTimeStatsCounterValue(void);

// Additional trace function to capture number of context switches
void threadRuntimeStatisticsContextSwitch(unsigned long threadNumber);

#ifdef __cplusplus
}
#endif
#endif

#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() freertosInitialiseRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE() freertosGetRunTimeStatsCounterValue()
#define traceTASK_SWITCHED_IN() threadRuntimeStatisticsContextSwitch((unsigned long)pxCurrentTCB->uxTCBNumber)
#endif

// No run time stats formatting
#define configUSE_STATS_FORMATTING_FUNCTIONS 0

/* Cortex-M specific definitions. */
#ifdef __NVIC_PRIO_BITS
/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
#define configPRIO_BITS __NVIC_PRIO_BITS
#else
#define configPRIO_BITS 4 /* 15 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#ifdef ARCH_NRF52
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         0xf
#else
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf
#endif

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#ifdef ARCH_NRF52
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	0x1
#else
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5
#endif

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#ifdef ARCH_NRF52
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY )
#else
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#endif

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#ifdef ARCH_NRF52
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY )
#else
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#endif


/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#if configUSE_TICKLESS_IDLE == 2
#define xPortSysTickHandler LPTMR_Systick
#else
    #ifdef ARCH_NRF52
        #define xPortSysTickHandler RTC1_IRQHandler
    #else
        #define xPortSysTickHandler SysTick_Handler
    #endif
#endif

#ifdef ARCH_SDL
#define portEND_SWITCHING_ISR(taskWoken) (void)0
#else
#define vPortYieldFromISR() portEND_SWITCHING_ISR(1)
#endif

/* Heap trace includes for C / C++ only (Assembler files include this header also) */
#if defined(__STDC__) || defined(__cplusplus) || defined(_MSC_VER)
#include "common/core/HeapTrace.h"
#endif

#endif /* FREERTOS_CONFIG_H */
