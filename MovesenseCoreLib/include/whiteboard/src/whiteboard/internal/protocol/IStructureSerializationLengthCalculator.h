#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"

namespace whiteboard
{

// Forward declarations
class AlienStructure;
class IStructureVisitorWrapper;

/** Interface for classes that handles space reservation for structure serilization */
class IStructureSerializationLengthCalculator
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IStructureSerializationLengthCalculator() {}

public:
    /** Calculates serialization length for given object
    *
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param structureAlignment Alingment of the structure
    * @param structureSize Size of the structure
    * @param pData Poiner to structure data
    * @param rVisitorWrapper Wrapper instance that can be used to call structure visitor
    * @return Required serialization length in bytes.
    */
    virtual size_t getSerializationLength(
        size_t bufferOffset,
        uint8 structureAlignment,
        size_t structureSize,
        const void* pData,
        const IStructureVisitorWrapper& rVisitorWrapper) const = 0;

    /** Calculates serialization length for given alien structure
    *
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param rAlienStructure Alien structure that should be serialized
    * @return Required serialization length in bytes.
    */
    virtual size_t getSerializationLength(
        size_t bufferOffset,
        const AlienStructure& rAlienStructure) const = 0;

    /** Calculates serialization length for given unknown structure
    *
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param pData Structure data that should be serialized
    * @return Required serialization length in bytes.
    */
    virtual size_t getUnknownStructureSerializationLength(
        size_t bufferOffset,
        const void* pData) const = 0;
};

} // namespace whiteboard
