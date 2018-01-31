#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

struct WbTimer; // Forward declaration hides the implementation
typedef WbTimer* WbTimerHandle;
#define WB_INVALID_TIMER NULL

// Timer callback signature
typedef void (*WbTimerCallback)(WbTimerHandle);

/** Creates a timer that calls given callback on desires interval. Timer is not running after creation, 
startTimer needs to be called in order to do that.

@param periodMs Period of the timer in milliseconds.
@param isContinuous true if the timer will call callback periodically until it has been stopped.
@param callback Function to call when the period expires.
@param pUserData Pointer to user data that can be associated with the created timer handle. Note data is not copied,
only a pointer to the data is stored.
*/
WB_API WbTimerHandle WbTimerCreate(uint32 periodMs, bool isContinuous, WbTimerCallback callback, void* pUserData);

/** Deletes timer and frees it's resources.

@param timerHandle Timer to delete.
*/
WB_API bool WbTimerDelete(WbTimerHandle timerHandle);

/** Starts a timer.

@param timerHandle Timer to start.
@return true if the timer was started, else false.
*/
WB_API bool WbTimerStart(WbTimerHandle timerHandle);

/** Changes timer period. If the timer is running it is stopped, then rescheduled and 
restarted.

@param timerHandle Timer to reschedule
@param periodMs New timer period in [ms]
@return true if rescheduling succeeded, else false.
*/
WB_API bool WbTimerReschedule(WbTimerHandle timerHandle, uint32 periodMs);

/** Stops a timer.

@param timerHandle Timer to stop.
@return true if the timer was running and was stopped, else false.
*/
WB_API bool WbTimerStop(WbTimerHandle timerHandle);

/** Retrieves the user-data associated with the given timer.

@param timerHandle Timer of which user data is to be fetched.
@return pointer to the user data.
*/
WB_API void* WbTimerGetUserData(WbTimerHandle timerHandle);

#ifdef MIM_HAVE_INTERRUPT_API

/** Starts a timer.
*
* @note This function should be called only from interrupt service routines
*
* @param timerHandle Timer to start.
* @return true if the timer was started, else false.
*/
WB_API bool WbTimerStartISR(WbTimerHandle timerHandle);

/** Stops a timer.
*
* @note This function should be called only from interrupt service routines
*
* @param timerHandle Timer to stop.
* @return true if the timer was running and was stopped, else false.
*/
WB_API bool WbTimerStopISR(WbTimerHandle timerHandle);

/** Changes timer period. If the timer is running it is stopped, then rescheduled and
* restarted.
*
* @note This function should be called only from interrupt service routines
*
* @param timerHandle Timer to reschedule
* @param periodMs New timer period in [ms]
* @return true if rescheduling succeeded, else false.
*/
WB_API bool WbTimerRescheduleISR(WbTimerHandle timerHandle, uint32 periodMs);

#endif

#ifdef WB_HAVE_HEAP_TRACE

#ifdef __cplusplus
extern "C" {
#endif

WB_HEAP_TRACE_DECLARE_WRAPPER(WbTimerHandle, WbTimerCreate, uint32, bool, WbTimerCallback, void*);
#define WbTimerCreate(periodMs, isContinuous, callback, pUserData) \
    WB_HEAP_TRACE_WRAPPER(WbTimerCreate, periodMs, isContinuous, callback, pUserData)

#ifdef __cplusplus
}
#endif

#endif // WB_HAVE_HEAP_TRACE

