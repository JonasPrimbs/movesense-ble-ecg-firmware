#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "wb-resources/resources.h"

class ConnectionReliabilityService FINAL : private wb::ResourceProvider, public wb::LaunchableModule
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
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceProvider::onSubscribe */
    virtual void onSubscribe(const wb::Request& request,
                             const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onUnsubscribe */
    virtual void onUnsubscribe(const wb::Request& request,
                               const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onRemoteWhiteboardDisconnected */
    virtual void onRemoteWhiteboardDisconnected(wb::WhiteboardId whiteboardId) OVERRIDE;
    /** @see whiteboard::ResourceProvider::onClientUnavailable */
    virtual void onClientUnavailable(wb::ClientId clientId) OVERRIDE;
    /** @see whiteboard::ResourceProvider::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

private:
    void stopRunning();
    void startRunning();

    wb::TimerId mCounterTimer;
    uint16_t mSubCounter;
    uint32_t mCounter;
};
