#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/builtinTypes/IDataTypeMetadataContainer.h"

namespace whiteboard
{

/** Interface for retrieving data for serialization */
class IStructureDataAccessor
{
public:
    /** Destructor. */
    virtual ~IStructureDataAccessor() {}

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(bool& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(int8& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(uint8& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(int16& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(uint16& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(int32& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(uint32& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(int64& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(uint64& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(float& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(double& rValue) = 0;

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    virtual bool getScalarValue(const char*& rValue) = 0;

    /** Called when structure serializer enters enumeration
    *
    * @param nameId Name ID of the enumeration type or ID_INVALID_STRING if enumeration is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterEnumeration(WB_RES::StringId nameId) = 0;

    /** Called when structure serializer exits enumeration
    *
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitEnumeration(WB_RES::StringId nameId) = 0;

    /** Called when structure serializer enters sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterStructure(WB_RES::StringId nameId) = 0;

    /** Called when structure serializer exits sub structure
    *
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitStructure(WB_RES::StringId nameId) = 0;

    /** Called when structure serializer enters property
    *
    * @param nameId Name ID of the property
    * @param required A value indicating whether the property is required
    * @param rHasValue On output contains a value indicating whether the optional has a value
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterProperty(WB_RES::StringId nameId, bool required, bool& rHasValue) = 0;

    /** Called when structure serializer exits property
    *
    * @param nameId Name ID of the property
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitProperty(WB_RES::StringId nameId) = 0;

    /** Called when structure serializer enters array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @param rNumberOfItems On output contains number of items in the array
    * @return A value indicating whether the data was retrieved
    */
    virtual bool enterArray(WB_RES::StringId nameId, uint16& rNumberOfItems) = 0;

    /** Called when structure serializer exits array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitArray(WB_RES::StringId nameId) = 0;

    /** Called when structure serializer enters new array item
    *
    * @param index Array item index
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterArrayItem(size_t index) = 0;

    /** Called when structure serializer exits array item
    *
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitArrayItem() = 0;
};

/** This class is used to serialize a whiteboard structure from a source data that is 
 * in another format e.g. JSON. */
class AlienStructureValueSerializer FINAL : public IValueSerializer
{
public:
    /** Default constructor */
    inline AlienStructureValueSerializer() {}

    /** Destructor */
    inline ~AlienStructureValueSerializer() {}

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
        const void* pData) const OVERRIDE FINAL;

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
        bool isReceiverDataType) const OVERRIDE FINAL;

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
        bool& rIsSenderDataType) const OVERRIDE FINAL;
};

/** Class for passing non-whiteboard data structure as whiteboard value */
class WB_API AlienStructure FINAL
{
public:
    /** Constructor
     *
     * @param rMetadataContainer Metadata container that can be used to retrieve information about given datatype
     * @param rDataAccessor Data accessor instance that can be used to retrieve data for the datatype
     * @param dataTypeId ID of the datatype
     */
    AlienStructure(
        const IDataTypeMetadataContainer& rMetadataContainer,
        IStructureDataAccessor& rDataAccessor,
        LocalDataTypeId dataTypeId);

    /** Destructor */
    ~AlienStructure();

    /** ID of the datatype */
    inline LocalDataTypeId getDataTypeId() const
    {
        return mDataTypeId;
    }

    /** Gets metadata container that can be used to retrieve information about given datatype */
    inline const IDataTypeMetadataContainer& getMetadataContainer() const
    {
        return mrMetadataContainer;
    }

    /** Gets data accessor instance that can be used to retrieve data for the datatype */
    inline IStructureDataAccessor& getDataAccessor() const
    {
        return mrDataAccessor;
    }

    /** Checks that the structure can be serialized with given information
    *
    * @return A value indicating whether the serialization can be done
    */
    bool validate();

private:
    /** Validates given data type befre serialization
    *
    * @param rDataType Metadata of the data type
    * @param nameId Name id of the current data type or ID_INVALID_STRING data type is anonynous
    * @return A value indicating whether the data type was successfully validated
    */
    bool validate(
        const WB_RES::DataTypeMetadata& rDataType,
        WB_RES::StringId nameId);

    /** Validates given array data type befre serialization
    *
    * @param rItemDataType Metadata of the array item
    * @param nameId Name id of the current data type or ID_INVALID_STRING data type is anonynous
    * @return A value indicating whether the data type was successfully validated
    */
    bool validateArray(
        const WB_RES::DataTypeMetadata& rItemDataType,
        WB_RES::StringId nameId);

    /** Validates given property type befre serialization
    *
    * @param rProperty Metadata of the property
    * @param rBaseDataType Metadata of the propertys data type
    * @return A value indicating whether the property was successfully validated
    */
    bool validateProperty(
        const WB_RES::StructurePropertyMetadata& rProperty,
        const WB_RES::DataTypeMetadata& rBaseDataType);

private:
    /** Metadata container that can be used to retrieve information about given datatype */
    const IDataTypeMetadataContainer& mrMetadataContainer;
    
    /** Data accessor instance that can be used to retrieve data for the datatype */
    IStructureDataAccessor& mrDataAccessor;

    /** ID of the datatype */
    LocalDataTypeId mDataTypeId;
};

} // namespace whiteboard
