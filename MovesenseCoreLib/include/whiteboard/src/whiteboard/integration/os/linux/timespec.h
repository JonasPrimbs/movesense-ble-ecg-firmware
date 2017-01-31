#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#ifdef ANDROID
#include <android/log.h>

/* Macros for converting between `struct timeval' and `struct timespec'.  */
#ifndef TIMEVAL_TO_TIMESPEC
#define TIMEVAL_TO_TIMESPEC(tv, ts)                                                                                              \
    {                                                                                                                            \
        (ts)->tv_sec = (tv)->tv_sec;                                                                                             \
        (ts)->tv_nsec = (tv)->tv_usec * 1000;                                                                                    \
    \
}
#endif

#ifndef TIMESPEC_TO_TIMEVAL
#define TIMESPEC_TO_TIMEVAL(tv, ts)                                                                                              \
    {                                                                                                                            \
        (tv)->tv_sec = (ts)->tv_sec;                                                                                             \
        (tv)->tv_usec = (ts)->tv_nsec / 1000;                                                                                    \
    \
}
#endif
#endif

#ifndef ANDROID /* LINUX */
#include <sys/time.h>
#include <signal.h>
#endif

/** Increases struct timespec using milliseconds*/
void increaseTimespec(struct timespec& tm, long milliseconds);