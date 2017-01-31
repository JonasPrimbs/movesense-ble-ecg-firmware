#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"

namespace whiteboard
{

/** Interface for classes that handles deserialization of unknown structures */
class IUnknownStructureDeserializer
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IUnknownStructureDeserializer() {}

public:
    /** Copies unknown structure to new dynamically allocated buffer.
    * Use commonMemPoolDeallocate to free the structure when it is no longer needed.
    * 
    * @param rStructure Structure that should be copied
    * @return Pointer to new copy of the structure
    */
    virtual UnknownStructure copy(const UnknownStructure& rStructure) const = 0;

    /** Deserializes the structure and calls tree visitor members while doing so.
    *
    * @param rStructure Structure that should be deserialized
    * @param rMetadataContainer Metadata container that can describe the structure type
    * @param metadataForSenderDataType A value indicating whether the dataTypeId and rMetadataContainer
    *        are for sender data types
    * @param rDataVisitor Structure visitor instance that should handle the data
    * @return A value indicating whether the structure was successfully deserialized
    */
    virtual bool deserialize(
        const UnknownStructure& rStructure,
        bool metadataForSenderDataType,
        const IDataTypeMetadataContainer& rMetadataContainer,
        IStructureDataVisitor& rDataVisitor) const = 0;
};

} // namespace whiteboard
