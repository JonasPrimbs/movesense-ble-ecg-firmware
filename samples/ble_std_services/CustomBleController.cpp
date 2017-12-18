#include <stdio.h>
#include "CustomBleController.hpp"
#include "BleServicesApp.hpp"
#include "common/core/debug.h"
#include <string.h>

// Whiteboard interface UUID: 61353090-8231-49CC-B57A-886370740041
const uint8_t WhiteboardUUID[16] =
{ 0x41, 0x00, 0x74, 0x70, 0x63, 0x88, 0x7A, 0xB5,
  0xCC, 0x49, 0x31, 0x82, 0x90, 0x30, 0x35, 0x61 };

const uint8_t StandardHrsUUID[2] = { 0x0D, 0x18 };

#define MANUF_SPEC_DATA { 0x9F, 0x00, 0x01, 0x02, 0x03, 0x00 }
#define ADVERT_FLAGS 0x06

#define MANUFACTURER_NAME           "Suunto"
#define BLE_SUUNTO_COMPANY_ID       0x009f

#define BLE_GAP_AD_TYPE_FLAGS 0x01
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE   0x02
#define BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_COMPLETE         0x03
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE  0x06
#define BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_COMPLETE 0x07
#define BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME 0x09
#define BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA 0xFF



CustomBleController::CustomBleController()
{
}


// For Adverising and sacn response data see Bluetooth Core Specification v 5.0
// or see examples, e.g.:
// https://www.pubnub.com/blog/2015-04-14-building-android-beacon-android-ibeacon-tutorial-overview/

uint32_t CustomBleController::PrepareAdvertisingData(uint8_t * buffer, uint32_t max_len)
{
    uint8_t flags = ADVERT_FLAGS;
    uint8_t name[30];
    uint32_t name_len;
    uint32_t size = 0;

    name_len = snprintf((char *) &name[0], sizeof(name) - 1, "Movesense %s",
            GetDeviceIdentifier());

    if (name_len > max_len - 9)         // 3 bytes of flags + 4 bytes of 16 bit UUID
        name_len = max_len - 9;         // + 2 bytes chunk header = 9

    size = AddChunkToBuffer(buffer, BLE_GAP_AD_TYPE_FLAGS, &flags, sizeof(flags));

    size += AddChunkToBuffer(buffer + size,
        BLE_GAP_AD_TYPE_16BIT_SERVICE_UUID_MORE_AVAILABLE,
        &StandardHrsUUID[0], 2);

    size += AddChunkToBuffer(buffer + size, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, &name[0],
            name_len);

    return size;
}


uint32_t CustomBleController::PrepareScanResponseData(uint8_t * buffer, uint32_t max_len)
{
    uint32_t size = 0;
    uint8_t manuf_data[] = MANUF_SPEC_DATA;

    size = AddChunkToBuffer(buffer, BLE_GAP_AD_TYPE_128BIT_SERVICE_UUID_MORE_AVAILABLE,
            &WhiteboardUUID[0], sizeof(WhiteboardUUID));

    size += AddChunkToBuffer(buffer + size, BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
            &manuf_data[0], sizeof(manuf_data));

    return size;
}


void CustomBleController::OnHrsNotificationEnabled()
{
    BleServicesApp::spInstance->mHrsEnableReq = true;
    BleServicesApp::queueDpc();
}


void CustomBleController::OnHrsNotificationDisabled()
{
    BleServicesApp::spInstance->mHrsEnableReq = false;
    BleServicesApp::queueDpc();
}


void CustomBleController::OnUartData(uint8_t * p_data, uint16_t length)
{
    uint8_t buffer[64];
    if (length >= sizeof(buffer))
        length = sizeof(buffer) - 1;

    memcpy(buffer, p_data, length);
    buffer[length] = 0;
    DEBUGLOG("UART: %s", buffer);
}
