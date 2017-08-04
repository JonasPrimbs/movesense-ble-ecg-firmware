#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <vector>
#include <thread>
#include "whiteboard/devicediscovery/UsbCdcEnumerator.h"
#include "whiteboard/integration/os/shared/semaphorecxx11.h"
#include <IOKit/IOKitLib.h>
#include <IOKit/IOMessage.h>
#include <IOKit/IOCFPlugIn.h>
#include <IOKit/IOBSD.h>
#include <IOKit/usb/IOUSBLib.h>
#include <IOKit/serial/IOSerialKeys.h>

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

    /** Enumerates currently connected devices
    *
    * @param enumCallback Callback that should be called for each of the connected device
    */
    void enumerateDevices(std::function<void(IDevice*)> enumCallback) override;

    enum class UpdateType
    {
        INIT,
        CONNECT
    };

    /** Updates list of devices */
    void updateDevices(const UpdateType updateType);

    /** Removes a device
     *
     * @param pDevice Device that should be removed
     */
    void removeDevice(IDevice* pDevice);

private:
    /** Register for PnP notifications */
    void startListeningDeviceChanges();

    /** Checks and constructs new IDevice object from given USB device handle
    *
    * @param usbDevice OS handle to the USB device
    * @return IDevice instance for the new device or nullptr if device is not supported
    */
    IDevice* checkNewDevice(io_service_t usbDevice);

private:
    const std::vector<UsbDeviceId> mSupportedDevices;
    std::vector<IDevice*> mDevices;
    mutable std::mutex mDevicesMutex;
    DeviceChangeListener_t mDeviceChangedListener;
    
    CFRunLoopRef mRunLoop;
    cxx11::semaphore mListenerSema;
    std::thread mPnpThread;
    io_iterator_t mDeviceAddedIterator;
    IONotificationPortRef mNotifyPort;
};

} // namespace device_discovery
} // namespace whiteboard
