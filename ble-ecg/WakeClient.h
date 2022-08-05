#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

// Timers:

/** Time interval between wake-up or lose of BLE connection and power off phase. */
const size_t SHUTDOWN_TIME_MS = 30000; // = 30 seconds.

/** Time interval after shutdown timer before power off. */
const size_t POWEROFF_TIME_MS = 3000; // = 3 second.

/** Time interval between two blinks to indicate aliveness. */
const size_t BLINK_PERIOD_MS = 5000;    // = 5 seconds.


// Wake Client:

class WakeClient FINAL :
    private wb::ResourceClient,
    public wb::LaunchableModule
{
public:

    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor for Wake Client. */
    WakeClient();
    /** Destructor for Wake Client. */
    ~WakeClient();

private:

    // Module:

    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId,
                          const wb::Value& rValue,
                          const wb::ParameterList& rParameters);


    /**
     * @brief Starts the timer to shut down the device.
     */
    void startShutdownTimer();
    /**
     * @brief Stops the timer to shut down the device.
     */
    void stopShutdownTimer();

    /**
     * @brief Starts the timer to power off the device.
     */
    void startPowerOffTimer();
    /**
     * @brief Stops the timer to power off the device.
     */
    void stopPowerOffTimer();

protected:

    /** @see whiteboard::ResourceClient::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

private:

    /** Timer ID for blinking. */
    wb::TimerId mBlinkTimer;

    /** Timer ID for shutdown. */
    wb::TimerId mShutdownTimer;

    /** Timer ID for power off. */
    wb::TimerId mPowerOffTimer;
};
