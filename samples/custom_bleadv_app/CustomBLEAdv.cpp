#include "movesense.h"

#include "CustomBLEAdv.h"
#include "common/core/debug.h"

#include "comm_ble/resources.h"

const int DATA_UPDATE_PERIOD_MS = 10000;

// Custom BLE Advertising sample
// This sample app shows how to modify BLE advertise data

const char* const CustomBLEAdv::LAUNCHABLE_NAME = "CustAdv";
// BLE Advertise data is specified in blocks: length in bytes "len" [int8], type id [int8], content [len-1 bytes]
// NOTE: Below we use invalid CompanyID 0xFEFE. Replace with your own if you use this in product.
// CompanyID's are reserved from Bluetooth SIG (www.bluetooth.org)
uint8_t s_customAvertiseData[] =
{
    // Block: Flags for BLE device
    0x2,0x1,0x6,
    // Block: Manuf specific data for embedding custom data.
    // Data here is one uint16_t for CompanyID, and one uint16_t for data payload.
    0x5, 0xFF, 0xFE,0xFE, 0x0,0x0
};

const size_t s_dataPayloadIndex = sizeof(s_customAvertiseData) -2; // Points to second last byte

CustomBLEAdv::CustomBLEAdv():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mTimer(wb::ID_INVALID_TIMER),
    mData(0)
{
}

CustomBLEAdv::~CustomBLEAdv()
{
}

bool CustomBLEAdv::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void CustomBLEAdv::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool CustomBLEAdv::startModule()
{
    // Start data update timer. true = trigger repeatedly
    mTimer = startTimer(DATA_UPDATE_PERIOD_MS, true);
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void CustomBLEAdv::stopModule()
{
    // Stop data update
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void CustomBLEAdv::onTimer(wb::TimerId timerId)
{
    // Update our data (running 16 bit counter)
    mData++;

    // Update data to advertise packet
    uint8_t *pDataBytes = reinterpret_cast<uint8_t*>(&mData);
    s_customAvertiseData[s_dataPayloadIndex] = *pDataBytes++;
    s_customAvertiseData[s_dataPayloadIndex+1] = *pDataBytes;

    // Update advertising packet
    WB_RES::AdvSettings advSettings;
    advSettings.interval = 1600; // 1000ms in 0.625ms BLE ticks
    advSettings.timeout = 0; // Advertise forever
    advSettings.advPacket = wb::MakeArray<uint8>(s_customAvertiseData, sizeof(s_customAvertiseData));
    // NOTE: To modify scan response packet, just set similarily advSettings.scanRespPacket. Data format is the same
    // Here the scanRespPacket is left default so that the device is found with the usual name.

    asyncPut(WB_RES::LOCAL::COMM_BLE_ADV_SETTINGS(), AsyncRequestOptions::Empty, advSettings);
}

void CustomBLEAdv::onPutResult(wb::RequestId requestId,
                               wb::ResourceId resourceId,
                               wb::Result resultCode,
                               const wb::Value& rResultData)
{
    if (resourceId.localResourceId == WB_RES::LOCAL::COMM_BLE_ADV_SETTINGS::LID)
    {
        DEBUGLOG("COMM_BLE_ADV_SETTINGS returned status: %d", resultCode);
    }
}
