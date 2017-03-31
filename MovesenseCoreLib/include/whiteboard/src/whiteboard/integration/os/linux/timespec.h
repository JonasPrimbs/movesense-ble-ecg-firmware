#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#ifdef ANDROID
#include <android/log.h>
#endif

#ifndef ANDROID /* LINUX */
#include <sys/time.h>
#include <signal.h>
#endif

/** Increases struct timespec using milliseconds*/
void increaseTimespec(struct timespec& tm, long milliseconds);