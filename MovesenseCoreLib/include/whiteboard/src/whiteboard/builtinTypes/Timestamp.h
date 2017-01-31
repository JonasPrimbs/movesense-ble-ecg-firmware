#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/
#include "../integration/port.h"
#include "Structures.h"

namespace whiteboard
{

WB_STRUCT_PACK_BEGIN()

/**
*	Timestamp
*	Range: -
*	Precision: 1us
*/
struct WB_STRUCT_PACKED Timestamp
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

    // Structure type identification and serialization
    typedef int Structure;
    static const LocalDataTypeId DATA_TYPE_ID = 27;
    static const StructureValueSerializer<Timestamp> serializer;
    WB_WHEN_STRUCTURE_CLEANING_NEEDED(static const StructureValueCleaner<Timestamp> cleaner;)

    // Visitor pattern implementation
    inline void visit(IStructureVisitor& rVisitor) { rVisitor.visit(timestamp); }
};

WB_STRUCT_PACK_END()

} // namespace whiteboard
