#include "movesense.h"

#include "BleServicesApp.hpp"
#include "common/core/debug.h"
#include "common/compiler/pack.h"

#include "component_led/resources.h"
#include "meas_hr/resources.h"
#include "ui_ind/resources.h"
#include "comm_ble/resources.h"
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

// Replace BLE Advertise data with one that has HRS serviceUUID listed
// (Some apps require it)

#define ADVERT_FLAGS 0x06

#define MANUFACTURER_NAME "Suunto"
#define BLE_SUUNTO_COMPANY_ID 0x009f

// Whiteboard interface UUID: 61353090-8231-49CC-B57A-886370740041
#define WhiteboardUUID                                                                                                           \
    {                                                                                                                            \
        0x41, 0x00, 0x74, 0x70, 0x63, 0x88, 0x7A, 0xB5, 0xCC, 0x49, 0x31, 0x82, 0x90, 0x30, 0x35, 0x61                           \
    }

#define StandardHrsUUID                                                                                                          \
    {                                                                                                                            \
        0x0D, 0x18                                                                                                               \
    }

#define BLE_GAP_AD_TYPE_FLAGS 0x01
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE 0x02
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE 0x03
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE 0x06
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE 0x07
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME 0x09
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA 0xFF

PACK_BEGIN();

struct HrsAdvPacket
{
    struct Flags
    {
        const uint8_t len = sizeof(Flags) - 1;
        const uint8_t id = BLE_GAP_AD_TYPE_FLAGS;
        const uint8_t flags = ADVERT_FLAGS;
    } flags;
    struct ManufData
    {
        const uint8_t len = sizeof(ManufData) - 1;
        const uint8_t id = BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA;
        const uint16_t manufId = BLE_SUUNTO_COMPANY_ID;
        const uint8_t data[1] = {0xFF};
    } manuf;
    struct HRSvc
    {
        const uint8_t len = sizeof(HRSvc) - 1;
        const uint8_t id = BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE;
        const uint8_t svcId[2] = StandardHrsUUID;
    } hrsUuid;
    struct WBSvc
    {
        const uint8_t len = sizeof(WBSvc) - 1;
        const uint8_t id = BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE;
        const uint8_t svcId[16] = WhiteboardUUID;
    } wbUuid;
};

PACK_END();

// Make sure adv packet is not too long (max 31 bytes)
STATIC_VERIFY(sizeof(HrsAdvPacket) <= 31, sizeof(HrsAdvPacket));

BleServicesApp* BleServicesApp::spInstance = NULL;

BleServicesApp::BleServicesApp()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      bleCtrl(),
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
    spInstance->mStates.HrsEnableReq = state ? 1 : 0;
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

void BleServicesApp::setBleAdvPacket()
{
    // Replace advPacket with one containing HRS UUID
    // Leave ScanResp as is (contains product name)
    const HrsAdvPacket advPacket;
    const uint8_t* advBytes = reinterpret_cast<const uint8_t*>(&advPacket);
    for (int i = 0; i < sizeof(advPacket); i++)
    {
        DEBUGLOG("Adv #%d: 0x%02X", i, advBytes[i]);
    }
    WB_RES::AdvSettings advSettings;
    advSettings.advPacket = wb::MakeArray<uint8_t>(advBytes, sizeof(advPacket));
    advSettings.interval = 800; // 500ms in 0.625ms BLE ticks
    advSettings.timeout = 0;    // Advertise forever

    asyncPut(WB_RES::LOCAL::COMM_BLE_ADV_SETTINGS(), AsyncRequestOptions(NULL, 0, true), advSettings);
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

    // Set modified BLE Advertising pkg
    setBleAdvPacket();
    return true;
}

void BleServicesApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void BleServicesApp::onNotify(wb::ResourceId resourceId,
                              const wb::Value& value,
                              const wb::ParameterList& parameters)
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
