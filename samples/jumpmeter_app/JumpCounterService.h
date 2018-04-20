#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "wb-resources/resources.h"

class JumpCounterService FINAL : private whiteboard::ResourceClient,
                                 private whiteboard::ResourceProvider,
                                 public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    JumpCounterService();
    ~JumpCounterService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    /**
    *	GET request handler.
    *
    *	@param requestId ID of the request
    *	@param clientId ID of the client that should receive the result
    *	@param resourceId ID of the associated resource
    *	@param parameters List of parameters for the request
    *	@return Result of the operation
    */
    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *	Subscribe notification callback.
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    virtual void onSubscribe(const whiteboard::Request& request,
                             const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *	Unsubscribe notification callback.
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    virtual void onUnsubscribe(const whiteboard::Request& request,
                               const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *  Whiteboard disconnect notification handler.
    *
    *  This can be used for example to cleanup possible subscription related information of clients from
    *  the remote whiteboard.
    *
    *  @param whiteboardId ID of the whiteboard that has been disconnected.
    *
    *  @see whiteboard::ResourceProvider::onSubscribe
    *  @see whiteboard::ResourceProvider::onUnsubscribe
    */
    virtual void onRemoteWhiteboardDisconnected(whiteboard::WhiteboardId whiteboardId) OVERRIDE;
    /**
    * Local client 'disconnect' notification handler.
    *
    *  This can be used for example to cleanup possible subscription related information of the client.
    *
    *  @see whiteboard::ResourceProvider::onSubscribe
    *  @see whiteboard::ResourceProvider::onUnsubscribe
    */
    virtual void onClientUnavailable(whiteboard::ClientId clientId) OVERRIDE;

    /**
    *	Callback for resource notifications.
    *   Note that this function will not be called for notifications that are
    *   of types WB_RESOURCE_NOTIFICATION_TYPE_INSERT or WB_RESOURCE_NOTIFICATION_TYPE_DELETE,
    *   just for notifications that are of type WB_RESOURCE_NOTIFICATION_TYPE_UPDATE.
    *
    *	@param resourceId Resource id associated with the update
    *	@param rValue Current value of the resource
    */
    virtual void onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                          const whiteboard::ParameterList& parameters);

    /**
    *	Callback for asynchronous UNSUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onUnsubscribeResult(whiteboard::RequestId requestId,
                                     whiteboard::ResourceId resourceId,
                                     whiteboard::Result resultCode,
                                     const whiteboard::Value& rResultData);
    /**
    *	Callback for asynchronous SUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onSubscribeResult(whiteboard::RequestId requestId,
                                   whiteboard::ResourceId resourceId,
                                   whiteboard::Result resultCode,
                                   const whiteboard::Value& rResultData);

private:
    whiteboard::Result startRunning(whiteboard::RequestId& remoteRequestId);
    whiteboard::Result stopRunning();

    void jumpFound(size_t nJumpSamples);
    whiteboard::ResourceId	mMeasAccResourceId;

    whiteboard::RequestMap<2, void> mOngoingRequests; // For storing relations of incoming & outgoing requests

    bool isRunning;
    float mLastJumpHeight;
    uint16_t mJumpCounter;
    uint16_t mSamplesSince0GStart;
};
