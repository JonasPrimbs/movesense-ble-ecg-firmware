#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/metadata/IDataTypeMetadataContainer.h"

namespace whiteboard
{

// Forward declarations
class UnknownStructureAccessor;
class MetadataMap;

/** Interface of visitors that walk structure contents */
class IStructureDataVisitor : public IDynamicallyAllocatable
{
public:
    /** Destructor. */
    virtual ~IStructureDataVisitor() {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(bool /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int8 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int16 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int32 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int64 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint8 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint16 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint32 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint64 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(float /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(double /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(const char* /*data*/) {}

    /** Called when structure deserializer enters enumeration
    *
    * @param nameId Name ID of the enumeration type or ID_INVALID_STRING if enumeration is anonymous
    */
    virtual void enterEnumeration(WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer exits enumeration
    *
    * @param nameId Name ID of the enumeration type or ID_INVALID_STRING if enumeration is anonymous
    */
    virtual void exitEnumeration(WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int8 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int16 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int32 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int64 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(uint8 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(uint16 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(uint32 /*data*/, WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer enters sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    */
    virtual void enterStructure(WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer exits sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    */
    virtual void exitStructure(WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer enters property
    *
    * @param nameId Name ID of the property
    * @param hasValue A value indicating whether the property has a value
    */
    virtual void enterProperty(WB_RES::StringId /*nameId*/, bool /*hasValue*/) {}

    /** Called when structure deserializer exits property
    *
    * @param nameId Name ID of the property
    */
    virtual void exitProperty(WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer enters array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @param numberOfItems Number of items in the array
    */
    virtual void enterArray(WB_RES::StringId /*nameId*/, size_t /*numberOfItems*/) {}

    /** Called when structure deserializer exits array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    */
    virtual void exitArray(WB_RES::StringId /*nameId*/) {}

    /** Called when structure deserializer enters new array item
    */
    virtual void enterArrayItem() {}

    /** Called when structure deserializer exits array item
    */
    virtual void exitArrayItem() {}
};

/** This class is used to re-serialize received unknown structure, for example to be passed forward. */
class UnknownStructureValueSerializer FINAL : public IValueSerializer
{
public:
    /** Default constructor */
    inline UnknownStructureValueSerializer() {}

    /** Destructor */
    inline ~UnknownStructureValueSerializer() {}

    /** Calculates serialization length of a structure
    *
    * @param rStructureSerializationLengthCalculator Structure serialization length calculator to use
    * @param bufferOffset Buffer offset for serialization start. Needed for alignment calculations.
    * @param pData Pointer to the actual object that should be serialized
    * @return Required serialization length in bytes.
    */
    size_t getStructureSerializationLength(
        const IStructureSerializationLengthCalculator& rStructureSerializationLengthCalculator,
        size_t bufferOffset,
        const void* pData) const OVERRIDE;

    /** Serializes the structure
    *
    * @param rStructureSerializer Structure serializer to use
    * @param pBuffer Destination buffer
    * @param bufferLength Length of the buffer
    * @param pData Pointer to the actual object that should be serialized
    * @param isReceiverDataType Is the message in receiver format (true)
    * @return Length of the serialized data in bytes.
    */
    size_t serializeStructure(
        const IStructureSerializer& rStructureSerializer,
        void* pBuffer,
        size_t bufferLength,
        const void* pData,
        bool isReceiverDataType) const OVERRIDE;

    /// @see whiteboard::IValueSerializer::deserializeStructureHeader
    bool deserializeStructureHeader(
        const void* pData,
        const IStructureDeserializer& rStructureDeserializer,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const OVERRIDE;

    /** Deserializes a structure
    *
    * @param rStructureDeserializer Structure deserializer instance
    * @param pBuffer Pointer to the buffer that has serialized structure
    * @param rIsReceiverDataType On output contains a value that indicates whether this is data type with receiver's data type ID
    * @param rIsSenderDataType On output contains a value that indicates whether this is data type with sender's data type ID
    * @return  Pointer to deserialized structure within the buffer or NULL on failure
    */
    const void* deserializeStructure(
        const IStructureDeserializer& rStructureDeserializer,
        void* pBuffer,
        bool& rIsReceiverDataType,
        bool& rIsSenderDataType) const OVERRIDE;
};

/** Class that represents unknown structure. Receiver can always convert request parameters and
* return values to UnknownStructure instance and then walk the structure contents in a more dynamic
* way. */
class WB_API UnknownStructure FINAL
{
public:
    /** Default constructor */
    UnknownStructure();

    /** Copy constructor.
     *
     * @note UnknownStructure is behaving like unique_ptr. Memory allocated for UnknownStructure
     *       is deallocated when object with ownership of the memory is deallocated (leaves scope).
     *       Ownership of the object is transferred with copy constructor and assignment operator.
     *       This also invalidates data pointer of previous owner.
     *
     * @param rOther UnknownStructure which ownership should be transferred
     */
    UnknownStructure(const UnknownStructure& rOther);
    UnknownStructure& operator=(const UnknownStructure& rOther);

    /** Destructor */
    ~UnknownStructure();

    /** Checks whether the structure contains valid information. */
    inline bool isValid() const
    {
        return mDataTypeId != WB_TYPE_NONE;
    }

    /** ID of the datatype. */
    inline LocalDataTypeId getDataTypeId() const
    {
        return mDataTypeId;
    }

    /** Gets protocol version used to encode the structure */
    inline ProtocolVersion getProtocolVersion() const
    {
        return mProtocolVersion;
    }

    /** Deserializes the structure and calls tree visitor members while doing so.
    *
    * @param metadataForSenderDataType A value indicating whether the dataTypeId and rMetadataContainer
    *        are for sender data types
    * @param rMetadataContainer Metadata container that can describe the structure type
    * @param rDataVisitor Structure visitor instance that should handle the data
    * @return A value indicating whether the structure was successfully deserialized
    */
    bool deserialize(
        bool metadataForSenderDataType,
        const IDataTypeMetadataContainer& rMetadataContainer,
        IStructureDataVisitor& rTreeVisitor) const;

    /** Deserializes the structure and calls tree visitor members while doing so.
    *
    * @param metadataForSenderDataType A value indicating whether the dataTypeId and rMetadataContainer
    *        are for sender data types
    * @param rMetadataMap Metadata map that can describe the structure type
    * @param rDataVisitor Structure visitor instance that should handle the data
    * @return A value indicating whether the structure was successfully deserialized
    */
    bool deserialize(
        bool metadataForSenderDataType,
        const MetadataMap& rMetadataMap,
        IStructureDataVisitor& rTreeVisitor) const;

    /** Copies unknown structure to a new instance. Both of the instances
     *  will have their own copy of the data.
     **/
    UnknownStructure copy() const;

private:
    /** Constructor 
     *
     * @param dataTypeId ID of the data type
     * @param protocolVersion Protocol version that was used to encode the structure
     * @param pData Encoded data
     * @param dataOwner Ownership of the data
     */
    UnknownStructure(LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData, bool dataOwner);

private:
    /** Library internal implementation can access these members */
    friend class UnknownStructureAccessor;

    /** Data type of the value */
    LocalDataTypeId mDataTypeId;

    /** Protocol version */
    ProtocolVersion mProtocolVersion;

    /** The data */
    mutable const void* mpData;

    /** Ownership of the data */
    mutable bool mDataOwner;
};

} // namespace whiteboard
