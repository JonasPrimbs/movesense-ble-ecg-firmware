#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/IStructureDeserializer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Class that handles structure deserilization */
class StructureDeserializer FINAL : public IStructureDeserializer
{
public:
    /** Destructor */
    virtual ~StructureDeserializer() {}

    /** Deserializes info from structure header
    *
    * @param pData Pointer to the actual object that should be deserialized
    * @param rIsReceiverDataType On output contains a value that indicates whether this is data type with receiver's data type ID
    * @param rIsSenderDataType On output contains a value that indicates whether this is data type with sender's data type ID
    * @return true if deserialization succeeded, else false
    */
    virtual bool deserializeHeader(
        const void* pData,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const OVERRIDE;

    /** Deserializes object from the given buffer
    *
    * @param pBuffer Buffer where the the object should be deserialized from
    * @param structureAlignment Alingment of the structure
    * @param structureSize Size of the structure
    * @param rIsReceiverDataType On output contains a value that indicates whether this is data type with receiver's data type ID
    * @param rIsSenderDataType On output contains a value that indicates whether this is data type with sender's data type ID
    * @param rVisitorWrapper Wrapper instance that can be used to call structure visitor
    * @return Pointer to deserialized structure within the buffer or NULL on failure
    */
    const void* deserialize(
        void* pData,
        uint8 structureAlignment,
        size_t structureSize,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType,
        const IStructureVisitorWrapper& rVisitorWrapper) const OVERRIDE;
};

} // namespace protocol_v9
} // namespace whiteboard
