#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"

namespace whiteboard {
namespace device_discovery {

/**
* BleEnumerator scans for BLE devices. When there is a change is the list of devices,
* listener is notified via callback.
*/
class WB_API BleEnumerator final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating Whiteboard capable BLE devices.
    *
    * @param numberOfServiceUuids Number of service UUIDs in the array
    * @param serviceUuids Array of service UUIDs that filter enumerated BLE devices
    **/
    BleEnumerator(size_t numberOfServiceUuids, const char** serviceUuids);

    /** Destructor */
    ~BleEnumerator();

    /** Initializes the enumerator and performs initial scan
    *
    * @param listener The listener which is notified in the constructor and when simulator starts or stops.
    */
    void init(const DeviceChangeListener_t listener) override;

    /** Enumerates currently connected devices
    *
    * @param enumCallback Callback that should be called for each of the connected device
    */
    void enumerateDevices(std::function<void(IDevice*)> enumCallback) override;

private:
    /** The implementation */
    class IDeviceEnumerator* mpImplementation;
};

} // namespace device_discovery
} // namespace whiteboard
