#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

// Windows 7 and newer OS
#define  _WIN32_WINNT   0x0601

// Don't bloat by default. Include extra stuff when required.
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <Windows.h>
#include <VersionHelpers.h>

// Define Windows 10 version check helper our selves, if we are still compiling with older Windows SDK
// Note that application must be targeted for Windows 10 or this will return always false
// See https://msdn.microsoft.com/en-us/library/windows/desktop/dn481241(v=vs.85).aspx
#ifndef _WIN32_WINNT_WIN10

#define _WIN32_WINNT_WIN10                  0x0A00

VERSIONHELPERAPI IsWindows10OrGreater()
{
    return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WIN10), LOBYTE(_WIN32_WINNT_WIN10), 0);
}

#endif
