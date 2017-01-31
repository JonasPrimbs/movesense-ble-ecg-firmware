#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"

namespace whiteboard {
namespace device_discovery {

/** Vendor ID is 4 hexadecimals. */
typedef uint16 UsbVendorId;

/** Product ID is 4 hexadecimals. */
typedef uint16 UsbProductId;

/** Vendor ID - product ID pair is used to identify devices */
struct UsbDeviceId
{
    /** Vendor ID */
    UsbVendorId vendorId;

    /** Product ID */
    UsbProductId productId;
};

inline bool operator==(const UsbDeviceId& id1, const UsbDeviceId& id2)
{
    return (id1.vendorId == id2.vendorId) && (id1.productId == id2.productId);
}

/**
* UsbCdcEnumerator listens for USB PnP events and detects new CDC serial port devices.
* When there is a change is the list of devices, listener is notified via callback.
*/
class UsbCdcEnumerator final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating USB CDC serial port devices.
    *
    * @param supportedDevices List of supported devices
    **/
    UsbCdcEnumerator(const UsbDeviceId& supportedDevice);

    /** Construct new instance for enumerating USB CDC serial port devices.
    *
    * @param supportedDevices List of supported devices
    **/
    UsbCdcEnumerator(const std::vector<UsbDeviceId>& supportedDevices);

    /** Destructor */
    ~UsbCdcEnumerator();

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
    class UsbCdcEnumeratorImpl* mpImplementation;
};

} // namespace device_discovery
} // namespace whiteboard
