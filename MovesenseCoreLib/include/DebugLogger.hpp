// Copyright (c) Suunto Oy 2018. All rights reserved.
#pragma once
#include <cstdarg>
#include <cstdlib>
#include "system_debug/resources.h"
#include "DebugServiceInput.hpp"

template <DebugServiceInput::MessageSource SRC>
class DebugLoggerBase
{
public:
    typedef WB_RES::DebugLevelValues::Type Level;

    template <typename... Args>
    static void fatal(const char* str, Args... args)
    {
        DebugServiceInput::log(SRC, Level::FATAL, str, args...);
    }

    template <typename... Args>
    static void error(const char* str, Args... args)
    {
        DebugServiceInput::log(SRC, Level::ERROR, str, args...);
    }

    template <typename... Args>
    static void warning(const char* str, Args... args)
    {
        DebugServiceInput::log(SRC, Level::WARNING, str, args...);
    }

    template <typename... Args>
    static void info(const char* str, Args... args)
    {
        DebugServiceInput::log(SRC, Level::INFO, str, args...);
    }

    template <typename... Args>
    static void verbose(const char* str, Args... args)
    {
        DebugServiceInput::log(SRC, Level::VERBOSE, str, args...);
    }

};


using SystemLogger = DebugLoggerBase<DebugServiceInput::MessageSource::SYSTEM>;
using DebugLogger = DebugLoggerBase<DebugServiceInput::MessageSource::USER>;
