#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

/**
@returns false if logging in the running thread is not allowed. Used in certain implementations to prevent debug logging on certain threads due to thread-stack limitations
Currently unused in this port.
*/
WB_API bool WbIsLoggingAllowed();
