#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <vector>
#include <thread>
#include "whiteboard/devicediscovery/IDeviceEnumerator.h"

namespace whiteboard {

// Forward declarations
class BLECentral;

namespace device_discovery {

/**
* BleEnumerator scans for BLE devices. When there is a change is the list of devices,
* listener is notified via callback.
*/
class BleEnumeratorImpl final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating Whiteboard capable BLE devices.
    **/
    BleEnumeratorImpl();

    /** Destructor */
    ~BleEnumeratorImpl();

    /** Initializes the enumerator and performs initial scan
    *
    * @param listener The listener which is notified in the constructor and when simulator starts or stops.
    */
    void init(const DeviceChangeListener_t listener) override;

    /** Gets list of currently connected devices
    *
    * @return List of currently connected devices
    */
    std::vector<Device*> getDevices() override;

private:
    enum class UpdateType
    {
        INIT,
        CONNECT,
        DISCONNECT
    };

    void updateDevices(const UpdateType);
    void setCallbacks();

private:
    std::vector<Device*> mDevices;
    DeviceChangeListener_t mDeviceChangedListener;
    BLECentral* mBleCentral;
};

} // namespace device_discovery
} // namespace whiteboard
