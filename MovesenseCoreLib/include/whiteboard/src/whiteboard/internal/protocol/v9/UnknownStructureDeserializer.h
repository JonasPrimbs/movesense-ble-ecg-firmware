#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/internal/protocol/IUnknownStructureDeserializer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that handles deserialization of unknown structures */
class UnknownStructureDeserializer FINAL : public IUnknownStructureDeserializer
{
public:
    /** Constructor
    *
    * @param pBuffer Owning buffer instance
    * @param bufferLength Length of the buffer
    * @param padding before the structure in the buffer to align it properly
    */
    inline UnknownStructureDeserializer() {}

    /** Destructor */
    virtual ~UnknownStructureDeserializer() {}

    /** Copies unknown structure to new dynamically allocated buffer.
    * Use commonMemPoolDeallocate to free the structure when it is no longer needed.
    * 
    * @param rStructure Structure that should be copied
    * @return Pointer to new copy of the structure
    */
    UnknownStructure copy(const UnknownStructure& rStructure) const OVERRIDE;

    /** Deserializes the structure and calls tree visitor members while doing so.
    *
    * @param rStructure Structure that should be deserialized
    * @param metadataForSenderDataType A value indicating whether the dataTypeId and rMetadataContainer
    *        are for sender data types
    * @param rMetadataContainer Metadata container that can describe the structure type
    * @param rDataVisitor Structure visitor instance that should handle the data
    * @return A value indicating whether the structure was successfully deserialized
    */
    bool deserialize(
        const UnknownStructure& rStructure,
        bool metadataForSenderDataType,
        const IDataTypeMetadataContainer& rMetadataContainer,
        IStructureDataVisitor& rDataVisitor) const OVERRIDE;

    /** Deserializes the structure and calls tree visitor members while doing so.
    *
    * @param rStructure Structure that should be deserialized
    * @param metadataForSenderDataType A value indicating whether the dataTypeId and rMetadataContainer
    *        are for sender data types
    * @param rMetadataMap Metadata map that can describe the structure type
    * @param rDataVisitor Structure visitor instance that should handle the data
    * @return A value indicating whether the structure was successfully deserialized
    */
    bool deserialize(
        const UnknownStructure& rStructure,
        bool metadataForSenderDataType,
        const MetadataMap& rMetadataContainer,
        IStructureDataVisitor& rDataVisitor) const OVERRIDE;
};

} // protocol_v9
} // namespace whiteboard
