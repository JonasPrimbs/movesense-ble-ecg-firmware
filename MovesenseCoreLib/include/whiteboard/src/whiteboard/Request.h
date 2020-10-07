#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Identifiers.h"
#include "whiteboard/metadata/MetadataStructures.h"

#ifdef WB_HAVE_BATCHING_PARAMETERS
#include "whiteboard/BatchingParameters.h"
#endif

namespace whiteboard
{

// Forward declarations
class RequestAccessor;

/** Request information structure */
class WB_API Request
{
public:
    // Request states
    enum RequestState
    {
        /** This Request instance is not owner of the request. Either instance has not been
        * initialized or it's ownership has been transferred to another Request object */
        NOT_OWNER,

        /** Request has not been handled yet */
        UNHANDLED,

        /** Request has been handled */
        HANDLED
    };

    /** Gets state of the request */
    inline RequestState getState() const
    {
        return static_cast<RequestState>(mState);
    }

    /** Gets type of the request */
    inline RequestType getRequestType() const
    {
        return static_cast<RequestType>(mRequestType);
    }

    /** Gets request ID of the request */
    inline RequestId getRequestId() const
    {
        return mRequestId;
    }

    /** Gets ID of the resource that should service this request */
    inline ResourceId getResourceId() const
    {
        return mResourceId;
    }

    /** DEPRECATED: This function will be removed in future Whiteboard versions. Use getResourceId().localResourceId for 
    * comparisons instead, because it produces more optimal code. For more information see Whiteboard best practices.
    *
    * Gets constant part of the resource ID that should service this request
    */
    inline ResourceId::Value getResourceConstId() const
    {
        return mResourceId.getConstId();
    }

    /** Gets ID of the client making the request */
    inline ClientId getClientId() const
    {
        return mClientId;
    }

    /** @return true if the request is an update that overrides previous similar request from the client.
    
    Currently used only if RequestType is REQUEST_SUBSCRIBE.
    
    @see whiteboard::RequestType
    */
    inline bool isUpdate() const
    {
        return mIsUpdate == 0 ? false : true;
    }

    /** @return true if request needs a response or not. */
    inline bool noRequestResponse() const
    {
        return mNoRequestResponse == 1 ? true : false;
    }

    /** @return true if request datatypes need to be forced to be sent as receivers datatype */
    inline bool forceReceiverDataType() const
    {
        return mForceReceiverDatatype == 1 ? true : false;
    }

#ifdef WB_HAVE_BATCHING_PARAMETERS
    /** @return optional batching parameters */
    inline const BatchingParameters& getBatchingParameters() const
    {
        return mBatchingParameters;
    }
#endif

    /** @return true if provider returned ok return code of returnResult */
    inline bool isSuccess() const
    {
        return  (mSuccess == 1) || (mState == NOT_OWNER);
    }

public:
    /** Default constructor */
    Request();

    /** Destructor */
    ~Request();

    /** Copy constructor 
     *
     * @note Requests have unique resource pointer semantics. When
     *       request is copied, ownership of the request is passed to
     *       a new instance. After copying old instance cannot be used
     *       in responses.
     *
     * @param rOther Request to copy
     */
    Request(const Request& rOther);

    /** Assingment operator
    *
    * @note Requests have unique resource pointer semantics. When
    *       request is copied, ownership of the request is passed to
    *       a new instance. After copying old instance cannot be used
    *       in responses.
    *
    * @param rOther Request to copy
    * @return Reference to this request object
    */
    Request& operator=(const Request& rOther);

private:
    /** Library internal implementation can access these members */
    friend class RequestAccessor;

    // State of the request
    mutable uint8 mState;

    // Type of the request
    uint8 mRequestType : 4;

    // Is this request update to the previous request
    uint8 mIsUpdate : 1;

    // No response required for the request
    uint8 mNoRequestResponse : 1;

    // Set receiver datatype bit
    uint8 mForceReceiverDatatype : 1;

    // Used currently in onSubscribe & returnResult to mark subscribe result successful by the provider
    uint8 mSuccess : 1;

    // Request ID of the request
    RequestId mRequestId;

    // ID of the resource that should service this request
    ResourceId mResourceId;

    // ID of the client making the request
    ClientId mClientId;

#ifdef WB_HAVE_BATCHING_PARAMETERS
    // Optional batching parameters
    BatchingParameters mBatchingParameters;
#endif
};

} // namespace whiteboard
