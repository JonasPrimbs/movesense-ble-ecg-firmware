// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Initialization.h"

/** Whiteboard configuration that we use */
extern const whiteboard::Config WB_CONFIG;

namespace whiteboard
{
// @return true if the program is running in CI environment (Win/Linux/Mac only)
bool isRunningInContinuousIntegration();
}
