#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include <whiteboard/integration/os/shared/semaphorecxx11.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/DpcFunctor.h>
#include <whiteboard/comm/CommAdapter.h>
#include "wbcmd-resources/resources.h"

namespace wbcmd
{

/** Helper class for handling connection process to watch & simulator */
class ConnectionHelper final : private whiteboard::ResourceClient
{
public:
    /** Constructor */
    ConnectionHelper()
        : whiteboard::ResourceClient("NetClient", WB_EXEC_CTX_APPLICATION)
    {
    }

    /** Destructor */
    virtual ~ConnectionHelper()
    {
    }

public:
    /** Makes connection to a remote Whiteboard. This function enables given communication adapter
     * and waits until Whiteboard has made a connection.
     *
     * @param pAdapter Communication adapter to use
     * @return Result of the operation
     */
    std::pair<whiteboard::Result, wb::WhiteboardId> enableAdapterAndWaitForConnection(whiteboard::CommAdapter* pAdapter)
    {
        mSubscribeResult = NO_RESULT;
        mNotifyResult = NO_RESULT;

        whiteboard::ResourceId resourceId(WB_RES::LOCAL::NET::ID);
        whiteboard::Result result = whiteboard::DpcFunctor::syncQueueOnce(
            this->getExecutionContextId(), this, &ConnectionHelper::asyncSubscribeHelper, resourceId);
        if ((result != whiteboard::HTTP_CODE_OK) && (result != whiteboard::HTTP_CODE_ACCEPTED))
        {
            return std::make_pair(result, 0);
        }

        // Wait until request is ready
        mCompleted.wait();
        if (mSubscribeResult != whiteboard::HTTP_CODE_OK)
        {
            return std::make_pair(mSubscribeResult, 0);
        }

        // Enable communication interface
        result = pAdapter->enable();
        if (result != whiteboard::HTTP_CODE_OK)
        {
            return std::make_pair(result, 0);
        }

        // Wait until a notification from RoutingTableProvider
        mCompleted.try_wait_for(std::chrono::milliseconds(CONNECTION_TIMEOUT));
        if (mNotifyResult == NO_RESULT)
        {
            mNotifyResult = whiteboard::HTTP_CODE_GATEWAY_TIMEOUT;
        }

        return std::make_pair(mNotifyResult, mWhiteboardId);
    }

    std::string mWhiteboardAddress;

private:
    const whiteboard::Result NO_RESULT = static_cast<whiteboard::Result>(0);
    const size_t CONNECTION_TIMEOUT = 50000;

    /** Helper function that allows use of syncQueueOnce with asyncSubscribe */
    whiteboard::Result asyncSubscribeHelper(whiteboard::ResourceId resourceId)
    {
        return asyncSubscribe(resourceId);
    }

    /**
    *	Callback for asynchronous SUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    void onSubscribeResult(
        whiteboard::RequestId /*requestId*/,
        whiteboard::ResourceId /*resourceId*/,
        whiteboard::Result resultCode,
        const whiteboard::Value& /*rResultData*/) override
    {
        mSubscribeResult = resultCode;
        mCompleted.notify();
    }

    /**
    *	Callback for resource notifications.
    *
    *	@param resourceId Resource id associated with the update
    *	@param rValue Current value of the resource
    *	@param rParameters Notification parameters
    */
    void onNotify(
        whiteboard::ResourceId /*resourceId*/,
        const whiteboard::Value& rValue,
        const whiteboard::ParameterList& parameters) override
    {
        mNotifyResult = whiteboard::HTTP_CODE_OK;
        mWhiteboardAddress = rValue.convertTo<const char*>();
        mWhiteboardId = parameters[1].convertTo<uint8>();
        WB_DEBUGLOG("Connected remote whiteboard id %d", mWhiteboardId);
        mCompleted.notify();
    }

private:
    wb::cxx11::semaphore mCompleted;
    volatile whiteboard::Result mSubscribeResult;
    volatile whiteboard::Result mNotifyResult;
    volatile whiteboard::WhiteboardId mWhiteboardId;
};

}
