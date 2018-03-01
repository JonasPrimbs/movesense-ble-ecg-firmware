// Copyright (c) Suunto Oy 2018. All rights reserved.
#pragma once
#include <cstdarg>
#include <cstdlib>
#include "system_debug/resources.h"


class DebugServiceInput
{
public:
    enum MessageSource
    {
        SYSTEM,
        USER
    };

    typedef WB_RES::DebugLevelValues::Type Level;

    // Note: this limit includes null-terminator.
    static constexpr size_t MAX_LOG_LINE_LENGTH = 80;

    static void log(MessageSource source, Level level, const char* str, ...);
};

