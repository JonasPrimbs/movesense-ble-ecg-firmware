#include "movesense.h"

#include "BleServicesApp.hpp"

#include "common/core/debug.h"
#include "common/compiler/pack.h"

#include "component_led/resources.h"
#include "meas_hr/resources.h"
#include "ui_ind/resources.h"
#include "comm_ble/resources.h"
#include "comm_ble_hrs/resources.h"
#include "comm_ble_nus/resources.h"
#include "component_max3000x/resources.h"
#include "system_mode/resources.h"

const char* const BleServicesApp::LAUNCHABLE_NAME = "BleServicesApp";

// Time between wake-up and going to power-off mode
#define AVAILABILITY_TIME 60000

// Time between turn on AFE wake circuit to power off
// (must be LED_BLINKING_PERIOD multiple)
#define WAKE_PREPARATION_TIME 5000

// LED blinking period in adertsing mode
#define LED_BLINKING_PERIOD 5000

BleServicesApp* BleServicesApp::spInstance = NULL;

BleServicesApp::BleServicesApp()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      mTimer(wb::ID_INVALID_TIMER),
      mStates({0,0}),
      mCounter(0)
{
    ASSERT(NULL == spInstance);
    spInstance = this;
}

BleServicesApp::~BleServicesApp()
{
}

void BleServicesApp::hrsNotificationChanged(bool enabled)
{
    if (enabled == mStates.HrsEnabled) return;

    if (enabled)
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

    mStates.HrsEnabled = enabled;
}

void BleServicesApp::handleNusDataRx(const wb::Array<uint8_t> rxDataArray)
{
    // As a sample do ROT-13 on the data and send it back
    uint8_t buffer[160];
    for(size_t i=0; i<rxDataArray.size(); i++)
    {
        buffer[i] = (uint8_t)(((uint16_t)rxDataArray[i] + 13) & 0xFF);
    }

    WB_RES::NUSData dataOut;
    dataOut.bytes = wb::MakeArray<uint8_t>(buffer, rxDataArray.size());
    asyncPost(WB_RES::LOCAL::COMM_BLE_NUS(), AsyncRequestOptions::ForceAsync, dataOut);
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

void BleServicesApp::onPutResult(wb::RequestId requestId,
                                 wb::ResourceId resourceId,
                                 wb::Result resultCode,
                                 const wb::Value& result)
{
    debugOut(true, "BleServicesApp::onPutResult: %u, result: %u", resourceId.localResourceId, resultCode);
}

bool BleServicesApp::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    startShutdownTimer();

    // Subscribe to BLE peers list changes
    asyncSubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    // Subscribe to HRS state notifications
    asyncSubscribe(WB_RES::LOCAL::COMM_BLE_HRS());

    // Subscribe to incoming NUS data
    asyncSubscribe(WB_RES::LOCAL::COMM_BLE_NUS());

    return true;
}

void BleServicesApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;

    // un-Subscribe from incoming NUS data
    asyncSubscribe(WB_RES::LOCAL::COMM_BLE_NUS());

    // un-Subscribe from HRS state notifications
    asyncUnsubscribe(WB_RES::LOCAL::COMM_BLE_HRS());

    // un-Subscribe from BLE peers list changes
    asyncUnsubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}


void BleServicesApp::onNotify(wb::ResourceId resourceId,
                              const wb::Value& value,
                              const wb::ParameterList& parameters)
{
    // Heart rate notification
    switch(resourceId.localResourceId)
    {
    case WB_RES::LOCAL::MEAS_HR::LID:
        {
            // Get average heart rate data
            const WB_RES::HRData& hrdata = value.convertTo<const WB_RES::HRData&>();
            uint16_t hr = hrdata.average;

            DEBUGLOG("HRS update: %d, rr_count: %d", hr, hrdata.rrData.size());

            // Forward hr + rr data to HRS module
            WB_RES::HRSData hrsData;
            hrsData.hr = hr;
            if (hrdata.rrData.size()>0)
            {
                hrsData.rr = wb::MakeArray<uint16_t>(&(hrdata.rrData[0]), hrdata.rrData.size());
            }
            asyncPost(WB_RES::LOCAL::COMM_BLE_HRS(), AsyncRequestOptions::Empty, hrsData);
        }
        break;

    // BLE connection notification
    case WB_RES::LOCAL::COMM_BLE_PEERS::LID:
        {
            // Get whiteborad routing table notification
            uint8_t peerState = value.convertTo<const WB_RES::PeerChange&>().state;
            DEBUGLOG("COMM_BLE_PEERS: peerState: %d", peerState);

            switch (peerState)
            {
            case WB_RES::PeerStateValues::CONNECTED:
                // if there is BLE connection, stop timer
                stopShutdownTimer();
                // Start listening to Nordic UART data
                asyncSubscribe(WB_RES::LOCAL::COMM_BLE_NUS());
                mStates.PeerConnected = 1;

                break;

            case WB_RES::PeerStateValues::DISCONNECTED:
                // if BLE connection is lost, prepare to shutdown
                // start the shut down process even if HrsEnabled is true, connection may be dropped
                startShutdownTimer();
                mStates.PeerConnected = 0;
                break;

            default:
                break;
            }
        }
        break;

    // HRS state change notification
    case WB_RES::LOCAL::COMM_BLE_HRS::LID:
        {
            // Get whiteborad routing table notification
            const bool hrsNotifEnabled = value.convertTo<const WB_RES::HRSState&>().notificationEnabled;
            DEBUGLOG("COMM_BLE_HRS: hrsNotifEnabled: %d", hrsNotifEnabled);

            hrsNotificationChanged(hrsNotifEnabled);
        }
        break;

    // NUS data notification
    case WB_RES::LOCAL::COMM_BLE_NUS::LID:
        {
            // Get whiteborad routing table notification
            auto receivedDataArray = value.convertTo<const WB_RES::NUSData&>().bytes;
            DEBUGLOG("COMM_BLE_NUS: %d bytes received.", receivedDataArray.size());

            handleNusDataRx(receivedDataArray);
        }
        break;
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
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty,
                 WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION);
        return;
    }

    const AsyncRequestOptions reqOptsForceAsync(NULL, 0, true);

    if (mCounter < AVAILABILITY_TIME + WAKE_PREPARATION_TIME)
    {
        // Prepare AFE to wake-up mode
        asyncPut(WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP(), reqOptsForceAsync, (uint8_t)1u);
        return;
    }

    // Make PUT request to switch LED on
    asyncPut(WB_RES::LOCAL::COMPONENT_LED(), AsyncRequestOptions::Empty, true);

    // Make PUT request to eneter power off mode
    asyncPut(WB_RES::LOCAL::SYSTEM_MODE(), reqOptsForceAsync, WB_RES::SystemMode::FULLPOWEROFF);
}
