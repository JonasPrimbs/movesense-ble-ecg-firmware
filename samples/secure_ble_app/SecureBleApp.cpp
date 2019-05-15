#include "movesense.h"

#include "SecureBleApp.hpp"
#include "CustomBleController.hpp"
#include "common/core/debug.h"

#include "component_led/resources.h"
#include "component_max3000x/resources.h"
#include "meas_hr/resources.h"
#include "ui_ind/resources.h"
#include "system_mode/resources.h"

const char* const SecureBleApp::LAUNCHABLE_NAME = "SecureBleApp";

// Time between wake-up and going to power-off mode
#define AVAILABILITY_TIME 60000

// Time between turn on AFE wake circuit to power off
// (must be LED_BLINKING_PERIOD multiple)
#define WAKE_PREPARATION_TIME 5000

// LED blinking period in adertsing mode
#define LED_BLINKING_PERIOD 5000

SecureBleApp * SecureBleApp::spInstance = NULL;
CustomBleController Ctrl;

SecureBleApp::SecureBleApp():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mTimer(wb::ID_INVALID_TIMER),
    mDpc(WB_EXEC_CTX_APPLICATION, this, &SecureBleApp::dpcHandler),
    mCounter(0),
    mHrsEnabled(false),
    mHrsEnableReq(false),
    mWbConnected(false)
{
    ASSERT(NULL == spInstance);
    spInstance = this;
}

SecureBleApp::~SecureBleApp()
{
}

bool SecureBleApp::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void SecureBleApp::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool SecureBleApp::startModule()
{
    startShutdownTimer();

    // Subscribe to Whiteboard routing table changes
    asyncSubscribe(WB_RES::LOCAL::NET());

    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void SecureBleApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void SecureBleApp::onNotify(wb::ResourceId resourceId,
                            const wb::Value& value,
                            const wb::ParameterList& parameters)
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
    else if (resourceId.localResourceId == WB_RES::LOCAL::NET::LID)
    {
        // Get whiteborad routing table notification
        uint8_t data = WB_RES::LOCAL::NET::EVENT::ParameterListRef(parameters).getNotificationType();

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

void SecureBleApp::startShutdownTimer()
{
    DEBUGLOG("Start shutdown timer");

    if (mTimer != wb::ID_INVALID_TIMER)
        stopTimer(mTimer);

    // Start timer
    mTimer = startTimer(LED_BLINKING_PERIOD, true);

    // Reset timeout counter
    mCounter = 0;
}

void SecureBleApp::stopShutdownTimer()
{
    DEBUGLOG("Stop shutdown timer");

    if (mTimer == wb::ID_INVALID_TIMER)
        return;

    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
}

void SecureBleApp::onTimer(wb::TimerId timerId)
{
//    mCounter = mCounter + LED_BLINKING_PERIOD;

    if (mCounter < AVAILABILITY_TIME)
    {
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty,
                 WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION);
        return;
    }

    if (mCounter < AVAILABILITY_TIME + WAKE_PREPARATION_TIME)
    {
        // Prepare AFE to wake-up mode
        asyncPut(WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP(), AsyncRequestOptions(NULL, 0, true), (uint8_t) 1);
        return;
    }

    // Make PUT request to switch LED on
    asyncPut(WB_RES::LOCAL::COMPONENT_LED(), AsyncRequestOptions::Empty, true);

    // Make PUT request to eneter power off mode
    asyncPut(WB_RES::LOCAL::SYSTEM_MODE(), AsyncRequestOptions(NULL, 0, true), // true = Force async
             WB_RES::SystemModeValues::FULLPOWEROFF);
}

void SecureBleApp::dpcHandler()
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
