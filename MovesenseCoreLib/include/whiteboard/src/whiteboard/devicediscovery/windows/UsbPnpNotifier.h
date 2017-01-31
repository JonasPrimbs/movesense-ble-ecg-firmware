/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/
#pragma once
#include <thread>
#include <mutex>
#include "whiteboard/integration/port.h"
#include "whiteboard/integration/bsp/windows/internal/wininc.h"
#include "whiteboard/devicediscovery/windows/WindowsHelpers.h"

namespace whiteboard
{
namespace device_discovery
{

typedef std::function<void(bool)> UsbPnpListener_t;

enum class PnpNotifierStatus_e
{
    RUNNING,
    EXITING
};

/**
* Notifying device Plug and Play (PnP) events to listeners.
*/
class UsbPnpNotifier final
{
public:
    /** Construct new notifier that will notify the listener when a USB device is attached or removed from the system. */
    UsbPnpNotifier(const UsbPnpListener_t listener);
    ~UsbPnpNotifier();

    void stop();

private: // Implementation
    void run();

    std::unique_ptr<HWND, WindowDeleter> createHiddenWindow() const;
    std::unique_ptr<VOID, NotifierDeleter> registerNotification(HWND hWnd);
    LRESULT handleCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    const UsbPnpListener_t mListener;
    std::unique_ptr<HWND, WindowDeleter> mWindow;
    std::unique_ptr<VOID, NotifierDeleter> mNotifier;
    const char* CLASS_NAME{ "UsbPnpNotifier" };
    std::thread mThread;

}; // PnpNotifier

} // namespace device_discovery

} // namespace whiteboard
