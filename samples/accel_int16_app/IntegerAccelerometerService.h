#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "wb-resources/resources.h"
#include "ResourceForwardingTracker.h"

class IntegerAccelerometerService FINAL : private wb::ResourceClient, private wb::ResourceProvider, public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    IntegerAccelerometerService();
    ~IntegerAccelerometerService();

private:
    /** @see wb::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see wb::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see wb::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see wb::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    /** asyncGet(...) response handling, @see wb::ResourceClient::onGetResult */
    virtual void onGetResult(wb::RequestId requestId, wb::ResourceId resourceId, wb::Result resultCode,
                             const wb::Value& rResultData) OVERRIDE;

    /**
    *	Subscribe notification callback.
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    virtual void onSubscribe(const wb::Request& request,
                             const wb::ParameterList& parameters) OVERRIDE;

    /**
    *	Unsubscribe notification callback.
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    virtual void onUnsubscribe(const wb::Request& request,
                               const wb::ParameterList& parameters) OVERRIDE;

    /**
    *  Whiteboard disconnect notification handler.
    *
    *  This can be used for example to cleanup possible subscription related information of clients from
    *  the remote whiteboard.
    *
    *  @param whiteboardId ID of the whiteboard that has been disconnected.
    *
    *  @see wb::ResourceProvider::onSubscribe
    *  @see wb::ResourceProvider::onUnsubscribe
    */
    virtual void onRemoteWhiteboardDisconnected(wb::WhiteboardId whiteboardId) OVERRIDE;
    /**
    * Local client 'disconnect' notification handler.
    *
    *  This can be used for example to cleanup possible subscription related information of the client.
    *
    *  @see wb::ResourceProvider::onSubscribe
    *  @see wb::ResourceProvider::onUnsubscribe
    */
    virtual void onClientUnavailable(wb::ClientId clientId) OVERRIDE;

    /**
    *	Callback for resource notifications.
    *   Note that this function will not be called for notifications that are
    *   of types WB_RESOURCE_NOTIFICATION_TYPE_INSERT or WB_RESOURCE_NOTIFICATION_TYPE_DELETE,
    *   just for notifications that are of type WB_RESOURCE_NOTIFICATION_TYPE_UPDATE.
    *
    *	@param resourceId Resource id associated with the update
    *	@param rValue Current value of the resource
    */
    virtual void onNotify(wb::ResourceId resourceId, const wb::Value& value,
                          const wb::ParameterList& parameters);

    /**
    *	Callback for asynchronous UNSUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onUnsubscribeResult(wb::RequestId requestId,
                                     wb::ResourceId resourceId,
                                     wb::Result resultCode,
                                     const wb::Value& rResultData);
    /**
    *	Callback for asynchronous SUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onSubscribeResult(wb::RequestId requestId,
                                   wb::ResourceId resourceId,
                                   wb::Result resultCode,
                                   const wb::Value& rResultData);

private:
    ResourceForwardingTracker mFwdTracker;
    wb::RequestMap<2, void> mOngoingRequests; // For storing relations of incoming & outgoing requests
    float mScaler;
};
