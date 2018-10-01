#include "movesense.h"

#include "BleServicesApp.hpp"
#include "CustomBleController.hpp"
#include "common/core/debug.h"

#include "component_led/resources.h"
#include "meas_hr/resources.h"
#include "ui_ind/resources.h"
#include "comm_ble/resources.h"
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


BleServicesApp* BleServicesApp::spInstance = NULL;
CustomBleController Ctrl;


BleServicesApp::BleServicesApp()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      mDpc(WB_EXEC_CTX_APPLICATION, this, &BleServicesApp::dpcHandler),
      mTimer(wb::ID_INVALID_TIMER),
      mStates{0},
      mCounter(0)
{
    ASSERT(NULL == spInstance);
    spInstance = this;
}


BleServicesApp::~BleServicesApp()
{
}

// static method
void BleServicesApp::setNotificationRequest(bool state)
{
    spInstance->mStates.HrsEnableReq = state ? 1:0;
    spInstance->mDpc.queue(false);
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

    // Subscribe to BLE peers list changes
    asyncSubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    return true;
}


/** @see whiteboard::ILaunchableModule::startModule */
void BleServicesApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
}


void BleServicesApp::onNotify(wb::ResourceId resourceId, const wb::Value& value, const wb::ParameterList& parameters)
{
    // Heart rate notification
    if (resourceId.localResourceId == WB_RES::LOCAL::MEAS_HR::LID)
    {
        // Get average heart rate data
        const WB_RES::HRData& hrdata = value.convertTo<const WB_RES::HRData&>();
        uint16_t hr = hrdata.average;

        DEBUGLOG("HRS update: %d, rr_count: %d", hr, hrdata.rrData.size());
        send_ble_hr_measurement(hr, &(hrdata.rrData[0]), hrdata.rrData.size());
        return;
    }

    // WB routing table notification
    if (resourceId.localResourceId == WB_RES::LOCAL::COMM_BLE_PEERS::LID)
    {
        // Get whiteborad routing table notification
        uint8_t peerState = value.convertTo<const WB_RES::PeerChange&>().state;
        DEBUGLOG("COMM_BLE_PEERS: peerState: %d", peerState);

        switch (peerState)
        {
        case WB_RES::PeerStateValues::CONNECTED:
            // if there is BLE connection, stop timer
            stopShutdownTimer();
            mStates.PeerConnected = 1;
            break;

        case WB_RES::PeerStateValues::DISCONNECTED:
            // if BLE connection is lost, prepare to shutdown
            // start the shut down process even if HrsEnabled is true, connection may be dropped
            startShutdownTimer();
            mStates.PeerConnected = 0;
            break;

        default:
            ASSERT(0);
        }
    }
}


void BleServicesApp::startShutdownTimer()
{
    DEBUGLOG("Start shutdown timer");

    if (mTimer != wb::ID_INVALID_TIMER)
    {
        stopTimer(mTimer);
    }

    // Start timer
    mTimer = startTimer(LED_BLINKING_PERIOD, true);

    // Reset timeout counter
    mCounter = 0;
}


void BleServicesApp::stopShutdownTimer()
{
    if (mTimer == wb::ID_INVALID_TIMER) return;
    DEBUGLOG("Stop shutdown timer");

    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
}


void BleServicesApp::onTimer(wb::TimerId timerId)
{
    mCounter = mCounter + LED_BLINKING_PERIOD;

    if (mCounter < AVAILABILITY_TIME)
    {
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, (uint16_t) 2u); // SHORT_VISUAL_INDICATION
        return;
    }

    const AsyncRequestOptions reqOptsForceAsync(NULL, 0, true);

    if (mCounter < AVAILABILITY_TIME + WAKE_PREPARATION_TIME)
    {
        // Prepare AFE to wake-up mode
        asyncPut(WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP(), reqOptsForceAsync, (uint8_t) 1u);
        return;
    }

    // Make PUT request to switch LED on
    asyncPut(WB_RES::LOCAL::COMPONENT_LED(), AsyncRequestOptions::Empty, true);

    // Make PUT request to eneter power off mode
    asyncPut(WB_RES::LOCAL::SYSTEM_MODE(), reqOptsForceAsync, WB_RES::SystemMode::FULLPOWEROFF);
}


void BleServicesApp::dpcHandler()
{
    if (mStates.HrsEnableReq == mStates.HrsEnabled) return;

    if (mStates.HrsEnableReq)
    {
        stopShutdownTimer();
        asyncSubscribe(WB_RES::LOCAL::MEAS_HR());
    }
    else
    {
        asyncUnsubscribe(WB_RES::LOCAL::MEAS_HR());
        if (!mStates.PeerConnected)
        {
            startShutdownTimer();
        }
    }

    mStates.HrsEnabled = mStates.HrsEnableReq;
}
