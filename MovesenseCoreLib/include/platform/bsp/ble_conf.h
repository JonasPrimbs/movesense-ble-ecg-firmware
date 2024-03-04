#pragma once

#include "common/compiler/genDef.h"

extern "C"
struct BleConfig {
    uint16_t BLE_GAP_DATA_LENGTH;
    uint16_t BLE_PERIPHERAL_LINK_COUNT;
    uint16_t BLE_CENTRAL_LINK_COUNT;
    uint16_t BLE_TOTAL_LINK_COUNT;
    uint16_t BLE_GAP_EVENT_LENGTH;
    uint16_t BLE_GATT_MAX_MTU_SIZE;
    uint16_t BLE_GATTS_ATTR_TAB_SIZE;
    uint16_t BLE_VS_UUID_COUNT;

    BleConfig() {
        BLE_GAP_DATA_LENGTH=0;
        BLE_PERIPHERAL_LINK_COUNT=0;
        BLE_CENTRAL_LINK_COUNT=0;
        BLE_TOTAL_LINK_COUNT=0;
        BLE_GAP_EVENT_LENGTH=0;
        BLE_GATT_MAX_MTU_SIZE=0;
        BLE_GATTS_ATTR_TAB_SIZE=0;
        BLE_VS_UUID_COUNT=0;
    }
    BleConfig(uint16_t data_length, 
        uint16_t peripheral_link_count,
        uint16_t central_link_count, 
        uint16_t total_link_count,
        uint16_t event_length,
        uint16_t max_mtu_size,
        uint16_t attr_tab_size,
        uint16_t uuid_count)
        : BLE_GAP_DATA_LENGTH(data_length), 
          BLE_PERIPHERAL_LINK_COUNT(peripheral_link_count), 
          BLE_CENTRAL_LINK_COUNT(central_link_count), 
          BLE_TOTAL_LINK_COUNT(total_link_count), 
          BLE_GAP_EVENT_LENGTH(event_length), 
          BLE_GATT_MAX_MTU_SIZE(max_mtu_size), 
          BLE_GATTS_ATTR_TAB_SIZE(attr_tab_size), 
          BLE_VS_UUID_COUNT(uuid_count)
    {
    }
};

// Defaults to NULL unless explicitly defined elsewhere
extern const BleConfig * WEAK g_BLE_ConfigPtr;