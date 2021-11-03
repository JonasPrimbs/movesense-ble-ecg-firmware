#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/DpcFunctor.h>
#include "common/compiler/genType.h"

class SecureBleApp FINAL : private whiteboard::ResourceClient,
                           public whiteboard::LaunchableModule
{

public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    static SecureBleApp * spInstance;


    SecureBleApp();
    ~SecureBleApp();

protected:
    whiteboard::TimerId mTimer;

    uint32_t mCounter;
    bool mWbConnected;

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
    *	@param value Current value of the resource
    */
    virtual void onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                          const whiteboard::ParameterList& parameters);

    /**
    *	Prepare to shutdown and set timer
    */
    void startShutdownTimer();
    void stopShutdownTimer();

    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

    /**
    *	DPS callback - events handler for HRS BLE events
    */
    void dpcHandler();
};
