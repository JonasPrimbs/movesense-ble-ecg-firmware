#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <vector>  // std::vector
#include <string>  // std::string
#include "BleEnumeratorImpl.h"

using namespace whiteboard::device_discovery; // BleEnumerator

namespace wbble
{
    struct BLEDeviceInfo
    {
        std::string serialNumber;
        std::string description;
        std::string device;
        std::string uuid;
    };

    // Prototypes
    std::vector<BLEDeviceInfo> enumerateBleDevices();                                 // List all connected BLE devices
    void setBleEnumeratorImpl(whiteboard::comm_ble::BleEnumeratorImpl* impl); // Set the BleEnumerator implementation pointer (to be used from the BleEnumerator)
} // namespace wbble

