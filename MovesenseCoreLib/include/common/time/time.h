#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2016.
All rights reserved.
******************************************************************************/

#include <stdint.h>
#include <time.h>

/**
* This method shall be used instead of mktime(). Standard implementation is
* is not safe to call from FreeRTOS, because it is taking host OS level locks.
*/
time_t mktimep(struct tm* tm);

/**
* This method shall be used instead of localtime(). Standard implementation is
* not reentrant and is not safe to call from FreeRTOS, because it is taking
* host OS level locks.
*/
void localtimep(const time_t* time, struct tm* tm);

/**
* This method shall be used instead of gmtime(). Standard implementation is
* not reentrant.
*/
void gmtimep(const time_t* time, struct tm* tm);

/**
* Convert tm to time_t as UTC time.
*/
time_t timegm(struct tm* tm);
