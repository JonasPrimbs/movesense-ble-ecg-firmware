#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class HrWakeupApp FINAL : private whiteboard::ResourceClient,
                           public whiteboard::LaunchableModule
{

public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    HrWakeupApp();
    ~HrWakeupApp();

private:
    whiteboard::TimerId mTimer;
    uint32_t counter;

    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

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
    *	Prepare to shutdown and set timer
    */
    void setShutdownTimer();

    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

};
