#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Identifiers.h"
#include "whiteboard/metadata/MetadataStructures.h"

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

    /** Gets constant part of the resource ID that should service this request */
    inline ResourceId::Value getResourceConstId() const
    {
        return mResourceId.getConstId();
    }

    /** Gets ID of the client making the request */
    inline ClientId getClientId() const
    {
        return mClientId;
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
    uint8 mRequestType;

    // Request ID of the request
    RequestId mRequestId;

    // ID of the resource that should service this request
    ResourceId mResourceId;

    // ID of the client making the request
    ClientId mClientId;
};

} // namespace whiteboard
