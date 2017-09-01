// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "wb-resources/resources.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/metadata/MetadataStructures.h"
#include "whiteboard/internal/services/MetadataHelpers.h"

namespace whiteboard
{

class ResourceMetadata;
namespace internal
{
    class ResourceSubtree;
}

namespace services
{

/// Metadata provider. Provides information about resources registered on the whiteboard and their REST API.
class MetadataProvider : private ResourceProvider
{
public:
    /// Initializes a new instance of MetadataProvider
    MetadataProvider();

    /// Destructor
    virtual ~MetadataProvider();

    /// Called by the WhiteboardServices launchable module
    void initSubmodule();
    void deinitSubmodule();

// TODO: Change unit tests to use public API and TestClient for testing, make this private and mark class final
protected:
    /**
    *   Metadata get request handler
    *
    *   @see whiteboard::ResourceProvider::onGetRequest
    */
    void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

    /***
    *   Timeout timer handler to reset stream operation pending too long
    *
    *  @see whiteboard::ResourceProvider::onTimer
    */
    virtual void onTimer(TimerId timerId) OVERRIDE;

    /// These functions get, fill, serialize, return result to requested metadata
    Result fillGetExecutionContextByIdMetadata(ExecutionContextId executionContextId,
                                               WB_RES::ExecutionContextMetadata& rOperationMetadata) const;
    Result fillFindResourceByNameMetadata(const char* resourceUri, uint16& rResourceId) const;

    void fillResourceMetadata(
        LocalResourceId localResourceId,
        const ResourceMetadata& rMetadata,
        WB_RES::LocalOperationId operationIdList[MetadataHelpers::MAX_NUMBER_OF_OPERATIONS],
        WB_RES::ResourceMetadata& rResourceMetadata) const;

    const whiteboard::metadata::Operation* getOperation(
        LocalResourceId resourceId,
        WB_RES::OperationType operationType,
        ResourceMetadata& rMetadata,
        uint16& rOperationId) const;

    Result fillGetOperationMetadata(LocalResourceId resourceId,
                                    WB_RES::OperationType operationType,
                                    WB_RES::LocalParameterId parameterIdList[WB_MAX_NUMBER_OF_PARAMETERS],
                                    WB_RES::LocalResponseId responseIdList[WB_MAX_NUMBER_OF_RESPONSES],
                                    WB_RES::OperationMetadata& rOperationMetadata) const;

    Result fillGetResponseMetadata(LocalResourceId resourceId,
                                   WB_RES::OperationType operationType,
                                   uint8 responseIndex,
                                   WB_RES::ResponseMetadata& rResponseMetadata) const;

    Result fillGetOperationByIdMetadata(
        const MetadataMap& rMetadata,
        WB_RES::LocalOperationId operationId,
        WB_RES::LocalParameterId parameterIdList[WB_MAX_NUMBER_OF_PARAMETERS],
        WB_RES::LocalResponseId responseIdList[WB_MAX_NUMBER_OF_RESPONSES],
        WB_RES::OperationMetadata& rOperationMetadata) const;

    Result fillGetParameterMetadata(
        const MetadataMap& rMetadata,
        WB_RES::LocalOperationId operationId,
        uint8 parameterIndex,
        WB_RES::ParameterMetadata& rParameterMetadata) const;
    Result fillGetParameterByIdMetadata(
        const MetadataMap& rMetadata,
        WB_RES::LocalParameterId parameterId,
        WB_RES::ParameterMetadata& rParameterMetadata) const;
    Result fillGetResponseByIdMetadata(
        const MetadataMap& rMetadata,
        WB_RES::LocalResponseId responseId,
        WB_RES::ResponseMetadata& rResponseMetadata) const;
    Result fillGetSecurityTagByIdMetadata(
        const MetadataMap& rMetadata,
        WB_RES::SecurityTagId securityTagId,
        WB_RES::SecurityTagMetadata& rSecurityTagMetadata) const;
    Result fillGetStringByIdMetadata(
        const MetadataMap& rMetadata,
        WB_RES::StringId stringId,
        WB_RES::StringMetadata& rStringMetadata) const;

    void handleGetDataTypeByIdMetadata(
        const Request& rRequest, LocalDataTypeId dataTypeId, const MetadataMap& rMetadata);

    void handleGetMetadataStream(
        const internal::ResourceSubtree& rResourceSubtree,
        const MetadataMap& rMetadata,
        const Request& rRequest, RequestId originalRequestId);

    Result returnGetResourceByIdMetadata(
        const Request& rRequest,
        LocalResourceId localResourceId);

    Result returnGetOperationMetadata(
        const Request& rRequest,
        LocalResourceId localResourceId,
        WB_RES::OperationType operationType);

    Result returnGetOperationByIdMetadata(
        const MetadataMap& rMetadata,
        const Request& rRequest,
        WB_RES::LocalOperationId localOperationId);

