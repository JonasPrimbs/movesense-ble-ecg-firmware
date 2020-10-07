#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/metadata/IDataTypeMetadata.h"
#include "whiteboard/metadata/MetadataStructures.h"
#include "whiteboard/Result.h"

namespace whiteboard
{

/** Map of read-only resource metadata */
class WB_API MetadataMap : public IDataTypeMetadata
{
public:
    /** Constructor 
     *
     * @param pMetadataBlob Metadata blob instance
     * @param numberOfBytes Size of the metadata blop
     */
    MetadataMap(const uint32* pMetadataBlob, size_t numberOfBytes);

    /** Destructor */
    virtual ~MetadataMap();

    /** Checks validity of the metadata blob 
    *
    * @return A value indicating whether the metadata blob i valid or not;
    * HTTP_CODE_OK - the blob is valid
    * HTTP_CODE_BAD_VERSION - the blob version is not correct
    * HTTP_CODE_BAD_REQUEST - the blob is corrupt
    */
    Result isValid() const;

    /// @return The metadata header
    inline const metadata::MetadataBlobHeader& getMetadataHeader() const
    {
        return *reinterpret_cast<const metadata::MetadataBlobHeader*>(mpMetadataBlob);
    }

    /// @return number of response lists in the metadata map
    inline metadata::MetadataBlobItemCount getStringMapLength() const
    {
        return getMetadataHeader().stringMapLength;
    }

    /// @return Number of execution contexts
    inline metadata::MetadataBlobItemCount getNumberOfExecutionContexts() const
    {
        return getMetadataHeader().numberOfExecutionContexts;
    }

    /// @return Number of security tags in the resource tree
    inline metadata::MetadataBlobItemCount getNumberOfMountPoints() const
    {
        return getMetadataHeader().numberOfMountPoints;
    }

    /// @return number of strings in the metadata map
    metadata::MetadataBlobItemCount countNumberOfStrings() const;

    /// @return number of properties in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfProperties() const
    {
        return getMetadataHeader().numberOfProperties;
    }

    /// @return number of property lists in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfPropertyLists() const
    {
        return getMetadataHeader().numberOfPropertyListMapEntries;
    }

    /// @return number of sequential enumeration items in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfSequentialEnumerationItems() const
    {
        return getMetadataHeader().numberOfSequentialEnumerationItems;
    }

    /// @return number of enumeration items in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfNonSequentialEnumerationItems() const
    {
        return getMetadataHeader().numberOfNonSequentialEnumerationItems;
    }

    /// @return number of data types in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfDataTypes() const
    {
        return getMetadataHeader().numberOfDataTypes;
    }

    /// @return Number of sparse map entries in the data type map
    inline metadata::MetadataBlobItemCount getNumberOfDataTypeSparseMapEntries() const
    {
        return getMetadataHeader().numberOfDataTypeSparseMapEntries;
    }

    /// @return number of parameters in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfParameters() const
    {
        return getMetadataHeader().numberOfParameters;
    }

    /// @return number of parameter lists in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfParameterLists() const
    {
        return getMetadataHeader().numberOfParameterListMapEntries;
    }

    /// @return number of responses in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfResponses() const
    {
        return getMetadataHeader().numberOfResponses;
    }

    /// @return number of response lists in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfResponseLists() const
    {
        return getMetadataHeader().numberOfResponseListMapEntries;
    }

    /// @return number of operations in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfOperations() const
    {
        return getMetadataHeader().numberOfOperations;
    }

    /// @return number of operation lists in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfOperationLists() const
    {
        return getMetadataHeader().numberOfOperationLists;
    }

    /// @return number of security tags in the metadata map
    inline metadata::MetadataBlobItemCount getNumberOfSecurityTags() const
    {
        return getMetadataHeader().numberOfSecurityTags;
    }

    /// @return Number of resource tree nodes
    inline metadata::MetadataBlobItemCount getNumberOfResourceTreeNodes() const
    {
        return getMetadataHeader().numberOfResourceTreeNodes;
    }

    /// @return Number of sparse map entries in the resource tree
    inline metadata::MetadataBlobItemCount getNumberOfResourceTreeSparseMapEntries() const
    {
        return getMetadataHeader().numberOfResourceTreeSparseMapEntries;
    }

    /** @return array of strings */
    inline const char* getStringMap() const
    {
        return reinterpret_cast<const char*>(
            mpMetadataBlob + getMetadataHeader().offsetToStringMap);
    }

    /** @return array of execution contexts */
    inline const metadata::ExecutionContext* getExecutionContexts() const
    {
        return reinterpret_cast<const metadata::ExecutionContext*>(
            mpMetadataBlob + getMetadataHeader().offsetToExecutionContexts);
    }

    /** @return array of properties */
    inline const metadata::Property* getProperties() const
    {
        return reinterpret_cast<const metadata::Property*>(
            mpMetadataBlob + getMetadataHeader().offsetToProperties);
    }

    /** @return array of property lists */
    inline const metadata::PropertyId* getPropertyLists() const
    {
        return reinterpret_cast<const metadata::PropertyId*>(
            mpMetadataBlob + getMetadataHeader().offsetToPropertyLists);
    }

