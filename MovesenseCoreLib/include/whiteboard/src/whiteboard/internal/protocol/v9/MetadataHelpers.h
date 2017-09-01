#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "wb-resources/resources.h"
#include "whiteboard/metadata/IDataTypeMetadataContainer.h"

namespace whiteboard
{

// Forward declarations
class MetadataMap;

namespace protocol_v9
{

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
