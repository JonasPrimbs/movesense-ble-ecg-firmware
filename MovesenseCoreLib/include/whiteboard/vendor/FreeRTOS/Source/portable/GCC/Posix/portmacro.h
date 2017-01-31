/*
	FreeRTOS.org V5.2.0 - Copyright (C) 2003-2009 Richard Barry.

	This file is part of the FreeRTOS.org distribution.

	FreeRTOS.org is free software; you can redistribute it and/or modify it
	under the terms of the GNU General Public License (version 2) as published
	by the Free Software Foundation and modified by the FreeRTOS exception.

	FreeRTOS.org is distributed in the hope that it will be useful,	but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
	more details.

	You should have received a copy of the GNU General Public License along
	with FreeRTOS.org; if not, write to the Free Software Foundation, Inc., 59
	Temple Place, Suite 330, Boston, MA  02111-1307  USA.

	A special exception to the GPL is included to allow you to distribute a
	combined work that includes FreeRTOS.org without being obliged to provide
	the source code for any proprietary components.  See the licensing section
	of http://www.FreeRTOS.org for full details.


	***************************************************************************
	*                                                                         *
	* Get the FreeRTOS eBook!  See http://www.FreeRTOS.org/Documentation      *
	*                                                                         *
	* This is a concise, step by step, 'hands on' guide that describes both   *
	* general multitasking concepts and FreeRTOS specifics. It presents and   *
	* explains numerous examples that are written using the FreeRTOS API.     *
	* Full source code for all the examples is provided in an accompanying    *
	* .zip file.                                                              *
	*                                                                         *
	***************************************************************************

	1 tab == 4 spaces!

	Please ensure to read the configuration and relevant port sections of the
	online documentation.

	http://www.FreeRTOS.org - Documentation, latest information, license and
	contact details.

	http://www.SafeRTOS.com - A version that is certified for use in safety
	critical systems.

	http://www.OpenRTOS.com - Commercial support, development, porting,
	licensing and training services.
*/


#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */
#define portCHAR		char
#define portFLOAT		float
#define portDOUBLE		double
#define portLONG		int
#define portSHORT		short
#define portSTACK_TYPE  size_t
#define portBASE_TYPE   long
    
#define portPOINTER_SIZE_TYPE size_t

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;
    
#if( configUSE_16_BIT_TICKS == 1 )
    typedef unsigned portSHORT TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xffff
#else
    typedef unsigned portLONG TickType_t;
    #define portMAX_DELAY ( TickType_t ) 0xffffffff
    
    /* 32-bit tick type on a 32/64-bit architecture, so reads of the tick count do
     not need to be guarded with a critical section. */
    #define portTICK_TYPE_IS_ATOMIC 1
#endif

/*-----------------------------------------------------------*/

/* Architecture specifics. */
#define portSTACK_GROWTH				( -1 )
#define portTICK_PERIOD_MS              ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portTICK_PERIOD_MICROSECONDS	( ( TickType_t ) 1000000 / configTICK_RATE_HZ )

#if defined(__x86_64__) || defined(_M_X64)
#define portBYTE_ALIGNMENT			8
#else
#define portBYTE_ALIGNMENT			4
#endif
/*-----------------------------------------------------------*/


/* Scheduler utilities. */
extern void vPortYieldFromISR( void );
extern void vPortYield( void );

#define portYIELD()					vPortYield()

/*-----------------------------------------------------------*/


/* Critical section management. */
extern void vPortDisableInterrupts( void );
extern void vPortEnableInterrupts( void );
#define portSET_INTERRUPT_MASK()	( vPortDisableInterrupts() )
#define portCLEAR_INTERRUPT_MASK()	( vPortEnableInterrupts() )

extern portBASE_TYPE xPortSetInterruptMask( void );
extern void vPortClearInterruptMask( portBASE_TYPE xMask );

#define portSET_INTERRUPT_MASK_FROM_ISR()		xPortSetInterruptMask()
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x)	vPortClearInterruptMask(x)


extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

