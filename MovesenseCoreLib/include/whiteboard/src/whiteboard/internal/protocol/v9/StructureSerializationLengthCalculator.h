#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/IStructureSerializationLengthCalculator.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that handles structure serialization
*/
class StructureSerializationLengthCalculator FINAL : public IStructureSerializationLengthCalculator
{
public:
    /** Destructor */
    virtual ~StructureSerializationLengthCalculator() {}

    /** Calculates serialization length for given object
    *
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param structureAlignment Alingment of the structure
    * @param structureSize Size of the structure
    * @param pData Poiner to structure data
    * @param rVisitorWrapper Wrapper instance that can be used to call structure visitor
    * @return Required serialization length in bytes.
    */
    size_t getSerializationLength(
        size_t bufferOffset,
        uint8 structureAlignment,
        size_t structureSize,
        const void* pData,
        const IStructureVisitorWrapper& rVisitorWrapper) const OVERRIDE;

    /** Calculates serialization length for given alien structure
    *
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param rAlienStructure Alien structure that should be serialized
    * @return Required serialization length in bytes.
    */
    size_t getSerializationLength(
        size_t bufferOffset,
        const AlienStructure& rAlienStructure) const OVERRIDE;

    /** Calculates serialization length for given unknown structure
    *
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param pData Structure data that should be serialized
    * @return Required serialization length in bytes.
    */
    size_t getUnknownStructureSerializationLength(
        size_t bufferOffset,
        const void* pData) const OVERRIDE;
};

} // namespace protocol_v9
} // namespace whiteboard
