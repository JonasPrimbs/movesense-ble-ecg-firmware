// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

#include "whiteboard/metadata/MetadataStructures.h"
#include "whiteboard/Result.h"
#include "whiteboard/Value.h"

namespace whiteboard
{

// Forward declarations
class MetadataMap;

/** Class that can be used to construct ResourceSubtree dynamically.
 *
 * @note This class is using dynamic memory allocations and should not
 * be used in production ESW code
 * @note Generated resource subtree is compatible with WBRES generated subtrees,
 *       but is not optimized.
 */
class WB_API MetadataBuilder FINAL
{
public:

    /** Default constructor; uses next available apiId of the local whiteboard
    */
    MetadataBuilder();

    /** Constructor
     *
     * @param apiId ID of API to generate. This ID should be unique for 
     *        the application
     */
    MetadataBuilder(metadata::ApiId apiId);

    /** Destructor */
    ~MetadataBuilder();

    /** Finish metadata building and construct resource subtree 
     *
     * @return Pointer to new ResourceSubtree instance
     */
    MetadataMap* finish();

    /** Adds a string to resource metadata
     *
     * @param string String to add
     * @return Id of the added string
     */
    metadata::StringId addString(const char* string);

    /** Adds a scalar datatype to resource metadata
    *
    * @param type Type of the scalar value
    * @param unitInfo Unit information
    * @return Id of the added scalar datatype
    */
    LocalDataTypeId addScalarDataType(ValueType type, unit::Info unitInfo);

    /** Adds a named datatype to resource metadata
    *
    * @param nameId Name of the type
    * @param dataTypeId Base data type
    * @return Id of the added named datatype
    */
    LocalDataTypeId addNamedDataType(metadata::StringId nameId, LocalDataTypeId dataTypeId);

    /** Adds a array datatype to resource metadata
    *
    * @param dataTypeId Item data type
    * @return Id of the added array datatype
    */
    LocalDataTypeId addArrayDataType(LocalDataTypeId dataTypeId);
 
    /** Adds a structure property to resource metadata
    *
    * @param rProperty Property to add
    * @return Id of the added structure property
    */
    metadata::PropertyId addProperty(const metadata::Property& rProperty);

    /** Adds a structure property to resource metadata
    *
    * @param nameId Name of the property
    * @param dataTypeId Data type of the property
    * @param required A value indicating whether property is required
    * @param inlineStorage A value indicating whether property is stored inline in the structure or pointer like outside it
    * @return Id of the added structure property
    */
    metadata::PropertyId addProperty(
        metadata::StringId nameId, LocalDataTypeId dataTypeId, bool required, bool inlineStorage);

    /** Adds a list of properties to resource metadata
    *
    * @tparam N Size of the property list
    * @param propertyIds List of properties to add
    * @return Id of the added property list
    */
    template <size_t N>
    inline metadata::PropertyListId addPropertyList(const metadata::PropertyId propertyIds[N])
    {
        return addPropertyList(N, propertyIds);
    }

    /** Adds a list of properties to resource metadata
    *
    * @param numberOfProperties Size of the property list
    * @param propertyIds List of properties to add
    * @return Id of the added property list
    */
    metadata::PropertyListId addPropertyList(size_t numberOfProperties, const metadata::PropertyId* propertyIds);

    /** Adds a structure to resource metadata
    *
    * @tparam N Size of the property list
    * @param propertyIds List of properties to add
    * @return Id of the added structure
    */
    template <size_t N>
    inline LocalDataTypeId addStructureDataType(const metadata::PropertyId propertyIds[N])
    {
        return addStructureDataType(addPropertyList(N, propertyIds));
    }

    /** Adds a structure to resource metadata
    *
    * @param numberOfProperties Size of the property list
    * @param propertyIds List of properties
    * @return Id of the added structure
    */
    inline LocalDataTypeId addStructureDataType(size_t numberOfProperties, const metadata::PropertyId* propertyIds)
    {
        return addStructureDataType(addPropertyList(numberOfProperties, propertyIds));
    }

