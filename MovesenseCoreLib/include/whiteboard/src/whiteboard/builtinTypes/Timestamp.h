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
*	Timestamp
*	Range: -
*	Precision: 1us
*/
struct Timestamp
{
    /** Default constructor */
    inline Timestamp() : timestamp(0){};

    /** Constructor that initializes timestamp value from int64.
    *
    * @param timestamp	Timestamp value as int64.
    */
    inline Timestamp(const int64 timestamp_) : timestamp(timestamp_){};

    /** The timestamp value */
    WB_ALIGNED(int64) timestamp;

    // Structure type identification
    typedef int Structure;
    static const LocalDataTypeId DATA_TYPE_ID = 27;
};

} // namespace whiteboard
