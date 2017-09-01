#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/metadata/IDataTypeMetadataContainer.h"

namespace whiteboard
{

/** Implementation of IDataTypeMetadataContainer interface for accessing local metadata
 *
 * @note This class uses dynamic memory allocation to handle structure properties and
 *       enumeration items.
 */
class WB_API LocalDataTypeMetadataContainer FINAL : public IDataTypeMetadataContainer
{
public:
    /** Default constructor */
    LocalDataTypeMetadataContainer();

    /** Destructor */
    virtual ~LocalDataTypeMetadataContainer();

    /** Gets string with given ID
    *
    * @param stringID ID of the string
    * @return Pointer to given string or NULL if the string was not found
    */
    const char* getString(const WB_RES::StringId stringId) const OVERRIDE;

    /** Releases all resources allocated for given string
    *
    * @param string String which resources should be released
    */
    void releaseString(const char* string) const OVERRIDE;

    /** Gets data type with given ID
    *
    * @param dataTypeId ID of the data type
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getDataType(const LocalDataTypeId dataTypeId, WB_RES::DataTypeMetadata& rMetadata) const OVERRIDE;

    /** Releases all resources allocated for given metadata structure
    *
    * @param rMetadata Metadata which resources should be released
    */
    void releaseDataType(WB_RES::DataTypeMetadata& rMetadata) const OVERRIDE;

private:
    /** Number of data types allocated. Used for internal book keeping */
    mutable size_t mDataTypeAllocCount;
};

} // namespace whiteboard