    /** Adds a structure to resource metadata
    *
    * @param propertyListId ID of property list
    * @return Id of the added structure
    */
    LocalDataTypeId addStructureDataType(metadata::PropertyListId propertyListId);

    /** Adds a list of enumeration items to resource metadata
    *
    * @tparam N Size of the enumeration item list
    * @param items List of enumeration items to add
    * @return Id of the added enumeration item list
    */
    template <size_t N>
    inline metadata::EnumerationItemListId addEnumerationItemList(const metadata::EnumerationItem items[N])
    {
        return addEnumerationItemList(N, items);
    }

    /** Adds a list of enumeration items to resource metadata
    *
    * @param numberOfItems Size of the enumeration item list
    * @param pItems List of enumeration items to add
    * @return Id of the added enumeration item list
    */
    metadata::EnumerationItemListId addEnumerationItemList(size_t numberOfItems, const metadata::EnumerationItem* pItems);

    /** Adds an enumeration data type to resource metadata
    *
    * @param dataTypeId ID of the base data type
    * @tparam N Size of the enumeration item list
    * @param items List of enumeration items to add
    * @return Id of the added enumeration data type
    */
    template <size_t N>
    inline LocalDataTypeId addEnumerationDataType(
        LocalDataTypeId dataTypeId, const metadata::EnumerationItem items[N])
    {
        return addEnumerationDataType(dataTypeId, addEnumerationItemList(N, items));
    }

    /** Adds an enumeration data type to resource metadata
    *
    * @param dataTypeId ID of the base data type
    * @param numberOfItems Size of the enumeration item list
    * @param pItems List of enumeration items to add
    * @return Id of the added enumeration data type
    */
    inline LocalDataTypeId addEnumerationDataType(
        LocalDataTypeId dataTypeId, size_t numberOfItems, const metadata::EnumerationItem* pItems)
    {
        return addEnumerationDataType(dataTypeId, addEnumerationItemList(numberOfItems, pItems));
    }

    /** Adds an enumeration data type to resource metadata
    *
    * @param dataTypeId ID of the base data type
    * @param enumerationItemListId Enumeration item list
    * @return Id of the added enumeration data type
    */
    LocalDataTypeId addEnumerationDataType(
        LocalDataTypeId dataTypeId, metadata::EnumerationItemListId enumerationItemListId);

    /** Adds a parameter to resource metadata
    *
    * @param rParameter Parameter to add
    * @return Id of the added parameter
    */
    metadata::ParameterId addParameter(const metadata::Parameter& rParameter);

    /** Adds a parameter to resource metadata
    *
    * @param nameId Name of the parameter
    * @param required A value indicating whether the parameter is required
    * @param dataTypeId Id of the parameter's data type
    * @return Id of the added parameter
    */
    metadata::ParameterId addParameter(metadata::StringId nameId, bool required, LocalDataTypeId dataTypeId);

    /** Adds a parameter list to resource metadata
    *
    * @tparam N Size of the parameter list
    * @param parameterIds List of parameters to add
    * @return Id of the added parameter list
    */
    template <size_t N>
    inline metadata::ParameterListId addParameterList(const metadata::ParameterId parameterIds[N])
    {
        return addParameterList(N, parameterIds);
    }

    /** Adds a parameter list to resource metadata
    *
    * @param parameterId1 Optional 1st parameter of the list
    * @param parameterId2 Optional 2nd parameter of the list
    * @param parameterId3 Optional 3rd parameter of the list
    * @param parameterId4 Optional 4th parameter of the list
    * @param parameterId5 Optional 5th parameter of the list
    * @param parameterId6 Optional 6th parameter of the list
    * @param parameterId7 Optional 7th parameter of the list
    * @param parameterId8 Optional 8th parameter of the list
    * @return Id of the added parameter list
    */
    metadata::ParameterListId addParameterList(
        const metadata::ParameterId parameterId1 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId2 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId3 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId4 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId5 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId6 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId7 = metadata::INVALID_PARAMETER,
        const metadata::ParameterId parameterId8 = metadata::INVALID_PARAMETER);

