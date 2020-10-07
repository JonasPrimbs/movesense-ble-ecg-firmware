#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include <whiteboard/metadata/IDataTypeMetadata.h>

namespace whiteboard
{

/** Wrapper class for mapping enumeration item names to their values */
class EnumerationItemNameToValueMapper FINAL
{
public:
    /** Constructor
     * @param rMetadata Metadata container that can be used to retrieve information about given datatype
     * @param enumerationDataTypeId ID of the enumeration datatype
     */
    EnumerationItemNameToValueMapper(const IDataTypeMetadata& rMetadata, LocalDataTypeId enumerationDataTypeId)
        : mrMetadata(rMetadata), mEnumerationDataTypeId(enumerationDataTypeId)
    {}

    /** Destructor */
    virtual ~EnumerationItemNameToValueMapper()
    {}

    /** Finds value of enumeration item
     *
     * @param itemName name of the enumeration item
     * @param rValue On output contains value of the enumeration item
     * @return A value indicating whether the value was found
     */
    bool findEnumerationItemValueByName(const char* itemName, int32& rValue) const
    {
        return mrMetadata.findEnumerationItemValueByName(mEnumerationDataTypeId, itemName, rValue);
    }

private:
    /** Metadata container that can be used to retrieve information about given datatype */
    const IDataTypeMetadata& mrMetadata;

    /** ID of the enumeration datatype */
    LocalDataTypeId mEnumerationDataTypeId;
};

} // namespace whiteboard
