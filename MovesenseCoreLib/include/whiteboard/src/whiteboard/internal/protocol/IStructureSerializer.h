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

/** Interface for structure serializers
 */
class IStructureSerializer
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IStructureSerializer() {}

public:
    /** Serializes object to given buffer
    *
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer in bytes
    * @param structureAlignment Alingment of the structure
    * @param structureSize Size of the structure
    * @param pData Pointer to structure data
    * @param isReceiverDataType A value indicating whether this message is in receiver's format
    * @param rVisitorWrapper Wrapper instance that can be used to call structure visitor
    * @return Length of the serialized data in bytes
    */
    virtual size_t serialize(
        void* pBuffer,
        size_t bufferLength,
        uint8 structureAlignment,
        size_t structureSize,
        const void* pData,
        bool isReceiverDataType,
        const IStructureVisitorWrapper& rVisitorWrapper) const = 0;

    /** Serializes alien data structure to given buffer
    *
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer in bytes
    * @param rAlienStructure Alien structure to serialize
    * @param isReceiverDataType A value indicating whether this message is in receiver's format
    * @return Length of the serialized data in bytes
    */
    virtual size_t serialize(
        void* pBuffer,
        size_t bufferLength,
        const AlienStructure& rAlienStructure,
        bool isReceiverDataType) const = 0;

    /** Serializes unknown data structure to given buffer
    *
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer in bytes
    * @param pData Structure data to serialize
    * @param isReceiverDataType A value indicating whether this message is in receiver's format
    * @return Length of the serialized data in bytes
    */
    virtual size_t serializeUnknownStructure(
        void* pBuffer,
        size_t bufferLength,
        const void* pData,
        bool isReceiverDataType) const = 0;
};

} // namespace whiteboard