    /** Adds a parameter list to resource metadata
    *
    * @param numberOfParameters Size of the parameter list
    * @param parameterIds List of parameters to add
    * @return Id of the added parameter list
    */
    metadata::ParameterListId addParameterList(size_t numberOfParameters, const metadata::ParameterId* parameterIds);

    /** Adds a response to resource metadata
    *
    * @param rResponse Response to add
    * @return Id of the added response
    */
    metadata::ResponseId addResponse(const metadata::Response& rResponse);

    /** Adds a response to resource metadata
    *
    * @param resultCode Result code
    * @param dataTypeId Optional data type of the response
    * @return Id of the added response
    */
    metadata::ResponseId addResponse(Result resultCode, LocalDataTypeId dataTypeId = ID_INVALID_LOCAL_DATA_TYPE);

    /** Adds a notification to resource metadata
    *
    * @param dataTypeId Data type of the notification
    * @param parameterListId Optional parameter list for the notification
    * @return Id of the added notification typed response
    */
    metadata::ResponseId addNotification(LocalDataTypeId dataTypeId, metadata::ParameterListId parameterListId = metadata::INVALID_PARAMETER_LIST);

    /** Adds a response list to resource metadata
    *
    * @note Notification must be first member subscription response lists
    *
    * @tparam N Size of the response list
    * @param responseIds List of responses to add
    * @return Id of the added response list
    */
    template <size_t N>
    inline metadata::ResponseListId addResponseList(const metadata::ResponseId responseIds[N])
    {
        return addResponseList(N, responseIds);
    }

    /** Adds a response list to resource metadata
    *
    * @note Notification must be first member subscription response lists
    *
    * @param responseId1 Optional 1st response of the list
    * @param responseId2 Optional 2nd response of the list
    * @param responseId3 Optional 3rd response of the list
    * @param responseId4 Optional 4th response of the list
    * @param responseId5 Optional 5th response of the list
    * @param responseId6 Optional 6th response of the list
    * @param responseId7 Optional 7th response of the list
    * @param responseId8 Optional 8th response of the list
    * @param responseId9 Optional 9th response of the list
    * @param responseId10 Optional 10th response of the list
    * @return Id of the added response list
    */
    metadata::ResponseListId addResponseList(
        const metadata::ResponseId responseId1 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId2 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId3 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId4 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId5 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId6 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId7 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId8 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId9 = metadata::INVALID_RESPONSE,
        const metadata::ResponseId responseId10 = metadata::INVALID_RESPONSE);

    /** Adds a response list to resource metadata
    *
    * @note Notification must be first member subscription response lists
    *
    * @param numberOfResponses Size of the response list
    * @param responseIds List of responses to add
    * @return Id of the added response list
    */
    metadata::ResponseListId addResponseList(size_t numberOfResponses, const metadata::ResponseId* responseIds);

    /** Adds an operation to resource metadata
    *
    * @param operationType Type of the operation
    * @param parameterListId List of parameters
    * @param responseListId List of response
    * @param securityMask Security mask of the operation (required security tokens)
    * @return Id of the added operation
    */
    metadata::OperationId addOperation(
        uint8 operationType,
        metadata::ParameterListId parameterListId,
        metadata::ResponseListId responseListId,
        metadata::SecurityMask securityMask);

