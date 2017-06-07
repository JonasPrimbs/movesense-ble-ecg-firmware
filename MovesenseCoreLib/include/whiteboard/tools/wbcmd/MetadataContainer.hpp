#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <whiteboard/builtinTypes/Structures.h>
#include <whiteboard/builtinTypes/UnknownStructure.h>
#include <map>
#include <string>
#include <functional>

namespace wbcmd
{

/** Metadata storage that keeps all data in memory */
class MetadataContainer : public wb::IDataTypeMetadataContainer
{
public:
    /** Default constructor */
    MetadataContainer() { }

    /** Destructor */
    virtual ~MetadataContainer() { }

    /** Checks whether the container is empty */
    bool isEmpty() const {
        // There must always be atleast one execution context
        return mExecutionContexts.empty();
    }

    /** Gets execution context with given ID
    *
    * @param executionContextId ID of the execution context
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getExecutionContext(const wb::ExecutionContextId executionContextId, WB_RES::ExecutionContextMetadata& rMetadata) const;

    /** Sets execution context with given ID
    *
    * @param executionContextId ID of the execution context
    * @param rMetadata Metadata of the execution context
    * @return A value indicating whether the execution context was successfully stored
    */
    bool setExecutionContext(const wb::ExecutionContextId executionContextId, const WB_RES::ExecutionContextMetadata& rMetadata);

    /** Removes execution context with given ID
    *
    * @param executionContextId ID of the execution context
    * @return A value indicating whether the execution context was removed
    */
    bool removeExecutionContext(const wb::ExecutionContextId executionContextId);

    /** Gets resource with given ID
    *
    * @param resourceId ID of the resource
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getResource(const wb::LocalResourceId resourceId, WB_RES::ResourceMetadata& rMetadata) const;

    /** Sets resource with given ID
    *
    * @param resourceId ID of the resource
    * @param rMetadata Metadata of thre resource
    * @return A value indicating whether the resource was successfully stored
    */
    bool setResource(const wb::LocalResourceId resourceId, const WB_RES::ResourceMetadata& rMetadata);

    /** Removes resource with given ID
    *
    * @param resourceId ID of the resource
    * @return A value indicating whether the resource was removed
    */
    bool removeResource(const wb::LocalResourceId resourceId);

    /** Gets operation with given ID
    *
    * @param operationId ID of the operation
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getOperation(const WB_RES::LocalOperationId operationId, WB_RES::OperationMetadata& rMetadata) const;

    /** Sets operation with given ID
    *
    * @param operationId ID of the operation
    * @param rMetadata Metadata of the operation
    * @return A value indicating whether the operation was successfully stored
    */
    bool setOperation(const WB_RES::LocalOperationId operationId, const WB_RES::OperationMetadata& rMetadata);

    /** Removes operation with given ID
    *
    * @param operationId ID of the operation
    * @return A value indicating whether the resource was removed
    */
    bool removeOperation(const WB_RES::LocalOperationId operationId);

    /** Gets response with given ID
    *
    * @param responseId ID of the response
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getResponse(const WB_RES::LocalResponseId responseId, WB_RES::ResponseMetadata& rMetadata) const;

    /** Sets response with given ID
    *
    * @param responseId ID of the response
    * @param rMetadata Metadata of the response
    * @return A value indicating whether the response was successfully stored
    */
    bool setResponse(const WB_RES::LocalResponseId responseId, const WB_RES::ResponseMetadata& rMetadata);

    /** Removes response with given ID
    *
    * @param responseId ID of the response
    * @return A value indicating whether the response was removed
    */
    bool removeResponse(const WB_RES::LocalResponseId responseId);

    /** Gets parameter with given ID
    *
    * @param parameterId ID of the parameter
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getParameter(const WB_RES::LocalParameterId parameterId, WB_RES::ParameterMetadata& rMetadata) const;

    /** Sets parameter with given ID
    *
    * @param parameterId ID of the parameter
    * @param rMetadata Metadata of the parameter
    * @return A value indicating whether the parameter was successfully stored
    */
    bool setParameter(const WB_RES::LocalParameterId parameterId, const WB_RES::ParameterMetadata& rMetadata);

