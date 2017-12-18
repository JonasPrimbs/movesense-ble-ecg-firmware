#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/
#include "../integration/port.h"
#include "Structures.h"

namespace whiteboard
{

/**
*	Time (HH:MM:SS.us100)
*	Range: 00:00:00.0000 - 23:59:59.9999
*	Precision: 100us
*/
struct Time
{
    /** Default constructor. */
    inline Time() : hour(0), minute(0), second(0), us100(0){}

    /** Constructor that initializes all time fields.
    *
    * @param hour	Hours.
    * @param minute	Minutes.
    * @param second	Seconds.
    * @param us100	100 microseconds (step is 100us).
    */
    inline Time(const uint8 hour_, const uint8 minute_, const uint8 second_, const uint16 us100_)
        : hour(hour_), minute(minute_), second(second_), us100(us100_){}

    /** Checks if Time is valid. Function validates seconds using range that allows leap seconds.
    *
    * @return	True if Time is valid, otherwise false.
    */
    inline bool isValidTime() const
    {
        bool valid = true;
        if (hour > 23)
            valid = false;
        if (minute > 59)
            valid = false;
        if (hour == 23 && minute == 59)
        { // Leap second allowed.
            if (second > 60)
                valid = false;
        }
        else // No leap second allowed.
        {
            if (second > 59)
                valid = false;
        }
        if (us100 > 9999)
            valid = false;
        return valid;
    }

    WB_ALIGNED(uint8) hour;
    WB_ALIGNED(uint8) minute;
    WB_ALIGNED(uint8) second;
    WB_ALIGNED(uint16) us100;

    // Structure type identification
    typedef int Structure;
    static const LocalDataTypeId DATA_TYPE_ID = 26;
};

} // namespace whiteboard
