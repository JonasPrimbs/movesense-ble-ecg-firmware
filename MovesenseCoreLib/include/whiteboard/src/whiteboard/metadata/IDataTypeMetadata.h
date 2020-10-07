#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "whiteboard/integration/port.h"
#include <whiteboard/metadata/MetadataStructures.h>

namespace whiteboard
{

/** Interface for data type metadata information containers */
#ifndef ARCH_HEXAGON
class IDataTypeMetadata : public IDynamicallyAllocatable
#else
/**
 * Hexagon toolchain has some issue where the compiler somehow ends up
 * with "dereference of null pointer is undefined" error. Not great to
 * bypass the WbMemAlloc but afaik there is currently no really use for it.
 */
class IDataTypeMetadata
#endif
{
public:
    /** Destructor */
    virtual ~IDataTypeMetadata() {}

    /** Gets string with given ID
    *
    * @param stringID ID of the string
    * @return Pointer to given string or NULL if the string was not found
    */
    virtual const char* getString(metadata::StringId stringId) const = 0;

    /** Returns a data type by ID
    *
    * @param dataTypeId ID of the data type
    * @return Object or NULL if invalid id is given
    */
    virtual const metadata::DataType* getDataType(LocalDataTypeId dataTypeId) const = 0;

    /** Returns base data type by ID
    *
    * @param rDataTypeId On input ID of the object. On output ID of the base data type.
    * @return Object or NULL if invalid id is given
    */
    const metadata::DataType* getBaseDataType(LocalDataTypeId& rDataTypeId) const;

    /** Returns base data type by ID. Does not traverse to base type of enumeration.
    *
    * @param rDataTypeId On input ID of the object. On output ID of the base data type.
    * @return Object or NULL if invalid id is given
    */
    const metadata::DataType* getEnumOrBaseDataType(LocalDataTypeId& rDataTypeId) const;

    /** Returns a property by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    virtual const metadata::Property* getProperty(metadata::PropertyId id) const = 0;

    /** Returns a property list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    virtual const metadata::PropertyList* getPropertyList(metadata::PropertyListId id) const = 0;

    /** Returns a enumeration item list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    virtual const metadata::EnumerationItemList getEnumerationItemList(metadata::EnumerationItemListId id) const = 0;

    /** Finds value of enumeration item
     *
     * @param dataTypeId ID of the data type
     * @param itemName name of the enumeration item
     * @param rValue On output contains value of the enumeration item
     * @return A value indicating whether the value was found
     */
    bool findEnumerationItemValueByName(const LocalDataTypeId dataTypeId, const char* itemName, int32& rValue) const;

    /** Finds name of enumeration item
    *
    * @param dataTypeId ID of the data type
    * @param value Value of the enumeration item
    * @return Name of the enumeration item or NULL if the value was found
    */
    const char* findEnumerationItemNameByValue(const LocalDataTypeId dataTypeId, int32 value) const;
};

} // namespace whiteboard
