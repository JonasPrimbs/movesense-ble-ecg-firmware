#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

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

    // 6 operations, get,put,post,delete,subscribe,unsubscribe
    
    static const uint8 MAX_NUMBER_OF_OPERATIONS = ELEMENTS(metadata::OperationList::operationIds);
    static const uint8 MAX_NUMBER_OF_RESPONSES = 32;

    static const uint8 MAX_STRUCTURE_PROPERTIES = WB_MAX_NUMBER_OF_STRUCTURE_PROPERTIES;
    static const uint16 MAX_ENUM_VALUES = 64;

    static void fillResponseMetadataConst(WB_RES::LocalParameterId responseId,
        const whiteboard::metadata::Response& response,
        WB_RES::ResponseMetadata& rResponseMetadata);

    static void fillParameterMetadataConst(WB_RES::LocalParameterId parameterId,
        const whiteboard::metadata::Parameter& parameter,
        WB_RES::ParameterMetadata& rParameterMetadata);

    static void fillOperationMetadataConst(
        const MetadataMap& rMetadata,
        WB_RES::LocalOperationId operationId,
        const whiteboard::metadata::Operation& operation,
        uint16 parameterIdList[WB_MAX_NUMBER_OF_PARAMETERS],
        uint16 responseIdList[MAX_NUMBER_OF_RESPONSES],
        WB_RES::OperationMetadata& rOperationMetadata);

    static void fillDataTypeMetadataScalar(whiteboard::LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType& rDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    static void fillDataTypeMetadataNamed(whiteboard::LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType& rDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    static void fillDataTypeMetadataArray(whiteboard::LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType& rDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    static void fillProperty(const whiteboard::metadata::Property& rProperty,
        WB_RES::StructurePropertyMetadata& rStructurePropertyMetadata);

    static void fillEnumItem(const whiteboard::metadata::EnumerationItem& rEnumItem,
        WB_RES::EnumerationItemMetadata& rEnumItemMetadata);

    static void fillDataTypeMetadataStructure(
        const MetadataMap& rMetadata,
        whiteboard::LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType* pDataType,
        size_t maxPropertyCount,
        WB_RES::StructurePropertyMetadata array[MAX_STRUCTURE_PROPERTIES],
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    static void fillDataTypeMetadataEnum(
        const MetadataMap& rMetadata,
        whiteboard::LocalDataTypeId dataTypeId,
        const whiteboard::metadata::DataType* pDataType,
        size_t maxEnumItemCount,
        WB_RES::EnumerationItemMetadata array[MAX_ENUM_VALUES],
        WB_RES::DataTypeMetadata& rDataTypeMetadata);
};

} // namespace services
} // namespace whiteboard
