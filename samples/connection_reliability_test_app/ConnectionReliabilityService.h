#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "wb-resources/resources.h"

class ConnectionReliabilityService FINAL : private whiteboard::ResourceProvider,
                                 public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    ConnectionReliabilityService();
    ~ConnectionReliabilityService();

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

    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

private:
    void stopRunning();
    void startRunning();

    uint16_t mSubCounter;
    whiteboard::TimerId mCounterTimer;
    uint32_t mCounter;
};
