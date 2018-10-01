#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/DpcFunctor.h>

class BleServicesApp FINAL : private wb::ResourceClient, public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Sets the HR service request active or inactive
    *   @param state True if service activation requested, false if cancelled.
    */
    static void setNotificationRequest(bool state);

    BleServicesApp();
    ~BleServicesApp();

protected:
    wb::DpcFunctor mDpc;
    wb::TimerId mTimer;

    uint32_t mCounter;
    struct InternalStates
    {
        uint16_t HrsEnabled    : 1;
        uint16_t HrsEnableReq  : 1;
        uint16_t PeerConnected : 1;
    };
    InternalStates mStates;
 
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
    virtual void onNotify(wb::ResourceId resourceId, const wb::Value& value, const wb::ParameterList& parameters);

    /** Prepare to shutdown and set timer */
    void startShutdownTimer();

    /** Cancel shutdown and the timer if active */
    void stopShutdownTimer();

    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

    /**
    *	DPS callback - events handler for HRS BLE events
    */
    void dpcHandler();

private:
    static BleServicesApp* spInstance;
};
