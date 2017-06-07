#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Request.h"

namespace whiteboard
{

/** Wrapper class to provide a more priviledged access to Request class */
class RequestAccessor
{
public:
    /** Initializes a new instance of the Request class
    *
    * @param requestType Type of the request
    * @param requestId ID of the client's request
    * @param clientId ID of the client making the request
    * @param resourceId ID of the resource that should service this request
    * @return New Request object
    */
    inline static Request makeRequest(RequestType requestType, RequestId requestId, ClientId clientId, ResourceId resourceId);

    /** Sets state of the Request to handled
    *
    * @param rRequest The request to access
    */
    inline static void setHandled(const Request& rRequest);

    /** Sets request ID of the Request
    *
    * @param rRequest The request to access
    * @param requestId New ID for the request
    */
    inline static void setRequestId(Request& rRequest, RequestId requestId);

    /** Sets resource ID associated with the Request
    *
    * @param rRequest The request to access
    * @param resourceId New ID for the resource
    */
    inline static void setResourceId(const Request& rRequest, ResourceId resourceId);

    /** Sets request updateType of the Request
    *
    * @param rRequest The request to access
    * @param isUpdate New update type of this request
    */
    inline static void setUpdate(const Request& rRequest, bool isUpdate);
};

inline Request RequestAccessor::makeRequest(RequestType requestType, RequestId requestId, ClientId clientId, ResourceId resourceId)
{
    Request request;
    request.mState = Request::UNHANDLED;
    request.mRequestType = requestType;
    request.mIsUpdate = 0;
    request.mReserved = 0;
    request.mRequestId = requestId;
    request.mClientId = clientId;
    request.mResourceId = resourceId;
    return request;
}

inline void RequestAccessor::setHandled(const Request& rRequest)
{
    // Request should be unhandled i.e. do not call returnResult twice (mState == HANDLED) 
    // or do not first copy request and then use old instance to call returnResult (mState == NOT_OWNER)
    if (rRequest.mState != Request::UNHANDLED)
    {
        const ClientId clientId = rRequest.getClientId();
        WB_SYSEVENT_API_ERROR("setHandled: %s: res=%u, cli=%u@wb=%u",
            (rRequest.mState == Request::HANDLED) ? "already handled" : "not owner",
            rRequest.getResourceId().localResourceId,
            clientId.localClientId,
            clientId.whiteboardId);
        WB_ASSERT(false);
    }
    rRequest.mState = Request::HANDLED;
}

inline void RequestAccessor::setRequestId(Request& rRequest, RequestId requestId)
{
    rRequest.mRequestId = requestId;
}

inline void RequestAccessor::setResourceId(const Request& rRequest, ResourceId resourceId)
{
    const_cast<Request&>(rRequest).mResourceId = resourceId;
}

inline void RequestAccessor::setUpdate(const Request& rRequest, bool isUpdate)
{
    const_cast<Request&>(rRequest).mIsUpdate = isUpdate ? 1 : 0;
}

} // namespace whiteboard