    void returnDatatypeMetadataStructure(
        const MetadataMap& rMetadata,
        const Request& rRequest, 
        LocalDataTypeId dataTypeId,
        const metadata::DataType* pDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    void returnDatatypeMetadataEnum(
        const MetadataMap& rMetadata,
        const Request& rRequest, 
        LocalDataTypeId dataTypeId,
        const metadata::DataType* pDataType,
        WB_RES::DataTypeMetadata& rDataTypeMetadata);

    bool serializeMetadataToBuffer(
        const internal::ResourceSubtree& rResourceSubtree,
        const MetadataMap& rMetadata,
        uint8* buffer, size_t bufferSize, size_t& bytesWrittenToBuffer);

    bool handleGetDataTypeMetadataStream(
        const MetadataMap& rMetadata,
        const uint8* bufferStart, uint8*& bufferPos, size_t bufferSize, size_t dataTypeIndex);

    bool serializeItemCountIfNeeded(const uint8* pBufferStart, uint8*& pBufferPos, size_t bufferSize, size_t itemCount);

    struct WB_API StreamRequestState
    {
        enum State
        {
            STREAMING_EXECUTION_CONTEXT_METADATA,
            STREAMING_RESOURCE_METADATA,
            STREAMING_OPERATION_METADATA,
            STREAMING_RESPONSE_METADATA,
            STREAMING_PARAMETER_METADATA,
            STREAMING_DATATYPE_METADATA,
            STREAMING_SECURITYTAG_METADATA,
            STREAMING_STRING_METADATA,
            STREAMING_COMPLETED,
            NUM_STATES
        };

        State mState;
        size_t mIndex;
        uint16 mPrevStringId;
        RequestId mRequestId;
        TimerId mTimeoutTimerId;
        bool mLenSerialized;

        inline StreamRequestState()
            : mState(STREAMING_EXECUTION_CONTEXT_METADATA),
              mIndex(0),
              mPrevStringId(0),
              mRequestId(ID_INVALID_REQUEST),
              mTimeoutTimerId(ID_INVALID_TIMER),
              mLenSerialized(false)
        {
        }

        inline void reset()
        {
            mState = STREAMING_EXECUTION_CONTEXT_METADATA;
            mIndex = 0;
            mPrevStringId = 0;
            mRequestId = ID_INVALID_REQUEST;
            mTimeoutTimerId = ID_INVALID_TIMER;
            mLenSerialized = false;
        }

        inline void tryMoveToNextState(State currentState, size_t totalItemCount)
        {
            if (mIndex < totalItemCount)
            {
                return;
            }

            mIndex = 0;
            mPrevStringId = 0;
            mLenSerialized = false;

            switch (currentState)
            {
            case STREAMING_EXECUTION_CONTEXT_METADATA:
                //WB_DEBUGLOG("-->STREAMING_RESOURCE_METADATA");
                mState = STREAMING_RESOURCE_METADATA;
                break;
            case STREAMING_RESOURCE_METADATA:
                //WB_DEBUGLOG("-->STREAMING_OPERATION_METADATA");
                mState = STREAMING_OPERATION_METADATA;
                break;
            case STREAMING_OPERATION_METADATA:
                //WB_DEBUGLOG("-->STREAMING_RESPONSE_METADATA");
                mState = STREAMING_RESPONSE_METADATA;
                break;
            case STREAMING_RESPONSE_METADATA:
                //WB_DEBUGLOG("-->STREAMING_PARAMETER_METADATA");
                mState = STREAMING_PARAMETER_METADATA;
                break;
            case STREAMING_PARAMETER_METADATA:
                //WB_DEBUGLOG("-->STREAMING_DATATYPE_METADATA");
                mState = STREAMING_DATATYPE_METADATA;
                break;
            case STREAMING_DATATYPE_METADATA:
                //WB_DEBUGLOG("-->STREAMING_SECURITYTAG_METADATA");
                mState = STREAMING_SECURITYTAG_METADATA;
                break;
            case STREAMING_SECURITYTAG_METADATA:
                //WB_DEBUGLOG("-->STREAMING_STRING_METADATA");
                mState = STREAMING_STRING_METADATA;
                break;
            case STREAMING_STRING_METADATA:
                //WB_DEBUGLOG("-->STREAMING_COMPLETED");
                mState = STREAMING_COMPLETED;
                break;
            case STREAMING_COMPLETED: // break intentionally missing
            default:
                //WB_DEBUGLOG("-->STREAMING_EXECUTION_CONTEXT_METADATA");
                mState = STREAMING_EXECUTION_CONTEXT_METADATA;
                break;
            }
        }
    };

    StreamRequestState mStreamRequestState;

private:
    /** Prevent use of copy constructor */
    MetadataProvider(const MetadataProvider&) DELETED;

    /** Prevent use of assigment operator */
    const MetadataProvider& operator=(const MetadataProvider&) DELETED;

};

} // namespace services
} // namespace whiteboard
