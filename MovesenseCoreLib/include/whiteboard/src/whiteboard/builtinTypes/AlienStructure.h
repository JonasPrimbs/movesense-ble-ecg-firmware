#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/metadata/IDataTypeMetadata.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_ALIEN_STRUCTURES)

#ifdef WB_HAVE_ALIEN_STRUCTURES

namespace whiteboard
{

// Forward declarations
class EnumerationItemNameToValueMapper;

/** Interface for retrieving data for serialization */
class IStructureDataAccessor : public IDynamicallyAllocatable
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
    * @param rEnumerationItemValueMapper Object that can be used to find scalar values for enumeration item names. This
    *        instance is destructed only after corresponding call to exitEnumeration
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterEnumeration(metadata::StringId nameId, const EnumerationItemNameToValueMapper& rEnumerationItemValueMapper) = 0;

    /** Called when structure serializer exits enumeration
    *
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitEnumeration(metadata::StringId nameId) = 0;

    /** Called when structure serializer enters sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterStructure(metadata::StringId nameId) = 0;

    /** Called when structure serializer exits sub structure
    *
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitStructure(metadata::StringId nameId) = 0;

    /** Called when structure serializer enters property
    *
    * @param nameId Name ID of the property
    * @param required A value indicating whether the property is required
    * @param rHasValue On output contains a value indicating whether the optional has a value
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool enterProperty(metadata::StringId nameId, bool required, bool& rHasValue) = 0;

    /** Called when structure serializer exits property
    *
    * @param nameId Name ID of the property
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitProperty(metadata::StringId nameId) = 0;

    /** Called when structure serializer enters array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @param rNumberOfItems On output contains number of items in the array
    * @return A value indicating whether the data was retrieved
    */
    virtual bool enterArray(metadata::StringId nameId, uint16& rNumberOfItems) = 0;

    /** Called when structure serializer exits array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    virtual bool exitArray(metadata::StringId nameId) = 0;

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
        const IDataTypeMetadata& rMetadataContainer,
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
    inline const IDataTypeMetadata& getMetadataContainer() const
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
    * @param dataTypeId ID of the data type
    * @param rDataType Metadata of the data type
    * @param nameId Name id of the current data type or ID_INVALID_STRING data type is anonynous
    * @return A value indicating whether the data type was successfully validated
    */
    bool validate(
        LocalDataTypeId dataTypeId,
        const metadata::DataType& rDataType,
        metadata::StringId nameId);

    /** Validates given array data type befre serialization
    *
    * @param itemDataTypeId ID of the data type
    * @param rItemDataType Metadata of the array item
    * @param nameId Name id of the current data type or ID_INVALID_STRING data type is anonynous
    * @return A value indicating whether the data type was successfully validated
    */
    bool validateArray(
        LocalDataTypeId itemDataTypeId,
        const metadata::DataType& rItemDataType,
        metadata::StringId nameId);

    /** Validates given property type befre serialization
    *
    * @param rProperty Metadata of the property
    * @param baseDataTypeId ID of the data type
    * @param rBaseDataType Metadata of the propertys data type
    * @return A value indicating whether the property was successfully validated
    */
    bool validateProperty(
        const metadata::Property& rProperty,
        LocalDataTypeId baseDataTypeId,
        const metadata::DataType& rBaseDataType);

private:
    /** Metadata container that can be used to retrieve information about given datatype */
    const IDataTypeMetadata& mrMetadataContainer;
    
    /** Data accessor instance that can be used to retrieve data for the datatype */
    IStructureDataAccessor& mrDataAccessor;

    /** ID of the datatype */
    LocalDataTypeId mDataTypeId;
};

} // namespace whiteboard

#endif