    /** Removes parameter with given ID
    *
    * @param parameterId ID of the parameter
    * @return A value indicating whether the parameter was removed
    */
    bool removeParameter(const WB_RES::LocalParameterId parameterId);

    /** Gets data type with given ID
    *
    * @param dataTypeId ID of the data type
    * @param rMetadata On output contains requested metadata
    * @return A value indicating whether data was returned
    */
    bool getDataType(const wb::LocalDataTypeId dataTypeId, WB_RES::DataTypeMetadata& rMetadata) const override final;

    /** Releases all resources allocated for given metadata structure
    *
    * @param rMetadata Metadata which resources should be released
    */
    void releaseDataType(WB_RES::DataTypeMetadata& /*rMetadata*/) const override final
    {
        // getDataType returns does not allocate new memory. No need to free anything here.
    }

    /** Sets data type with given ID
    *
    * @param dataTypeId ID of the data type
    * @param rMetadata Metadata of the data type
    * @return A value indicating whether the data type was successfully stored
    */
    bool setDataType(const wb::LocalDataTypeId dataTypeId, const WB_RES::DataTypeMetadata& rMetadata);

    /** Removes data type with given ID
    *
    * @param dataTypeId ID of the data type
    * @return A value indicating whether data was returned
    */
    bool removeDataType(const wb::LocalDataTypeId dataTypeId);

    /// @see whiteboard::IMetadataContainer::getSecurityTag
    bool getSecurityTag(const WB_RES::SecurityTagId securityTagId, WB_RES::SecurityTagMetadata& rMetadata) const;
    bool setSecurityTag(const WB_RES::SecurityTagId securityTagId, const WB_RES::SecurityTagMetadata& rMetadata);
    bool removeSecurityTag(const WB_RES::SecurityTagId securityTagId);

    /// @see whiteboard::IMetadataContainer::getString
    const char* getString(const WB_RES::StringId stringId) const override final;
    
    /** Releases all resources allocated for given string
    *
    * @param string String which resources should be released
    */
    void releaseString(const char* /*string*/) const override final
    {
        // getString returns does not allocate new memory. No need to free anything here.
    }

    bool setString(const WB_RES::StringId stringId, const WB_RES::StringMetadata& rMetadata);
    bool removeString(const WB_RES::StringId stringId);

    /**
    Seeks through all named data types within the container to find the one of which name is given as an input.
    @param name [in] name of the named dataType to find
    @return Local id of the data type or ID_INVALID_LOCAL_DATATYPE if not found
    */
    wb::LocalDataTypeId findNamedDataTypeByName(const std::string& name) const;

    /**
    Seeks through enum items of the given datatype (must be enum) to find the value that matches the given enum item name. 
    The find is case insensitive.

    @param dataTypeId [in] name of the named dataType to find
    @param name [in] name of the named dataType to find
    @param rValue [out] The value of the given enumeration item name
    @return false if not found, rValue is untouched then
    */
    bool findEnumValueByName(wb::LocalDataTypeId dataTypeId, const std::string& name, std::string& rValue) const;

private:
    /** Map of execution contexts */
    std::map<wb::ExecutionContextId, WB_RES::ExecutionContextMetadata> mExecutionContexts;

    /** Map of resources */
    std::map<wb::LocalResourceId, WB_RES::ResourceMetadata> mResources;

    /** Map of operations */
    std::map<WB_RES::LocalOperationId, WB_RES::OperationMetadata> mOperations;

    /** Map of responses */
    std::map<WB_RES::LocalResponseId, WB_RES::ResponseMetadata> mResponses;

    /** Map of parameters */
    std::map<WB_RES::LocalParameterId, WB_RES::ParameterMetadata> mParameters;

    /** Map of data types */
    std::map<wb::LocalDataTypeId, WB_RES::DataTypeMetadata> mDataTypes;

    /** Map of security tags */
    std::map<uint16, WB_RES::SecurityTagMetadata> mSecurityTags;

    /** Map of strings. Sorted in reverse order so that we can use
    * std::map::lower_bound to find location of compressed strings. */
    std::map<uint16, std::string, std::greater<uint16>> mStrings;
};

} // namespace wbcmd
