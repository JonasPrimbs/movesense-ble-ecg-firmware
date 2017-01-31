#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

/** Map of read-only resource metadata */
struct MetadataMap
{
    /** Number of strings */
    size_t mStringMapLength;

    /** String map (id is offset of a string in the map) */
    const char* mpStringMap;

    /** Number of properties */
    size_t mNumberOfProperties;

    /** List of properties */
    const metadata::Property* mpProperties;

    /** Number of property lists */
    size_t mNumberOfPropertyLists;

    /** List of property lists */
    const metadata::PropertyId* mpPropertyLists;

    /** Number of enumeration items */
    size_t mNumberOfEnumerationItems;

    /** List of enumeration items */
    const metadata::EnumerationItem* mpEnumerationItems;

    /** Number of data types */
    size_t mNumberOfDataTypes;

    /** List of data types */
    const metadata::DataType* mpDataTypes;

    /** Number of entries in sparce data type map */
    size_t mNumberOfDataTypeSparseMapEntries;

    /** List of data type sparse map entries */
    const metadata::DataTypeId* mpDataTypeSparseMap;

    /** Number of parameters */
    size_t mNumberOfParameters;

    /** List of parameters */
    const metadata::Parameter* mpParameters;

    /** Number of parameter lists */
    size_t mNumberOfParameterLists;

    /** List of parameter lists */
    const metadata::ParameterId* mpParameterLists;

    /** Number of responses */
    size_t mNumberOfResponses;

    /** List of responses */
    const metadata::Response* mpResponses;

    /** Number of response lists */
    size_t mNumberOfResponseLists;

    /** List of response lists */
    const metadata::ResponseId* mpResponseLists;

    /** Number of operations */
    size_t mNumberOfOperations;

    /** List of operations */
    const metadata::Operation* mpOperations;

    /** Number of operation lists */
    size_t mNumberOfOperationLists;

    /** List of operation lists */
    const metadata::OperationList* mpOperationLists;

    /** Number of security tags */
    size_t mNumberOfSecurityTags;

    /** List of security tags */
    const metadata::SecurityTag* mpSecurityTags;

