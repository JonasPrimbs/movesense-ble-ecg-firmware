#pragma once
/****************************************************************************

        Copyright (c) Suunto Oy 2007.
        All rights reserved.

****************************************************************************/
/**
        @file daylightsavingAlg.h
        Routines for handling daylight saving

        @note
        Original implementation in Lola
*/

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"
#include "date.h"
#include "lTime.h"

namespace nea
{

/**
 * daylightsavingGetDSTDates return the start and the end date of the
 * daylight saving period.
 */
void daylightsavingGetDSTDates(int16 year, int8 rule, int8 dates[4]);

/**
 * daylightsavingGetDSTCorrection returns 0 or 1 depending on whether
 * daylight saving is off (0) or on (1) for a given date.
 */
int8 daylightsavingGetDSTCorrection(int16 year, int8 month, int8 day, int8 rule);

/**
 * daylightsavingGetDSTCorrection2 returns 0 or DST correction in minutes
 * depending on whether daylight saving is off or on for given UTC date and time.
 * Argument utcOffset must be standard offset without DST correction and in minutes.
 */
int8 daylightsavingGetDSTCorrection2(const Date* utcDate, const TimeHM* utcTime, int16 utcOffset, int8 rule);

/**
 * Returns DST correction value in minutes for given rule.
 */
int8 daylightsavingGetDSTMinutes(int8 rule);

/*
 * The day of the year is the order number of the current date from
 * 7.11.2006. E.g. daylightsavingDateToDayCount(2006,12,11) is 34.
 *
 */
int32 daylightsavingDateToDayCount(int16 year, int8 month, int8 day);

/**
 * The day of the year (doy) is the order number of the current date from
 * 7.11.2006. E.g. day_of_year(2006,12,11) is 34. The day of the week
 * is the order number of the day within week i.e. 1=Sunday,..., 7=Saturday.
 */
int8 daylightsavingGetWeekdayIndex(int32 doy);

/**
 * Leap year is every fourth year except every 100th year except
 * every 400th year which is leap year
 */
int8 daylightsavingIsLeapYear(int16 year);

/**
 * Returns DST transitions in standard time (DST correction excluded).
 * Example: Forward is at 1:00 AM local time and backward is at 2:00 AM with DST -> both are 1:00
 * standard time.
 *          Forward is at 0:00 AM local time and backward is at 0:00 AM with DST -> first is 0:00
 * and second 23:00 previous day standard time.
 */
void daylightsavingGetDSTTimes(int16 utcOffset, int8 rule, TimeHM times[2]);

/**
 * Returns date time combinations of DST transition times in the given year.
 * First is when clock is moved forwards.
 * Second is to where clock is moved forwards.
 * Third is to where clock is moved backwards.
 * Fourth is when clock is moved backwards.
 */
void daylightsavingsGetDSTLocalDateTimes(int16 year, int16 offset, int8 rule, Date dates[4], TimeHM times[4]);

} // namespace nea
