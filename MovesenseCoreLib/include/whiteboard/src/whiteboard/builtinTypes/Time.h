#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/
#include "../integration/port.h"
#include "Structures.h"

namespace whiteboard
{

WB_STRUCT_BITFIELD_PACK_BEGIN()

/**
*	Time (HH:MM:SS.us100)
*	Range: 00:00:00.0000 - 23:59:59.9999
*	Precision: 100us
*/
struct WB_STRUCT_BITFIELD_PACKED Time
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

#ifdef WB_STRUCTS_SUPPORT_BITFIELDS
    uint32 hour : 5;
    uint32 minute : 6;
    uint32 second : 6;
    uint32 us100 : 15;
#else
    WB_ALIGNED(uint8) hour;
    WB_ALIGNED(uint8) minute;
    WB_ALIGNED(uint8) second;
    WB_ALIGNED(uint16) us100;
#endif

    // Structure type identification and serialization
    typedef int Structure;
    static const LocalDataTypeId DATA_TYPE_ID = 26;
    WB_API static const StructureValueSerializer<Time> serializer;
    WB_WHEN_STRUCTURE_CLEANING_NEEDED(WB_API static const StructureValueCleaner<Time> cleaner;)

    // Visitor pattern implementation
    inline void visit(IStructureVisitor& rVisitor)
    {
        rVisitor.visit(hour).visit(minute).visit(second).visit(us100);
    }
};

WB_STRUCT_BITFIELD_PACK_END()

} // namespace whiteboard
