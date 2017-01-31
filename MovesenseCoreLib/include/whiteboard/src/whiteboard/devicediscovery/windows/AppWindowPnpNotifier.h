// Copyright (c) Suunto Oy 2015. All rights reserved.

#pragma once

#include <thread>
#include <mutex>
#include "whiteboard/integration/port.h"
#include "whiteboard/integration/bsp/windows/internal/wininc.h"
#include "whiteboard/devicediscovery/windows/WindowsHelpers.h"
#include "whiteboard/devicediscovery/shared/semaphorecxx11.h"

namespace whiteboard
{
namespace device_discovery
{

enum class WindowEvent { CREATED, DESTROYED, NONE };
typedef std::function<void(HWND, WindowEvent)> WindowCreatedOrDestoyedListener_t;

/**
* Notifies window Plug and Play (PnP) events to listeners.
*/
class AppWindowPnpNotifier final
{
public:
    AppWindowPnpNotifier(const WindowCreatedOrDestoyedListener_t listener);
    ~AppWindowPnpNotifier();

    void stop();

private: // Implementation
    void run();

    std::unique_ptr<HWND, WindowDeleter> createHiddenWindow() const;
    LRESULT handleCallback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK callback(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    semaphore mCreationSemaphore;
    const WindowCreatedOrDestoyedListener_t mListener;
    std::unique_ptr<HWND, WindowDeleter> mWindow;
    std::unique_ptr<VOID, NotifierDeleter> mNotifier;
    const char* CLASS_NAME{ "AppWindowPnpNotifier" };
    std::thread mThread;

}; // PnpNotifier

} // namespace device_discovery

} // namespace whiteboard
