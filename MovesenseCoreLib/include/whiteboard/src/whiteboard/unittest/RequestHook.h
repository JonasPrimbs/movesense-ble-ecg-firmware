#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#ifndef WB_UNITTEST_BUILD
#error Include only in unit test build
#endif

#include "whiteboard/Identifiers.h"
#include "whiteboard/Request.h"
#include "whiteboard/Result.h"
#include "whiteboard/Value.h"
#include "whiteboard/ValueStorage.h"

namespace whiteboard
{

/**
    Structure for cleanly specifying RequestId ranges for request hook filtering in unit tests.
*/
struct WB_API RequestIdRange
{
    static
#ifndef ARCH_HEXAGON
    // Hexagon's 16-bit absolute addresses are not enough to address global constants
    const
#endif
    RequestIdRange ANY;

    /// Start of the specified range, ID_INVALID_REQUEST indicates no low limit
    RequestId mFirst;
    /// End of the specified range, ID_INVALID_REQUEST indicates no high limit
    RequestId mLast;
};

/**
    Unit test helper structure to define request hooks for overriding specific responses to the client
    with the specified response Result and Value pair.
*/
struct RequestHookDef
{
    /// Type of request to match, REQUEST_NOTYPE to match any request type
    RequestType mReqType;           // used as filter
    ///  Resource to match, ID_INVALID_RESOURCE to match any resource
    ResourceId mResourceId;         // used as filter
    /// Request ID range to match, ID_INVALID_REQUEST indicates any / no limit
    RequestIdRange mRequestIdRange; // used as filter
    /// Result code to be returned from the async request method upon hook match
    Result mCallResult;             // used to replace the sync result code of a request matching the hook
    /// Result code to be given to the client async result/response callback upon hook match
    Result mReplacerResult;         // used to replace the async response result code of a request matching the hook
    /// Value data to be given to the client async result/response callback upon hook match
    ValueStorage mReplacerValue;    // used to replace the response data of a request matching the hook
};

/**
    Unit test helper structure for storing response information between the synchronous call to the async
    request method and the actual passing of the overriding response data to the client's callback.
*/
struct RequestHookParams
{
    RequestHookParams(const RequestType type, const RequestId reqId, const ResourceId rscId, const Result resCode, const RequestHookDef* pHook) :
        reqType(type), requestId(reqId), resourceId(rscId), resultCode(resCode), pHookMatch(pHook)
    {
    }

    /// stored type of the overridden request response
    RequestType reqType;
    /// request ID of the request for which the response is overridden
    RequestId requestId;
    /// resource ID of the overridden request response
    ResourceId resourceId;
    /// Result code that was specified in the hook for the async response
    Result resultCode;
    /// Pointer to the hook definition that resulted in the override
    const RequestHookDef* pHookMatch;
};

}
