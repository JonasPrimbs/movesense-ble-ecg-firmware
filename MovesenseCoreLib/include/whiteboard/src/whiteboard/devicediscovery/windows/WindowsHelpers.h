#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/bsp/windows/internal/wininc.h"
#include <SetupAPI.h>
#include <memory>
#include <thread>

struct WindowDeleter
{
    typedef HWND pointer;
    virtual void operator()(HWND handle) const { DestroyWindow(handle); }
};

struct WindowAndClassDeleter final : public WindowDeleter
{
    typedef HWND pointer;
    void operator()(HWND handle) const override
    { 
        char className[128];
        GetClassNameA(handle, className, sizeof(className));
        DestroyWindow(handle);
        UnregisterClassA(className, NULL);
    }
};

struct NotifierDeleter
{
    typedef HDEVNOTIFY pointer;
    void operator()(HDEVNOTIFY handle) const { UnregisterDeviceNotification(handle); }
};

struct DevInfoListDeleter
{
    typedef HDEVINFO pointer;
    void operator()(HDEVINFO handle) const { SetupDiDestroyDeviceInfoList(handle); }
};

#define WM_TERMINATE_THREAD     WM_USER

inline void TerminateWindowThread(std::thread& thread)
{
    PostThreadMessage(GetThreadId(thread.native_handle()), WM_TERMINATE_THREAD, 0, 0);
    thread.join();
}

/// Translate and dispatch all messages from the queue
inline bool DispatchThreadWindowMessages()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_TERMINATE_THREAD)
        {
            return false;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return true;
}

template<class TCallback, class TClassName>
void registerWindowClass(TCallback callback, TClassName classname)
{
    auto wc = WNDCLASS{};
    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc = callback;
    wc.lpszClassName = classname;
    RegisterClass(&wc);
}

template<class TClassName>
void unregisterWindowClass(TClassName classname)
{
    UnregisterClass(classname, NULL);
}
