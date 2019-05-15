#pragma once

#include "app-resources/resources.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>

class HelloWorldService FINAL : private wb::ResourceProvider, public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    HelloWorldService();
    ~HelloWorldService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceProvider::onGetRequest */
    virtual void onGetRequest(const wb::Request& request,
                              const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onSubscribe */
    virtual void onSubscribe(const wb::Request& request,
                             const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onUnsubscribe */
    virtual void onUnsubscribe(const wb::Request& request,
                               const wb::ParameterList& parameters) OVERRIDE;

protected:
    /** @see whiteboard::ResourceProvider::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

private:
    wb::TimerId mTimer;
    uint32_t mCounter;
};
