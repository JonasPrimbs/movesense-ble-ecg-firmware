#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <mutex>
#include "whiteboard/Communication.h"

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"
#include "whiteboard/devicediscovery/Connection.h"

namespace whiteboard {
namespace device_discovery {

typedef std::vector<Connection> ConnectedDeviceList_t;
typedef std::function<void(const ConnectedDeviceList_t&)> DeviceDiscoveryCallback_t;

/**
* Discovering and connecting specific Suunto USB or simulator devices.
*
* Devices are ad hoc connected when enumerator reports a set of devices.
*/
class DeviceDiscoverer final
{
public:
    /** Construct new instance of DeviceDiscoverer class.
    *
    * @param rEnumerators List of enumerators to use
    * @param discoveryCb The user-specified function to call when device is detected
    * @note The callback might be called from a different thread than where the class was originally constructed.
    */
    DeviceDiscoverer(DeviceEnumeratorList_t& rEnumerators, const DeviceDiscoveryCallback_t discoveryCb);
    ~DeviceDiscoverer();

private:
    void deviceChanged(IDeviceEnumerator& rEnumerator);

private:
    const DeviceDiscoveryCallback_t mDiscoveryCallback;
    DeviceEnumeratorList_t mEnumerators;
    std::mutex mConnectionMutex;
    ConnectedDeviceList_t mConnections;
};

} // namespace device_discovery
} // namespace whiteboard