#define portDISABLE_INTERRUPTS()	portSET_INTERRUPT_MASK()
#define portENABLE_INTERRUPTS()		portCLEAR_INTERRUPT_MASK()
#define portENTER_CRITICAL()		vPortEnterCritical()
#define portEXIT_CRITICAL()			vPortExitCritical()
/*-----------------------------------------------------------*/
	
	
#ifndef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1
#endif

/* 64-bit is not optimized */
#if portBYTE_ALIGNMENT == 8
	#undef configUSE_PORT_OPTIMISED_TASK_SELECTION
	#define configUSE_PORT_OPTIMISED_TASK_SELECTION 0
#endif
	
#if configUSE_PORT_OPTIMISED_TASK_SELECTION == 1
	
	/* Check the configuration. */
	#if( configMAX_PRIORITIES > 32 )
		#error configUSE_PORT_OPTIMISED_TASK_SELECTION can only be set to 1 when configMAX_PRIORITIES is less than or equal to 32.  It is very rare that a system requires more than 10 to 15 difference priorities as tasks that share a priority will time slice.
	#endif
	
	/* Store/clear the ready priorities in a bit map. */
	#define portRECORD_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) |= ( 1UL << ( uxPriority ) )
	#define portRESET_READY_PRIORITY( uxPriority, uxReadyPriorities ) ( uxReadyPriorities ) &= ~( 1UL << ( uxPriority ) )
	
	
	/*-----------------------------------------------------------*/
	
	#ifdef __clang__
		#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities )	  \
		__asm volatile(	"movl %0, %%eax									\n\t" \
						"bsrl %%eax, %%eax								\n\t" \
						"movl %%eax, %1									\n\t" \
			:"=r"(uxTopPriority) : "r"(uxReadyPriorities) : "eax" )
	#elif defined(__GNUC__)
		#define portGET_HIGHEST_PRIORITY( uxTopPriority, uxReadyPriorities )	  \
		__asm volatile(	"mov %0, %%eax									\n\t" \
						"bsr %%eax, %%eax								\n\t" \
						"mov %%eax, %1									\n\t" \
			:"=r"(uxTopPriority) : "r"(uxReadyPriorities) : "eax" )
	#else
		#error No definition for portGET_HIGHEST_PRIORITY macro
	#endif /* __GNUC__ */
	
#endif /* configUSE_PORT_OPTIMISED_TASK_SELECTION */
	
#ifndef __GNUC__
	__pragma( warning( disable:4211 ) ) /* Nonstandard extension used, as extern is only nonstandard to MSVC. */
#endif
	

/* Task function macros as described on the FreeRTOS.org WEB site. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

#define portNOP()

#define portOUTPUT_BYTE( a, b )

extern void vPortForciblyEndThread( void *pxTaskToDelete );
#define traceTASK_DELETE( pxTaskToDelete )		vPortForciblyEndThread( pxTaskToDelete )

extern void vPortAddTaskHandle( void *pxTaskHandle );
#define traceTASK_CREATE( pxNewTCB )			vPortAddTaskHandle( pxNewTCB )

/* Posix Signal definitions that can be changed or read as appropriate. */
#define SIG_SUSPEND					SIGUSR1
#define SIG_RESUME					SIGUSR2

/* Enable the following hash defines to make use of the real-time tick where time progresses at real-time. */
#define SIG_TICK					SIGALRM
#define TIMER_TYPE					ITIMER_REAL
/* Enable the following hash defines to make use of the process tick where time progresses only when the process is executing.
#define SIG_TICK					SIGVTALRM
#define TIMER_TYPE					ITIMER_VIRTUAL		*/
/* Enable the following hash defines to make use of the profile tick where time progresses when the process or system calls are executing.
#define SIG_TICK					SIGPROF
#define TIMER_TYPE					ITIMER_PROF */

/* Make use of times(man 2) to gather run-time statistics on the tasks. */
extern void vPortFindTicksPerSecond( void );
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()	vPortFindTicksPerSecond()		/* Nothing to do because the timer is already present. */
extern unsigned long ulPortGetTimerValue( void );
#define portGET_RUN_TIME_COUNTER_VALUE()			ulPortGetTimerValue()			/* Query the System time stats for this process. */

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

