// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"
#include "whiteboard/integration/os/shared/semaphorecxx11.h"
#include "whiteboard/devicediscovery/windows/AppWindowPnpNotifier.h"

namespace whiteboard {
namespace device_discovery {

// Forward declarations
class AppWindowDevice;

/** Information about enumerated device */
struct DeviceInfo
{
    /// The device serial number
    SuuntoSerial mSerial;

    /// The device description
    std::string mDescription;

    /// The device address
    std::string mAddress;
};

/** Implementation of AppWindowEnumerator for Windows */
class AppWindowEnumeratorImpl final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating application instances that will be detected
    * using given window titles.
    *
    * @param knownWindowTitles The list of known window titles that are recognized as applications.
    **/
    AppWindowEnumeratorImpl(const std::vector<std::string> &knownWindowTitles);

    /** Destructor */
    ~AppWindowEnumeratorImpl();

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
    /** Called when SimulatorPnPNotifier finds that top level window has been created or destroyed */
    void onWindowNotification(HWND hwnd, const WindowEvent event);

    /** Thread loop of the update thread */
    void updateThreadLoop();

    /** Actual device scanning */
    void updateDevices(bool initialUpdate);

    /** Enumerates devices
    *
    * @return List of currently connected devices
    */
    std::vector<DeviceInfo> doEnumerate();

private:
    std::vector<AppWindowDevice*> mDevices;
    const std::vector<std::string> mKnownWindowTitles;
    DeviceChangeListener_t mDeviceChangedListener;
    AppWindowPnpNotifier mNotifier;
    cxx11::semaphore mUpdateThreadShutdownSemaphore;
    cxx11::semaphore mUpdateSemaphore;
    std::thread mUpdateThread;
};

} // namespace device_discovery
} // namespace whiteboard
