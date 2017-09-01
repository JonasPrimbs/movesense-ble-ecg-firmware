#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "wb-resources/resources.h"
#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

// Forward declarations
class MetadataMap;

namespace services
{

/// Utility class for the metadata structures specified in the meta.yaml
class MetadataHelpers
{
private:
    // Prevent construction of this class
    MetadataHelpers();
    MetadataHelpers(const MetadataHelpers&);

public:
    /** Gets name of the enumeration value that matches given value
    *
    * @param value Enumeration value
    * @param rDataType Enumeration data type meta data
    * @return Name ID of the enumeration value or ID_INVALID_STRING if value is not valid enumeration value
    */
    static WB_RES::StringId getEnumerationValue(int32 value, const WB_RES::DataTypeMetadata& rDataType);

    /** Gets name of the enumeration value that matches given value
    *
    * @param value Enumeration value
    * @param rItemList Enumeration item list to search
    * @return Name ID of the enumeration value or ID_INVALID_STRING if value is not valid enumeration value
    */
    static metadata::StringId getEnumerationValue(int32 value, const metadata::EnumerationItemList& rItemList);

    // 6 operations, get,put,post,delete,subscribe,unsubscribe
    static const uint8 MAX_NUMBER_OF_OPERATIONS = ELEMENTS(metadata::OperationList::operationIds);

    /** Maximum number of enumeration values you can ask from MetadataProvider. There can be more values
    * that are used internally.
    */
    static const uint16 MAX_ENUM_VALUES = 64;

    /** Fills WB_RES::ResponseMetadata structure using internal metadata for a response
    *
    * @param responseId ID of the response
    * @param rResponse Internal metadata
    * @param rResponseMetadata On output contains metadata of response
    */
    static void fillResponseMetadata(
        WB_RES::LocalParameterId responseId,
        const metadata::Response& rResponse,
        WB_RES::ResponseMetadata& rResponseMetadata);

    /** Fills WB_RES::ParameterMetadata structure using internal metadata for a parameter
    *
    * @param parameterId ID of the parameter
    * @param rParameter Internal metadata
    * @param rParameterMetadata On output contains metadata of parameter
    */
    static void fillParameterMetadata(
        WB_RES::LocalParameterId parameterId,
        const metadata::Parameter& rParameter,
        WB_RES::ParameterMetadata& rParameterMetadata);

    /** Fills WB_RES::OperationMetadata structure using internal metadata for a operation
    *
    * @param rMetadata Metadata map that contains the metadata
    * @param operationId ID of the operation
    * @param rOperation Internal metadata
    * @param parameterIdList On output contains list of parameter IDs
    * @param responseIdList On output contains list of response IDs
    * @param rOperationMetadata On output contains metadata of the operation
    */
    static void fillOperationMetadata(
        const MetadataMap& rMetadata,
        WB_RES::LocalOperationId operationId,
        const metadata::Operation& rOperation,
        uint16 parameterIdList[WB_MAX_NUMBER_OF_PARAMETERS],
        uint16 responseIdList[WB_MAX_NUMBER_OF_RESPONSES],
        WB_RES::OperationMetadata& rOperationMetadata);

    /** Fills WB_RES::DataTypeMetadata structure using internal metadata for scalar data type
    *
    * @param dataTypeId ID of the data type
    * @param rDataType Internal metadata
    * @param rDataTypeMetadata On output contains metadata of scalar data type
    */
    static void fillDataTypeMetadataScalar(
        LocalDataTypeId dataTypeId,
        const metadata::DataType& rDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    /** Fills WB_RES::DataTypeMetadata structure using internal metadata for named data type
    *
    * @param dataTypeId ID of the data type
    * @param rDataType Internal metadata
    * @param rDataTypeMetadata On output contains metadata of named data type
    */
    static void fillDataTypeMetadataNamed(
        LocalDataTypeId dataTypeId,
        const metadata::DataType& rDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    /** Fills WB_RES::DataTypeMetadata structure using internal metadata for array data type
    *
    * @param dataTypeId ID of the data type
    * @param rDataType Internal metadata
    * @param rDataTypeMetadata On output contains metadata of array data type
    */
    static void fillDataTypeMetadataArray(
        LocalDataTypeId dataTypeId,
        const metadata::DataType& rDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    /** Fills WB_RES::StructurePropertyMetadata structure using internal metadata for a property
    *
    * @param rProperty Internal metadata
    * @param rStructurePropertyMetadata On output contains metadata of the property
    */
    static void fillProperty(
        const metadata::Property& rProperty,
        WB_RES::StructurePropertyMetadata& rStructurePropertyMetadata);

    /** Fills WB_RES::EnumerationItemMetadata structure using internal metadata for a enumeration item
    *
    * @param rEnumItem Internal metadata
    * @param rEnumItemMetadata On output contains metadata of the enumeration item
    */
    static void fillEnumItem(
        const metadata::EnumerationItem& rEnumItem,
        WB_RES::EnumerationItemMetadata& rEnumItemMetadata);

    /** Fills WB_RES::DataTypeMetadata structure using internal metadata for structure data type
    *
    * @param rMetadata Metadata map that contains the metadata
    * @param dataTypeId ID of the data type
    * @param rDataType Internal metadata
    * @param maxPropertyCount Maximum number of properties
    * @param array On output contains metadata of all structure properties
    * @param rDataTypeMetadata On output contains metadata of structure data type
    */
    static void fillDataTypeMetadataStructure(
        const MetadataMap& rMetadata,
        LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType& rDataType,
        size_t maxPropertyCount,
        WB_RES::StructurePropertyMetadata array[WB_MAX_NUMBER_OF_STRUCTURE_PROPERTIES],
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    /** Fills WB_RES::DataTypeMetadata structure using internal metadata for enumeration data type
    *
    * @param rMetadata Metadata map that contains the metadata
    * @param dataTypeId ID of the data type
    * @param rDataType Internal metadata
    * @param maxEnumItemCount Maximum number of enumeration items
    * @param array On output contains metadata of all enumeration items
    * @param rDataTypeMetadata On output contains metadata of structure data type
    */
    static void fillDataTypeMetadataEnum(
        const MetadataMap& rMetadata,
        LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType& rDataType,
        size_t maxEnumItemCount,
        WB_RES::EnumerationItemMetadata array[MAX_ENUM_VALUES],
        WB_RES::DataTypeMetadata& rDataTypeMetadata);
};

} // namespace services
} // namespace whiteboard
