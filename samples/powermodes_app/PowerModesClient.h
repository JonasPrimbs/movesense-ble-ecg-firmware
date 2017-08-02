#pragma once

#include "app-resources/resources.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

typedef enum {
    POWER_MODE_IDLE = 0,
    POWER_MODE_SLOW_ADVERTISE,
    POWER_MODE_FAST_ADVERTISE,
    POWER_MODE_ACCEL_13,
    POWER_MODE_ACCEL_104,
    POWER_MODE_ACCEL_833,
    POWER_MODE_GYRO_13,
    POWER_MODE_GYRO_104,
    POWER_MODE_GYRO_833,
    POWER_MODE_MAGN_13,
    POWER_MODE_MAGN_104,
    POWER_MODE_MAGN_833,
    POWER_MODE_POWER_OFF, // TODO: move last!
} POWER_MODE_e;

class PowerModesClient FINAL : private whiteboard::ResourceClient,
                               public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    PowerModesClient();
    ~PowerModesClient();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

protected:
    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

    /**
    *	Callback for resource notifications.
    *
    *	@param resourceId Resource id associated with the update
    *	@param rValue Current value of the resource
    *	@param rParameters Notification parameters
    */
    virtual void onNotify(whiteboard::ResourceId resourceId,
                          const whiteboard::Value& rValue,
                          const whiteboard::ParameterList& rParameters) OVERRIDE FINAL;

private:
    void startLedBlinkSequence(int flashTimes, int speedMs);
    void leavePowerMode(POWER_MODE_e mode);
    void enterPowerMode(POWER_MODE_e newMode);

    // Sample rate & sample count test
    int32_t mSampleRate;
    int32_t mSampleCount;

    // Power mode logic
    volatile whiteboard::TimerId mPowerModeTimer;
    uint32_t mPowerModeCounter;

    // Led blinker
    volatile whiteboard::TimerId mLedTimer;
    uint32_t mLedCounter;
    uint32_t mLedCount;
};