    /** Returns a string by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const char* getStringById(metadata::StringId id) const
    {
        if (id == metadata::INVALID_STRING)
        {
            return NULL;
        }

        if (id >= mStringMapLength)
        {
            return NULL;
        }

        return &mpStringMap[id];
    }

    /** Returns a data type by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::DataType* getDataTypeById(metadata::DataTypeId id) const
    {
        if (id == metadata::INVALID_DATATYPE)
        {
            return NULL;
        }

        size_t apiId = id >> metadata::API_ID_SHIFT;
        id &= (1 << metadata::API_ID_SHIFT) - 1;

        // Check overflow of bins
        if (apiId + 1 >= mNumberOfDataTypeSparseMapEntries)
        {
            return NULL;
        }

        // Check overflow to next bin
        if (id >= mpDataTypeSparseMap[apiId + 1])
        {
            return NULL;
        }

        id += mpDataTypeSparseMap[apiId];
        return &mpDataTypes[id];
    }

    /** Returns a data type by index
    *
    * @param index ID of the object
    * @param rDataTypeId On output contains ID of the data type
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::DataType* getDataTypeByIndex(size_t index, metadata::DataTypeId& rDataTypeId) const
    {
        if (index >= mNumberOfDataTypes)
        {
            return NULL;
        }

        // Resolve ID
        metadata::ApiId apiId = 0;
        while (index >= mpDataTypeSparseMap[apiId + 1])
        {
            ++apiId;
        }
        
        rDataTypeId = static_cast<metadata::DataTypeId>(
            (static_cast<metadata::DataTypeId>(apiId) << metadata::API_ID_SHIFT) + (index - mpDataTypeSparseMap[apiId]));
        return &mpDataTypes[index];
    }

    /** Returns a property by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::Property* getPropertyById(metadata::PropertyId id) const
    {
        if (id == metadata::INVALID_PROPERTY)
        {
            return NULL;
        }

        if (id >= mNumberOfProperties)
        {
            return NULL;
        }

        return &mpProperties[id];
    }

    /** Returns a property list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::PropertyList* getPropertyListById(metadata::PropertyListId id) const
    {
        if (id == metadata::INVALID_PROPERTY_LIST)
        {
            return NULL;
        }

        if (id >= mNumberOfPropertyLists)
        {
            return NULL;
        }

        return &metadata::PropertyList::toList(&mpPropertyLists[id]);
    }

    /** Returns a enumeration item list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::EnumerationItemList* getEnumerationItemListById(metadata::EnumerationItemListId id) const
    {
        if (id == metadata::INVALID_ENUMERATION_ITEM_LIST)
        {
            return NULL;
        }

        if (id >= mNumberOfEnumerationItems)
        {
            return NULL;
        }

        return &metadata::EnumerationItemList::toList(&mpEnumerationItems[id]);
    }

    /** Returns a parameter by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::Parameter* getParameterById(metadata::ParameterId id) const
    {
        if (id == metadata::INVALID_PARAMETER)
        {
            return NULL;
        }

        if (id >= mNumberOfParameters)
        {
            return NULL;
        }

        return &mpParameters[id];
    }

    /** Returns a parameter list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::ParameterList* getParameterListById(metadata::ParameterListId id) const
    {
        if (id == metadata::INVALID_PARAMETER_LIST)
        {
            return NULL;
        }

        if (id >= mNumberOfParameterLists)
        {
            return NULL;
        }

        return &metadata::ParameterList::toList(&mpParameterLists[id]);
    }

    /** Returns a response by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::Response* getResponseById(metadata::ResponseId id) const
    {
        if (id == metadata::INVALID_RESPONSE)
        {
            return NULL;
        }

        if (id >= mNumberOfResponses)
        {
            return NULL;
        }

        return &mpResponses[id];
    }

    /** Returns a response list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::ResponseList* getResponseListById(metadata::ResponseListId id) const
    {
        if (id == metadata::INVALID_RESPONSE_LIST)
        {
            return NULL;
        }

        if (id >= mNumberOfResponseLists)
        {
            return NULL;
        }

        return &metadata::ResponseList::toList(&mpResponseLists[id]);
    }

    /** Returns a operation by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::Operation* getOperationById(metadata::OperationId id) const
    {
        if (id == metadata::INVALID_OPERATION)
        {
            return NULL;
        }

        if (id >= mNumberOfOperations)
        {
            return NULL;
        }

        return &mpOperations[id];
    }

    /** Returns a operation list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::OperationList* getOperationListById(metadata::OperationListId id) const
    {
        if (id == metadata::INVALID_OPERATION_LIST)
        {
            return NULL;
        }

        if (id >= mNumberOfOperationLists)
        {
            return NULL;
        }

        return &mpOperationLists[id];
    }

    /** Returns a security tag by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    inline const metadata::SecurityTag* getSecurityTagById(SecurityTagId id) const
    {
        if (id == metadata::INVALID_SECURITY_TAG)
        {
            return NULL;
        }

        if (id >= mNumberOfSecurityTags)
        {
            return NULL;
        }

        return &mpSecurityTags[id];
    }

    /// @return number of strings in the const metadata map
    size_t getNumberOfStrings() const
    {
        size_t count = 0;
        for (size_t i = 0; i < mStringMapLength; i++)
        {
            if (mpStringMap[i] == 0)
            {
                count++;
            }
        }

        return count;
    }

    /// @return number of properties in the const metadata map
    inline size_t getNumberOfProperties() const
    {
        return mNumberOfProperties;
    }

    /// @return number of property lists in the const metadata map
    inline size_t getNumberOfPropertyLists() const
    {
        return mNumberOfPropertyLists;
    }

    /// @return number of enumeration items in the const metadata map
    inline size_t getNumberOfEnumerationItems() const
    {
        return mNumberOfEnumerationItems;
    }

    /// @return number of data types in the const metadata map
    inline size_t getNumberOfDataTypes() const
    {
        return mNumberOfDataTypes;
    }

    /// @return number of parameters in the const metadata map
    inline size_t getNumberOfParameters() const
    {
        return mNumberOfParameters;
    }

    /// @return number of parameter lists in the const metadata map
    inline size_t getNumberOfParameterLists() const
    {
        return mNumberOfParameterLists;
    }

    /// @return number of responses in the const metadata map
    inline size_t getNumberOfResponses() const
    {
        return mNumberOfResponses;
    }

    /// @return number of response lists in the const metadata map
    inline size_t getNumberOfResponseLists() const
    {
        return mNumberOfResponseLists;
    }

    /// @return number of operations in the const metadata map
    inline size_t getNumberOfOperations() const
    {
        return mNumberOfOperations;
    }

    /// @return number of operation lists in the const metadata map
    inline size_t getNumberOfOperationLists() const
    {
        return mNumberOfOperationLists;
    }

    /// @return number of security tags in the const metadata map
    inline size_t getNumberOfSecurityTags() const
    {
        return mNumberOfSecurityTags;
    }
};

} // namespace whiteboard
