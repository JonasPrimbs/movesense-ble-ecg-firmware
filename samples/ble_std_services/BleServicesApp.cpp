#include "movesense.h"

#include "BleServicesApp.hpp"
#include "CustomBleController.hpp"
#include "common/core/debug.h"

#include "component_led/resources.h"
#include "meas_hr/resources.h"
#include "ui_ind/resources.h"
#include "component_max3000x/resources.h"
#include "system_mode/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "wb-resources/resources.h"

#include <float.h>
#include <math.h>


const char* const BleServicesApp::LAUNCHABLE_NAME = "BleServicesApp";

// Time between wake-up and going to power-off mode
#define AVAILABILITY_TIME 60000

// Time between turn on AFE wake circuit to power off
// (must be LED_BLINKING_PERIOD multiple)
#define WAKE_PREPARATION_TIME 5000

// LED blinking period in adertsing mode
#define LED_BLINKING_PERIOD 5000


BleServicesApp * BleServicesApp::spInstance = NULL;
CustomBleController Ctrl;


BleServicesApp::BleServicesApp()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),

      mDpc(WB_EXEC_CTX_APPLICATION, this, &BleServicesApp::dpcHandler),
      mTimer(whiteboard::ID_INVALID_TIMER),
      mHrsEnabled(false),
      mHrsEnableReq(false),
      mWbConnected(false),
      mCounter(0)
{
    spInstance = this;
}


BleServicesApp::~BleServicesApp()
{
}


bool BleServicesApp::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}


void BleServicesApp::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}


/** @see whiteboard::ILaunchableModule::startModule */
bool BleServicesApp::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    startShutdownTimer();

    // Subscribe to Whiteboard routing table changes
    asyncSubscribe(WB_RES::LOCAL::NET());

    return true;
}


/** @see whiteboard::ILaunchableModule::startModule */
void BleServicesApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer == whiteboard::ID_INVALID_TIMER;
}


void BleServicesApp::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                                   const whiteboard::ParameterList& parameters)
{
    // Heart rate notification
    if (resourceId.localResourceId == WB_RES::LOCAL::MEAS_HR::LID)
    {
        // Get average heart rate data
        uint16_t data = value.convertTo<const WB_RES::HRData&>().average;

        DEBUGLOG("HRS update: %d", data);
        send_ble_hr_measurement(data);
        return;
    }

    // WB routing table notification
    if (resourceId.localResourceId == WB_RES::LOCAL::NET::LID)
    {
        // Get whiteborad routing table notification
        uint8_t data = WB_RES::LOCAL::NET::EVENT::ParameterListRef(parameters)
            .getNotificationType();

        // if there is whiteboard connection, stop timer
        if (data == WB_RES::RoutingTableNotificationTypeValues::ROUTE_NOTIFICATION_NEW)
        {
            stopShutdownTimer();
            mWbConnected = true;
            return;
        }

        // if whiteboard connection lost, prepare to shutdown
        if (data == WB_RES::RoutingTableNotificationTypeValues::ROUTE_NOTIFICATION_LOST)
        {
            mWbConnected = false;
            if (!mHrsEnabled)
                startShutdownTimer();
        }

        return;
    }
}


void BleServicesApp::startShutdownTimer()
{
    DEBUGLOG("Start shutdown timer");

    if (mTimer != whiteboard::ID_INVALID_TIMER)
        stopTimer(mTimer);

    // Start timer
    mTimer = startTimer(LED_BLINKING_PERIOD, true);

    // Reset timeout counter
    mCounter = 0;
}


void BleServicesApp::stopShutdownTimer()
{
    DEBUGLOG("Stop shutdown timer");

    if (mTimer == whiteboard::ID_INVALID_TIMER)
        return;

    stopTimer(mTimer);
    mTimer = whiteboard::ID_INVALID_TIMER;
}


void BleServicesApp::onTimer(whiteboard::TimerId timerId)
{
    mCounter = mCounter + LED_BLINKING_PERIOD;

    if (mCounter < AVAILABILITY_TIME)
    {
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL::ID,
            AsyncRequestOptions::Empty, (uint16_t)2); // SHORT_VISUAL_INDICATION
        return;
    }

    if (mCounter < AVAILABILITY_TIME + WAKE_PREPARATION_TIME)
    {
        // Prepare AFE to wake-up mode
        asyncPut(WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP::ID,
            AsyncRequestOptions(NULL, 0, true), (uint8_t) 1);
        return;
    }

    // Make PUT request to switch LED on
    asyncPut(WB_RES::LOCAL::COMPONENT_LED::ID, AsyncRequestOptions::Empty, true);

    // Make PUT request to eneter power off mode
    asyncPut(WB_RES::LOCAL::SYSTEM_MODE::ID,
        AsyncRequestOptions(NULL, 0, true), // Force async
        (uint8_t)1U);                       // WB_RES::SystemMode::FULLPOWEROFF
}


void BleServicesApp::dpcHandler()
{
    if (mHrsEnableReq == mHrsEnabled)
        return;

    if (mHrsEnableReq)
    {
        this->stopShutdownTimer();
        asyncSubscribe(WB_RES::LOCAL::MEAS_HR());
    }
    else
    {
        asyncUnsubscribe(WB_RES::LOCAL::MEAS_HR());
        if (!mWbConnected)
            startShutdownTimer();
    }

    mHrsEnabled = mHrsEnableReq;
}
