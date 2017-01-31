#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/Request.h"
#include "whiteboard/Result.h"
#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

// Forward declarations
class ParameterList;
class Value;
struct MetadataMap;

/** Maximum length of resource name */
static const int MAX_RESOURCE_NAME_LENGTH = 64;

/** Interface for accessing resource metadata */
class ResourceMetadata FINAL
{
public:
    /** Default constructor
    */
    ResourceMetadata();

    /** Constructor
     *
     * @param localResourceId Id of the resource which metadata this is
     * @param rMetadataMap Metadata map that contains data for this resource
     * @param rPath Pointer to resource metadata
     */
    ResourceMetadata(LocalResourceId localResourceId, const MetadataMap& rMetadataMap, const metadata::Path& rPath);

    /** Destructor */
    ~ResourceMetadata();

    /** Gets metadata map that contains data for this resource
    *
    * @return Metadata map of the resource
    */
    const MetadataMap& getMetadataMap() const;

    /**
    *	Gets name of the resource
    *
    *	@return Name of the resource
    */
    const char* getName() const;

    /**
    *	Gets execution context ID of the resource
    *
    *	@return ID of resource's execution context
    */
    ExecutionContextId getExecutionContextId() const;

    /**
    *	Gets ID of parent resource
    *
    *	@return ID of parent resource or ID_INVALID_LOCAL_RESOURCE if this the root resource
    */
    LocalResourceId getParentId() const;

    /**
    *	Gets ID of next sibling in the resource tree
    *
    *	@return ID of next sibling in resource tree or ID_INVALID_LOCAL_RESOURCE if this is last resource
    */
    LocalResourceId getNextSiblingId() const;

    /**
    *	Gets ID of first child resource in the resource tree
    *
    *	@return ID of first child in resource tree or ID_INVALID_LOCAL_RESOURCE if there are no children
    */
    LocalResourceId getFirstChildId() const;

    /** Gets metadata descriptor of the resource
     *
     * @return Reference to resource metadata
     */
    const metadata::Path& getMetadataDescriptor() const;

    /** Returns a string by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const char* getStringById(metadata::StringId id) const;

    /** Returns a data type by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::DataType* getDataTypeById(metadata::DataTypeId id) const;

    /** Returns a property by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::Property* getPropertyById(metadata::PropertyId id) const;

    /** Returns a property list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::PropertyList* getPropertyListById(
        metadata::PropertyListId id) const;

    /** Returns a enumeration item list by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::EnumerationItemList* getEnumerationItemListById(
        metadata::EnumerationItemListId id) const;

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
    const metadata::ParameterList* getParameterListById(
        metadata::ParameterListId id) const;

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
    const metadata::ResponseList* getResponseListById(
        metadata::ResponseListId id) const;

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
    const metadata::OperationList* getOperationListById(
        metadata::OperationListId id) const;

    /** Returns a security tag by ID
    *
    * @param id ID of the object
    * @return Object or NULL if invalid id is given
    */
    const metadata::SecurityTag* getSecurityTagById(metadata::SecurityTagId id) const;

    /**
    Checks that the given operation is allowed for this resource, with the given security rights.

    @param requestType [in] Type of the request to be performed
    @param clientSecurityMask [in] security mask of the resource client's executions context
    @return true if security mask has adequate rights, else false.
    */
    bool checkSecurityMask(RequestType requestType, metadata::SecurityMask clientSecurityMask) const;

    /**
    Type-checks provider updateResource, that the value is what is spec'ed in the yaml and the parameters match.

    @param rValue [in] The value the provider gives in the ResourceProvider::updateResource-call
    @param rParameterList [in] The parameter list the provider gives in the ResourceProvider::updateResource-call

    @return true if given input match with the yaml, otherwise false. In debug version outputs reson
    for the failure in the debuglog.
    */
    bool typecheckProviderUpdateResource(const Value& rValue, const ParameterList& rParameterList) const;

    /**
    Type-checks provider returnResult, that the resultCode and value is what is spec'ed in the yaml.

    @param requestType [in] Type of returnResult to check (GET, PUT, ...)
    @param resultCode [in] Code given by the provider in the ResourceProvider::returnResult-call
    @param rValue [in] The value the provider gives in the ResourceProvider::updateResource-call

    @return true if given input match with the yaml, otherwise false. In debug version outputs reson
    for the failure in the debuglog.
    */
    bool typecheckProviderReturnResult(RequestType requestType, Result resultCode, const Value& rValue) const;

    /**
    Type-checks client request (GET, PUT, SUBSCRIBE...), that the parameters match what is spec'ed in the yaml

    @param requestType [in] Type of the request the client is performing (GET, PUT, ...)
    @param rParameterList [in] The parameter list the given by the client in it's request.

    @return true if given input match with the yaml, otherwise false. In debug version outputs reson
    for the failure in the debuglog.
    */
    bool typecheckClientRequest(RequestType requestType, const ParameterList& rParameterList) const;

    /** @return count of path variables found between this resource and the root resource */
    size_t getPathParameterCount() const;

    /**
    @return true if this resource is a path variable. An example of a path variable is
    /maps/{mapId}/tiles/{tileId}, where {mapId} and {tileId} are path variables.
    */
    bool isPathParameter() const;

    /** @return true if any of the parents of this resource is a path variable resource */
    bool isThisOrAnyParentPathParameter() const;

private:
    /** Gets metadata for operation
    *
    * @param requestType Type of the request
    * @return Operation metadata or NULL if operation is not found
    */
    const metadata::Operation* getOperationMetadata(
        RequestType requestType) const;

    /** Gets metadata for operation parameters
    *
    * @param requestType Type of the request
    * @param parameterCount On output contains number of parameters
    * @param pParameters On output contains pointer to parameter metadata
    * @return A value indicating whether metadata was found
    */
    bool getParameterMetadata(
        RequestType requestType,
        size_t& parameterCount,
        const metadata::Parameter** pParameters) const;

    /** Gets metadata for operation response 
     *
     * @param requestType Type of the request
     * @param resultCode Result code of the operation
     * @param pResponse On output contains pointer to response metadata
     * @return A value indicating whether metadata was found
     */
    bool getResponseMetadata(
        RequestType requestType,
        Result resultCode,
        const metadata::Response*& pResponse) const;

    /** Checks data type against all possible base types
     *
     * @param datatypeId Id of the data type 
     * @param datatypeToCheck Id of the expected base data type
     * @return A value indicating whether data types match
     */
    bool checkAgainstBaseType(uint16 datatypeId, uint16 datatypeToCheck) const;

private:
    /** ID of the local resource which metadata this is */
    LocalResourceId mLocalResourceId;

    /** Metadata map to use with given resource */
    const MetadataMap* mpMetadataMap;
    
    /** Pointer to resource metadata */
    const metadata::Path* mpPath;
};

} // namespace whiteboard
