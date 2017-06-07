#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <vector>
#include <whiteboard/devicediscovery/IDeviceEnumerator.h>

namespace whiteboard {
namespace comm_ble {

/**
* BleEnumerator scans for BLE devices. When there is a change is the list of devices,
* listener is notified via callback.
*/
class BleEnumeratorImpl final : public device_discovery::IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating Whiteboard capable BLE devices.
    *
    * @param numberOfServiceUuids Number of service UUIDs in the array
    * @param serviceUuids Array of service UUIDs that filter enumerated BLE devices
    **/
    BleEnumeratorImpl(size_t numberOfServiceUuids, const char** serviceUuids);

    /** Destructor */
    ~BleEnumeratorImpl();

    /** Initializes the enumerator and performs initial scan
    *
    * @param listener The listener which is notified in the constructor and when simulator starts or stops.
    */
    void init(const DeviceChangeListener_t listener) override;

    /** Enumerates currently connected devices
    *
    * @param enumCallback Callback that should be called for each of the connected device
    */
    void enumerateDevices(std::function<void(device_discovery::IDevice*)> enumCallback) override;

public:
    enum class UpdateType
    {
        INIT,
        CONNECT,
        DISCONNECT
    };

    /** Updates list of devices */
    void updateDevices(const UpdateType updateType);

private:
    std::vector<device_discovery::IDevice*> mDevices;
    DeviceChangeListener_t mDeviceChangedListener;
};

} // namespace comm_ble
} // namespace whiteboard

