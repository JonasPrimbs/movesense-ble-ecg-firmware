#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/UsbCdcEnumerator.h"
#include "whiteboard/devicediscovery/windows/UsbPnpNotifier.h"

namespace whiteboard {
namespace device_discovery {

/**
* UsbCdcEnumerator listens for USB PnP events and detects new CDC serial port devices.
* When there is a change is the list of devices, listener is notified via callback.
*/
class UsbCdcEnumeratorImpl final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating USB CDC serial port devices.
    *
    * @param supportedDevices List of supported devices
    **/
    UsbCdcEnumeratorImpl(const std::vector<UsbDeviceId>& supportedDevices);

    /** Destructor */
    ~UsbCdcEnumeratorImpl();

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
    /** Updates the device list.
     *
     * @param initialUpdate A value indicating whether this is initial update
     */
    void updateDevices(bool initialUpdate);

    /** Information about enumerated device */
    struct DeviceInfo
    {
        /** Vendor identifier */
        UsbVendorId mVid;

        /** Product identifier */
        UsbProductId mPid;

        /// The device serial number (USB or simulator serial)
        SuuntoSerial mSerial;

        /// The device description (from USB description or simulator)
        std::string mDescription;

        /// The device name. For example "COM6" on Windows or "/dev/cu.usbmodem1411" on OS X.
        std::string mDevice;
    };
    
    /** Enumerates devices
    *
    * @return List of enumerated devices
    */
    std::vector<DeviceInfo> doEnumerate();

private:
    const std::vector<UsbDeviceId> mSupportedDevices;
    std::vector<Device*> mDevices;
    DeviceChangeListener_t mDeviceChangedListener;
    UsbPnpNotifier notifier;
};

} // namespace device_discovery
} // namespace whiteboard
