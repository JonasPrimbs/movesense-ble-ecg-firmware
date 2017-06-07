#include "movesense.h"

#include "PowerModesClient.h"
#include "common/core/debug.h"

#include "app-resources/resources.h"
#include "device_measurement_angularvelocity/resources.h"
#include "suunto_device_component_led/resources.h"
#include "suunto_device_measurement_linearacceleration/resources.h"
#include "suunto_device_measurement_magneticfield/resources.h"
#include "suunto_device_system_mode/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"

#include <float.h>
#include <math.h>

const size_t TIME_PER_POWERMODE_MS = 15000;
const size_t LED_BLINK_INTERVAL_MS = 400;

const char* const PowerModesClient::LAUNCHABLE_NAME = "PowMod";

#define SAMPLE_RATE 13

PowerModesClient::PowerModesClient()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION)
{
    mLedCounter = mPowerModeCounter = 0;
    mLedTimer = mPowerModeTimer = whiteboard::ID_INVALID_TIMER;
}

PowerModesClient::~PowerModesClient()
{
}

bool PowerModesClient::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void PowerModesClient::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

/** @see whiteboard::ILaunchableModule::startModule */
bool PowerModesClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Start timer. true = trigger repeatedly
    mPowerModeTimer = startTimer(TIME_PER_POWERMODE_MS, true);
    startLedBlinkSequence((int)mPowerModeCounter + 1, LED_BLINK_INTERVAL_MS);

    return true;
}

/** @see whiteboard::ILaunchableModule::startModule */
void PowerModesClient::stopModule()
{
    // Stop LED timer
    stopTimer(mPowerModeTimer);
    mPowerModeTimer == whiteboard::ID_INVALID_TIMER;
}

void PowerModesClient::startLedBlinkSequence(int flashTimes, int speedMs)
{
    mLedCounter = 0;
    mLedCount = flashTimes * 2;
    // Start LED timer. true = trigger repeatedly. Will be stopped in timer handler
    mLedTimer = startTimer(speedMs, true);
}

void PowerModesClient::enterPowerMode(POWER_MODE_e newMode)
{
    DEBUGLOG("enterPowerMode: mode: %d", newMode);
    switch (newMode)
    {
    case POWER_MODE_IDLE:
        // Do nothing
        break;
    case POWER_MODE_ACCELEROMETER:
        // subscribe to accelerometer
        asyncSubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, NULL, SAMPLE_RATE);
        break;

    case POWER_MODE_GYROSCOPE:
        // subscribe to accelerometer
        asyncSubscribe(WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::ID, NULL, SAMPLE_RATE);
        break;

    case POWER_MODE_MAGNETOMETER:
        // subscribe to accelerometer
        asyncSubscribe(WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::ID, NULL, SAMPLE_RATE);
        break;

    case POWER_MODE_POWER_OFF:
        asyncPut(WB_RES::LOCAL::DEVICE_SYSTEM_MODE::ID,
                 AsyncRequestOptions(NULL, 0, true), // Force async
                 (uint8_t)1U);                       // WB_RES::SystemMode::FULLPOWEROFF
        break;
    default:
        break;
    }
}

void PowerModesClient::leavePowerMode(POWER_MODE_e mode)
{
    DEBUGLOG("leavePowerMode: mode: %d", mode);
    whiteboard::RequestId remoteRequestId;
    switch (mode)
    {
    case POWER_MODE_IDLE:
        // Do nothing
        break;

    case POWER_MODE_ACCELEROMETER:
        // un-subscribe from accelerometer
        asyncUnsubscribe(WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID, NULL, SAMPLE_RATE);
        break;

    case POWER_MODE_GYROSCOPE:
        // un-subscribe from accelerometer
        asyncUnsubscribe(WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::ID, NULL, SAMPLE_RATE);
        break;

    case POWER_MODE_MAGNETOMETER:
        // un-subscribe from accelerometer
        asyncUnsubscribe(WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::ID, NULL, SAMPLE_RATE);
        break;

    case POWER_MODE_POWER_OFF:
        // Will never happen...
        break;
    default:
        break;
    }
}

void PowerModesClient::onTimer(whiteboard::TimerId timerId)
{
    if (timerId == mPowerModeTimer)
    {
        leavePowerMode((POWER_MODE_e)mPowerModeCounter);

        // increment counter and switch led on/off based on odd / evenness of it
        mPowerModeCounter++;

        // switch to next power mode happens when led blinking is finished
        startLedBlinkSequence((int)mPowerModeCounter + 1, LED_BLINK_INTERVAL_MS);
    }

    else if (timerId == mLedTimer)
    {
        bool bLedOn = (++mLedCounter & 0x1) != 0;

        // Make PUT request to switch on/off led. No need to handler result in onPutResult.
        asyncPut(WB_RES::LOCAL::DEVICE_COMPONENT_LED::ID, AsyncRequestOptions::Empty, bLedOn);

        if (mLedCounter >= mLedCount)
        {
            stopTimer(mLedTimer);
            mLedTimer = whiteboard::ID_INVALID_TIMER;
            mLedCounter = mLedCount = 0;

            // switch to next power mode
            enterPowerMode((POWER_MODE_e)mPowerModeCounter);
        }
    }
}

void PowerModesClient::onNotify(whiteboard::ResourceId resourceId,
                                const whiteboard::Value& rValue,
                                const whiteboard::ParameterList& rParameters)
{
    switch (resourceId.getConstId())
    {
    case WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::ID:
        DEBUGLOG("MEAS_ACC_SAMPLERATE notification.");
        break;
    case WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::ID:
        DEBUGLOG("MEAS_GYRO_SAMPLERATE notification.");
        break;
    case WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::ID:
        DEBUGLOG("MEAS_MAGN_SAMPLERATE notification.");
        break;

    default:
        break;
    }
}