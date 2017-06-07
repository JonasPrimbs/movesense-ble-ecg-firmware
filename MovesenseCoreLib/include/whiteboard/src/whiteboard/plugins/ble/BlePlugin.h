#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

// Interface functions for dynamically (or statically linked) BLE communication
// plugins.

// Forward declarations;
namespace whiteboard {
class CommAdapter;
namespace device_discovery {
class IDeviceEnumerator;
} // namespace device_discovery
} // namespace whiteboard

extern "C"
{

/** Creates a new device enumerator instance for enumerating BLE devices
 *
 * @param numberOfServiceUuids Size of the service UUID list
 * @param serviceUuids List of service UUIDs that are used to filter BLE devices
 * @return New device enumerator instance
 */
whiteboard::device_discovery::IDeviceEnumerator* createBleEnumerator(
    size_t numberOfServiceUuids, const char** serviceUuids);
    
/** Creates a new device enumerator instance for enumerating BLE devices
 *
 * @param unused Unused parameter
 * @return New communication interface for working with BLE connections
 */
whiteboard::CommAdapter* createBleAdapter(void* unused);

}

