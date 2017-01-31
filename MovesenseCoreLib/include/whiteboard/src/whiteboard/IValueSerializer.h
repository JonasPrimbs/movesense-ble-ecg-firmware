#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Identifiers.h"

namespace whiteboard
{

// Forward declarations
class IStructureSerializationLengthCalculator;
class IStructureSerializer;
class IStructureDeserializer;

/** Interface for value class serializer */
class WB_API IValueSerializer
{
public:
    /** Calculates serialization length of a structure
    *
    * @param rStructureSerializationLengthCalculator Structure serialization length calculator to use
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param pData Pointer to the actual object that should be serialized
    * @return Required serialization length in bytes.
    */
    virtual size_t getStructureSerializationLength(
        const IStructureSerializationLengthCalculator& rStructureSerializationLengthCalculator,
        size_t bufferOffset,
        const void* pData) const = 0;

    /** Serializes the structure
    *
    * @param rStructureSerializer Structure serializer to use
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer
    * @param pData Pointer to the actual object that should be serialized
    * @param isReceiverDataType Is the message in receiver format (true)
    * @return Length of the serialized data in bytes.
    */
    virtual size_t serializeStructure(
        const IStructureSerializer& rStructureSerializer,
        void* pBuffer,
        size_t bufferLength,
        const void* pData,
        bool isReceiverDataType) const = 0;

    /** Deserializes info from structure header
    *
    * @param pData Pointer to the actual object that should be deserialized
    * @param rStructureDeserializer Structure deserializer instance
    * @param rIsReceiverDataType On output contains a value that indicates whether this is data type with receiver's data type ID
    * @param rIsSenderDataType On output contains a value that indicates whether this is data type with sender's data type ID
    * @return true if deserialization succeeded, else false
    */
    virtual bool deserializeStructureHeader(
        const void* pData,
        const IStructureDeserializer& rStructureDeserializer,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const = 0;

    /** Deserializes a structure
    *
    * @param rStructureDeserializer Structure deserializer instance
    * @param pBuffer Pointer to the buffer that has serialized structure
    * @param rIsReceiverDataType On output contains a value that indicates whether this is data type with receiver's data type ID
    * @param rIsSenderDataType On output contains a value that indicates whether this is data type with sender's data type ID
    * @return  Pointer to deserialized structure within the buffer or NULL on failure
    */
    virtual const void* deserializeStructure(
        const IStructureDeserializer& rStructureDeserializer,
        void* pBuffer,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const = 0;

    /** Destructor */
    virtual ~IValueSerializer() {}
};

} // namespace whiteboard