    /** Adds an operation list to resource metadata
    *
    * @param getId Optional get operation
    * @param putId Optional put operation
    * @param postId Optional post operation
    * @param deleteId Optional delete operation
    * @param subscribeId Optional subscribe operation
    * @param unsubscribeId Optional unsubscribe operation
    * @return Id of the added operation list
    */
    metadata::OperationListId addOperationList(
        metadata::OperationId getId = metadata::INVALID_OPERATION,
        metadata::OperationId putId = metadata::INVALID_OPERATION,
        metadata::OperationId postId = metadata::INVALID_OPERATION,
        metadata::OperationId deleteId = metadata::INVALID_OPERATION,
        metadata::OperationId subscribeId = metadata::INVALID_OPERATION,
        metadata::OperationId unsubscribeId = metadata::INVALID_OPERATION);

    /** Adds root resource */
    LocalResourceId addRoot();

    /** Adds a resource to resource metadata
    *
    * @param nameId Name of the resource
    * @param executionContextId Id of resource's execution context
    * @param parentId ID of the parent resource
    * @param operations List of operations
    * @param pathParameterCount Number of path parameters in the path
    * @param pathParameterId Id of path parameter of current resource
    * @return Id of the added resource
    */
    LocalResourceId addResource(
        metadata::StringId nameId,
        ExecutionContextId executionContextId,
        LocalResourceId parentId,
        metadata::OperationListId operations,
        uint8 pathParameterCount = 0,
        metadata::ParameterId pathParameterId = metadata::INVALID_PARAMETER);

private:
    template <typename T>
    size_t addObject(size_t& rItemCount, T*& rpItemList, const T& rItem);

    template <typename T>
    size_t addObjects(size_t& rItemCount, T*& rpItemList, size_t numberOfItems, const T* pItems);

    /** Number of characters to allocate more when needed */
    static const size_t CHAR_ALLOCATION_STEP = 64;

    /** Number of items to allocate more when needed */
    static const size_t ALLOCATION_STEP = 8;

private:
    /** API ID */
    metadata::ApiId mApiId;

    /** Number of strings */
    size_t mStringMapLength;

    /** String map (id is offset of a string in the map) */
    char* mpStringMap;

    /** Number of execution contexts */
    size_t mNumberOfExecutionContexts;

    /** List of execution contexts */
    metadata::ExecutionContext* mpExecutionContexts;

    /** Number of security tags */
    size_t mNumberOfSecurityTags;

    /** List of security tags */
    metadata::SecurityTag* mpSecurityTags;

    /** Number of properties */
    size_t mNumberOfProperties;

    /** List of properties */
    metadata::Property* mpProperties;

    /** Number of property lists */
    size_t mNumberOfPropertyListMapEntries;

    /** List of property lists */
    metadata::PropertyId* mpPropertyLists;

    /** Number of enumeration items */
    size_t mNumberOfEnumerationItems;

    /** List of enumeration items */
    metadata::EnumerationItem* mpEnumerationItems;

    /** Number of data types */
    size_t mNumberOfDataTypes;

    /** List of data types */
    metadata::DataType* mpDataTypes;

    /** Number of parameters */
    size_t mNumberOfParameters;

    /** List of parameters */
    metadata::Parameter* mpParameters;

    /** Number of parameter lists */
    size_t mNumberOfParameterListMapEntries;

    /** List of parameter lists */
    metadata::ParameterId* mpParameterLists;

    /** Number of responses */
    size_t mNumberOfResponses;

    /** List of responses */
    metadata::Response* mpResponses;

    /** Number of response lists */
    size_t mNumberOfResponseListMapEntries;

    /** List of response lists */
    metadata::ResponseId* mpResponseLists;

    /** Number of operations */
    size_t mNumberOfOperations;

    /** List of operations */
    metadata::Operation* mpOperations;

    /** Number of operation lists */
    size_t mNumberOfOperationListMapEntries;

    /** List of operation lists */
    metadata::OperationList* mpOperationLists;

    /** Number of resources */
    size_t mNumberOfResources;

    /** Array of metadata tree nodes. */
    metadata::ResourceTreeNode* mpTreeNodes;
};

} // namespace whiteboard
