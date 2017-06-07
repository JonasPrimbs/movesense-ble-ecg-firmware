#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "wb-resources/resources.h"
#include "whiteboard/builtinTypes/IDataTypeMetadataContainer.h"

namespace whiteboard
{

// Forward declarations
class MetadataMap;

namespace protocol_v9
{

/** Gets name of the enumeration value that matches given value
*
* @param value Enumeration value
* @param rDataType Enumeration data type meta data
* @return Name ID of the enumeration value or ID_INVALID_STRING if value is not valid enumeration value
*/
WB_RES::StringId getEnumerationValue(int32 value, const WB_RES::DataTypeMetadata& rDataType);

/** Gets name of the enumeration value that matches given value
*
* @param value Enumeration value
* @param rItemList Enumeration item list to search
* @return Name ID of the enumeration value or ID_INVALID_STRING if value is not valid enumeration value
*/
metadata::StringId getEnumerationValue(int32 value, const metadata::EnumerationItemList& rItemList);

/** Calculates data type size and alignment
*
* @param rMetadataContainer Metadata container that can describe the structure type
* @param rDataType The data type
* @param rSize On output contains size of the data type
* @param rAlignment On output contains alignment of the data type
* @return A value indicating whether data type size and alignment was successfully calculated
*/
bool calculateDataTypeSizeAndAlignment(
    const IDataTypeMetadataContainer& rMetadataContainer,
    const WB_RES::DataTypeMetadata& rDataType,
    uint32& rSize,
    uint8& rAlignment);

/** Calculates data type size and alignment
*
* @param rMetadataMap Metadata map that can describe the structure type
* @param rDataType The data type
* @param rSize On output contains size of the data type
* @param rAlignment On output contains alignment of the data type
* @return A value indicating whether data type size and alignment was successfully calculated
*/
bool calculateDataTypeSizeAndAlignment(
    const MetadataMap& rMetadataMap,
    const metadata::DataType& rDataType,
    uint32& rSize,
    uint8& rAlignment);

} // namespace protocol_v9
} // namespace whiteboard
