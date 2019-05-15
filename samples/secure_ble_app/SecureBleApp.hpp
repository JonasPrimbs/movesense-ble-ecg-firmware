#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/DpcFunctor.h>

class SecureBleApp FINAL : private wb::ResourceClient, public wb::LaunchableModule
{
friend class CustomBleController;

public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    static SecureBleApp * spInstance;
    static void queueDpc() { spInstance->mDpc.queue(false); }

    SecureBleApp();
    ~SecureBleApp();

protected:
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
    void startShutdownTimer();
    void stopShutdownTimer();

    /**
    *	DPS callback - events handler for HRS BLE events
    */
    void dpcHandler();

private:
    wb::TimerId mTimer;
    wb::DpcFunctor mDpc;
    uint32_t mCounter;
    bool mHrsEnabled;
    bool mHrsEnableReq;
    bool mWbConnected;
};
