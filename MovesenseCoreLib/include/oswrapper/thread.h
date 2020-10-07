#pragma once
#include "common/compiler/genType.h"
#include "common/core/HeapTrace.h"

#if defined(__cplusplus)
extern "C" {
#endif

enum ThreadPriority
{
    FW_THREAD_PRIORITY_IDLE,   // Reserved to the FreeRTOS
    FW_THREAD_PRIORITY_LOW,    // No usage limits
    FW_THREAD_PRIORITY_NORMAL, // Maximum time usage < 100 ms
    FW_THREAD_PRIORITY_DEFAULT = FW_THREAD_PRIORITY_NORMAL,
    FW_THREAD_PRIORITY_HIGH,    // Maximum time usage < 5 ms
    FW_THREAD_PRIORITY_CRITICAL // Maximum time usage < 1 ms (FreeRTOS timer)
};

typedef void* ThreadHandle;
typedef void (*ThreadFunction)(void* pUserData);

/*thread creation and deletion; ATTENTION: If you call threadCreate in multiple threads, the following methods should not be
called:
- threadGetCount
- threadGetMaxStackDepth
- threadGetAllThreadHandles
As the data returned there is not protected by mutex and the data is updated in threadCreate and threadDelete
*/
ThreadHandle threadCreate(ThreadFunction threadFunction,
                          const char* pThreadName,
                          uint16_t stackDepth,
                          void* pUserData,
                          enum ThreadPriority priority HEAP_TRACE_PARAMS_DECL);

void threadDelete(ThreadHandle threadHandle);

// controlling functions
void threadSuspend(ThreadHandle threadHandle);
void threadResume(ThreadHandle threadHandle);
void threadResumeISR(ThreadHandle threadHandle);
void threadSuspendAll();
void threadResumeAll();

/** Gets handle of current thread. Note that this function will return valid values only for
 * when run in OS thread. For any pre OS thread including main program entry point this function
 * may return what ever garbage and should not be trusted even for two consecutive calls.
 *
 * @return Handle to current OS thread
 */
ThreadHandle threadGetCurrentThreadHandle(void);

/** Gets handle of current thread the safe way. This function will return NULL for all calls
* until scheduler has started which should cover all pre OS calls. Note that this adds overhead
* to the call and its use should be avoided when code is executed in OS thread.
*
* @return Handle to current OS thread
*/
ThreadHandle threadGetCurrentThreadHandleSafe(void);

enum ThreadPriority threadGetPriority(ThreadHandle threadHandle);
void threadSetPriority(ThreadHandle threadHandle, enum ThreadPriority priority);

// query functions
const char* threadGetName(ThreadHandle threadHandle);
uint32_t threadGetStackHighWaterMark(ThreadHandle threadHandle);
// Extra query functions, see note in createThread!
uint8_t threadGetCount(void);
uint16_t threadGetMaxStackDepth(ThreadHandle threadHandle);
const ThreadHandle*
threadGetAllThreadHandles(uint8_t* numThreads); // This excludes OS internal threads, such as possible timer-thread.

// critical sections
void threadEnterCritical(void);
void threadExitCritical(void);

// delay
void threadDelay(uint32_t millis);
void threadDelayExact(uint32_t* previousWakeTimeMillis, uint32_t absWakeTimeMillis);

// misc
void threadYield(void);

// Thread local storage
void* threadGetLocalStoragePointer(ThreadHandle threadHandle, int index);
void threadSetLocalStoragePointer(ThreadHandle threadHandle, int index, void* pValue);

#ifdef CMAKE_NEA_HAVE_HEAP_TRACE

#define threadCreate(threadFunction, pThreadName, stackDepth, pUserData, priority)                                               \
    threadCreate(threadFunction, pThreadName, stackDepth, pUserData, priority, HEAP_TRACE_ENTER("threadCreate"))

#endif

#ifdef BUILD_BSP_THREAD_RUN_TIME_STATS

struct ThreadStatistics
{
    const char* name;
    uint8 state;
    uint8 basePriority;
    uint8 currentPriority;
    uint8 runTimePercentage;
    uint32_t runTimeTicks;
    uint32_t contextSwitches;
    uint32_t freeStack;
    size_t programCounter;
    size_t returnAddress;
};

enum ThreadState
{
    OS_THREAD_READY = 0U,
    OS_THREAD_RUNNING = 1U,
    OS_THREAD_BLOCKED = 2U,
    OS_THREAD_SUSPENDED = 3U,
    OS_THREAD_TERMINATED = 4U
};

size_t threadGetRuntimeStatistics(struct ThreadStatistics* pThreadStatistics, size_t maxCount);

#endif

#if defined(__cplusplus)
} // extern "C" {
#endif
