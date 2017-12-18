#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include <ctime>
#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/Date.h"
#include "whiteboard/builtinTypes/Time.h"
#include "whiteboard/builtinTypes/Structures.h"

namespace whiteboard
{

/**
*	DateTime (DD.MM.YYYY HH:MM:SS.us100 TimeZone)
*	Range: 1.1.1 00:00:00.0000 - 31.12.9999 23:59:59.9999
*	Precision: 100us
*	TimeZone: [-48, 56]; UTC-12 to UTC+14 with 15min resolution.
*/
struct DateTime
{
    /** Default constructor. */
    inline DateTime() : date(1, 1, 1), time(0, 0, 0, 0), timeZone(0){};

    /** Constructor that initializes date and time from references to Date and Time.
    *
    * @param rDate		Reference to Date.
    * @param rTime		Reference to Time.
    * @param rTimeZone	Time zone information in 15-min resolution (i.e. UTC+1 = 4).
    */
    inline DateTime(const Date& rDate, const Time& rTime, const int8 rTimeZone = 0)
        : date(rDate), time(rTime), timeZone(rTimeZone){};

    /**
    *	Constructor that initializes date and time from time_t structure. Assumes 1sec resolution.
    *	Uses UTC time and adds delta of given time zone. Subseconds are initialized to zero.
    *
    * @param rTime	Standard time_t structure.
    * @param rTimeZone	Time zone information in 15min resolution (i.e. UTC+1 = 4).
    */
    inline DateTime(const time_t& rTime, const int8 rTimeZone = 0) : timeZone(rTimeZone)
    {
        time_t tmpTime = rTime;
        // Add time zone to given timestamp.
        tmpTime += timeZone * 900; // 15min resolution (=900s)
                                   // Do conversion to UTC time format.
#if _MSC_VER
        struct tm timeInfo;
        gmtime_s(&timeInfo, &tmpTime);
#else
        struct tm timeInfo = *gmtime(&tmpTime);
#endif
        // date
        date.year = static_cast<uint16>(timeInfo.tm_year + 1900);
        date.month = static_cast<uint8>(timeInfo.tm_mon + 1);
        date.day = static_cast<uint8>(timeInfo.tm_mday);
        // time
        time.hour = static_cast<uint8>(timeInfo.tm_hour);
        time.minute = static_cast<uint8>(timeInfo.tm_min);
        time.second = static_cast<uint8>(timeInfo.tm_sec);
        time.us100 = 0;
    }

    /** Checks if Date and Time are valid.
    *
    * @return	True if date, time and timeZone are valid, otherwise false.
    */
    bool isValidDateTime() const
    {
        bool validTimeZone = (timeZone >= (-12 * 4) && timeZone <= (+14 * 4));
        return date.isValidDate() && time.isValidTime() && validTimeZone;
    }

    /** Date value */
    WB_ALIGNED(Date) date;

    /** Time value */
    WB_ALIGNED(Time) time;

    /** Time zone with 15-minute resolution ranging from UTC -12 to UTC +14; [-48, 56] */
    WB_ALIGNED(int8) timeZone;

    // Structure type identification
    typedef int Structure;
    static const LocalDataTypeId DATA_TYPE_ID = 25;
};

} // namespace whiteboard
