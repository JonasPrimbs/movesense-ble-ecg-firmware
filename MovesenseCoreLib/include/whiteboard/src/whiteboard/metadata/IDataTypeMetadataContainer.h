#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "wb-resources/resources.h"

namespace whiteboard
{

/** Interface for data type metadata information containers */
class IDataTypeMetadataContainer : public IDynamicallyAllocatable
{
public:
    /** Destructor */
    virtual ~IDataTypeMetadataContainer() {}

    /** Gets string with given ID
    *
    * @param stringID ID of the string
    * @return Pointer to given string or NULL if the string was not found
    */
    virtual const char* getString(const WB_RES::StringId stringId) const = 0;

    /** Releases all resources allocated for given string
    *
    * @param string String which resources should be released
    */
    virtual void releaseString(const char* string) const = 0;

    /** Gets data type with given ID
    *
    * @param dataTypeId ID of the data type
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    virtual bool getDataType(const LocalDataTypeId dataTypeId, WB_RES::DataTypeMetadata& rMetadata) const = 0;

    /** Releases all resources allocated for given metadata structure
    *
    * @param rMetadata Metadata which resources should be released
    */
    virtual void releaseDataType(WB_RES::DataTypeMetadata& rMetadata) const = 0;
};

} // namespace whiteboard
