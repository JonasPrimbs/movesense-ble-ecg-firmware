#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2012.
All rights reserved.

******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef void* TimerHandle;
typedef void (*TimerCallback)(TimerHandle timer);

TimerHandle timerCreate(uint32_t millis, const char* pTimerName, bool repeat, TimerCallback callbackFunc, void* pUserData);
bool timerDelete(TimerHandle timerHandle);

bool timerIsActive(TimerHandle timerHandle);
bool timerStart(TimerHandle timerHandle);
bool timerStop(TimerHandle timerHandle);
bool timerReset(TimerHandle timerHandle);

bool timerStartISR(TimerHandle timerHandle);
bool timerStopISR(TimerHandle timerHandle);

bool timerSetPeriod(TimerHandle timerHandle, uint32_t millis);
bool timerSetPeriodISR(TimerHandle timerHandle, uint32_t millis);

void* timerGetUserData(TimerHandle timer);

#if defined(__cplusplus)
} // extern "C" {
#endif

#if __cplusplus

template <class T> T timerGetUserDataAs(TimerHandle timer)
{
    return static_cast<T>(timerGetUserData(timer));
}

#endif
