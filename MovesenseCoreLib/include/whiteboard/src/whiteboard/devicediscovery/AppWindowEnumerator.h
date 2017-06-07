#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/devicediscovery/IDeviceEnumerator.h"

namespace whiteboard {
namespace device_discovery {

/**
* WindowEnumerator detects new applications by listening window events. For application to
* be detectable, its window title must be in form 
* When there is a change is the list of devices, listener is notified via callback.
*/
class WB_API AppWindowEnumerator final : public IDeviceEnumerator
{
public:
    /** Construct new instance for enumerating application instances that will be detected
     * using given window titles.
     *
     * @param knownWindowTitles The list of known window titles that are recognized as applications.
     **/
    AppWindowEnumerator(const std::vector<std::string> &knownWindowTitles);

    /** Destructor */
    ~AppWindowEnumerator();

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
    /** Implementation */
    class AppWindowEnumeratorImpl* mpImplementation;
};

} // namespace device_discovery
} // namespace whiteboard
