#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "shared.h"

/** Some hardcoded constants that ease the implementation,
* but should match real world
*/
const size_t ArraySize = 8;
const size_t ArrayAlignment = 4;
const size_t WrapperFor32BitPointerSize = 4;
const size_t WrapperFor32BitPointerAlignment = 4;

class DataType;

/** Calculates data type size
*
* @param pType Data type which size should be calculated
* @return Data type size
*/
size_t calculateDataTypeSize(const DataType* pType);

/** Calculates data type alignment
*
* @param pType Data type which alignment should be calculated
* @return Data type alignment
*/
size_t calculateDataTypeAlignment(const DataType* pType);

/** Calculates serialization length of data type metadata
*
* @param rDataTypes List of all data types (for metadata structure information)
* @param pType Data type which metadata serialization length should be calculated
* @return Data type metadata serialization length
*/
size_t calculateDataTypeMetadataSerializationLength(const std::vector<DataType*>& rDataTypes, const DataType* pType);
