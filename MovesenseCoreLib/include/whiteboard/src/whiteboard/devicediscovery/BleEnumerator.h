#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"

namespace whiteboard {
namespace device_discovery {

/**
* BleEnumerator scans for BLE devices. When there is a change is the list of devices,
* listener is notified via callback.
*/
class BleEnumerator final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating Whiteboard capable BLE devices.
    **/
    BleEnumerator();

    /** Destructor */
    ~BleEnumerator();

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
    /** The implementation */
    class BleEnumeratorImpl* mpImplementation;
};

} // namespace device_discovery
} // namespace whiteboard
