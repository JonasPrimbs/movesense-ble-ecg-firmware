#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "wb-resources/resources.h"
#include "whiteboard/builtinTypes/IDataTypeMetadataContainer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Gets name of the enumeration value that matches given value
*
* @param value Enumeration value
* @param rDataType Enumeration data type meta data
* @return Name ID of the enumeration value or ID_INVALID_STRING if value is not valid enumeration value
*/
WB_RES::StringId getEnumerationValue(int32 value, const WB_RES::DataTypeMetadata& rDataType);

/** Calculates data type size and alignment
*
* @param rMetadataContainer Metadata container that can describe the structure type
* @param rDataType The data type
* @param rSize On output contains size of the data type
* @param rAlignment On output contains alignment of the data type
* @return A value indicating whether data type size and alignment was successfully calculated
*/
bool calculateDataTypeSizeAndAlignment(const IDataTypeMetadataContainer& rMetadataContainer,
    const WB_RES::DataTypeMetadata& rDataType,
    uint32& rSize,
    uint8& rAlignment);

} // namespace protocol_v9
} // namespace whitespace
