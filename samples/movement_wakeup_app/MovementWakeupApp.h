#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class MovementWakeupApp FINAL : private wb::ResourceClient, public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    MovementWakeupApp();
    ~MovementWakeupApp();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId, const wb::Value& value,
                          const wb::ParameterList& parameters);

    /** @see whiteboard::ResourceClient::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

    /**
    *	Prepare to shutdown and set timer
    */
    void setShutdownTimer();

    wb::TimerId mTimer;
    uint32_t counter;
};