    /** @return array of sequential enumeration items */
    inline const metadata::SequentialEnumerationItem* getSequentialEnumerationItems() const
    {
        return reinterpret_cast<const metadata::SequentialEnumerationItem*>(
            mpMetadataBlob + getMetadataHeader().offsetToSequentialEnumerationItems);
    }

    /** @return array of enumeration items */
    inline const metadata::EnumerationItem* getNonSequentialEnumerationItems() const
    {
        return reinterpret_cast<const metadata::EnumerationItem*>(
            mpMetadataBlob + getMetadataHeader().offsetToNonSequentialEnumerationItems);
    }

    /** @return array of data types */
    inline const metadata::DataType* getDataTypes() const
    {
        return reinterpret_cast<const metadata::DataType*>(
            mpMetadataBlob + getMetadataHeader().offsetToDataTypes);
    }

    /** @return array of data type sparse map entries */
    inline const LocalDataTypeId* getDataTypeSparseMap() const
    {
        return reinterpret_cast<const LocalDataTypeId*>(
            mpMetadataBlob + getMetadataHeader().offsetToDataTypeSparseMap);
    }

    /** @return array of parameters */
    inline const metadata::Parameter* getParameters() const
    {
        return reinterpret_cast<const metadata::Parameter*>(
            mpMetadataBlob + getMetadataHeader().offsetToParameters);
    }

    /** @return array of parameter lists */
    inline const metadata::ParameterId* getParameterLists() const
    {
        return reinterpret_cast<const metadata::ParameterId*>(
            mpMetadataBlob + getMetadataHeader().offsetToParameterLists);
    }

    /** @return array of responses */
    inline const metadata::Response* getResponses() const
    {
        return reinterpret_cast<const metadata::Response*>(
            mpMetadataBlob + getMetadataHeader().offsetToResponses);
    }

    /** @return array of response lists */
    inline const metadata::ResponseId* getResponseLists() const
    {
        return reinterpret_cast<const metadata::ResponseId*>(
            mpMetadataBlob + getMetadataHeader().offsetToResponseLists);
    }

    /** @return array of operations */
    inline const metadata::Operation* getOperations() const
    {
        return reinterpret_cast<const metadata::Operation*>(
            mpMetadataBlob + getMetadataHeader().offsetToOperations);
    }

    /** @return array of operation lists */
    inline const metadata::OperationList* getOperationLists() const
    {
        return reinterpret_cast<const metadata::OperationList*>(
            mpMetadataBlob + getMetadataHeader().offsetToOperationLists);
    }

    /** @return array of security tags */
    inline const metadata::SecurityTag* getSecurityTags() const
    {
        return reinterpret_cast<const metadata::SecurityTag*>(
            mpMetadataBlob + getMetadataHeader().offsetToSecurityTags);
    }

    /** @return array of resource tree nodes in the resource tree */
    inline const metadata::ResourceTreeNode* getResourceTreeNodes() const
    {
        return reinterpret_cast<const metadata::ResourceTreeNode*>(
            mpMetadataBlob + getMetadataHeader().offsetToResourceTreeNodes);
    }

    /** @return array of resource tree node sparse map entries */
    inline const LocalResourceId* getResourceTreeSparseMap() const
    {
        return reinterpret_cast<const LocalResourceId*>(
            mpMetadataBlob + getMetadataHeader().offsetToResourceTreeSparseMap);
    }

    /** Returns a string by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const char* getString(metadata::StringId id) const OVERRIDE;

    /** Returns a data type by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::DataType* getDataType(LocalDataTypeId id) const OVERRIDE;

    /** Returns a data type by index
    *
    * @param index ID of the object
    * @param rDataTypeId On output contains ID of the data type
    * @return Object or NULL if invalid id is given
    */
    const metadata::DataType* getDataTypeByIndex(size_t index, LocalDataTypeId& rDataTypeId) const;

    /** Finds data type by name
    *
    * @param name Name of the data type
    * @param rDataTypeId On output contains ID of the data type
    * @return Object or NULL if data type was not found
    */
    const metadata::DataType* getDataTypeByName(const char* name, LocalDataTypeId& rDataTypeId) const;

    /** Returns a property by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::Property* getProperty(metadata::PropertyId id) const OVERRIDE;

    /** Returns a property list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::PropertyList* getPropertyList(metadata::PropertyListId id) const OVERRIDE;
 
    /** Returns a enumeration item list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::EnumerationItemList getEnumerationItemList(metadata::EnumerationItemListId id) const OVERRIDE;
 
    /** Returns a parameter by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::Parameter* getParameterById(metadata::ParameterId id) const;

    /** Returns a parameter list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::ParameterList* getParameterListById(metadata::ParameterListId id) const;

    /** Returns a response by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::Response* getResponseById(metadata::ResponseId id) const;

    /** Returns a response list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::ResponseList* getResponseListById(metadata::ResponseListId id) const;

    /** Returns a operation by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::Operation* getOperationById(metadata::OperationId id) const;

    /** Returns a operation list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::OperationList* getOperationListById(metadata::OperationListId id) const;

    /** Returns a security tag by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::SecurityTag* getSecurityTagById(SecurityTagId id) const;

private:
    /** Metadata blob */
    const uint8* mpMetadataBlob;
};

} // namespace whiteboard
