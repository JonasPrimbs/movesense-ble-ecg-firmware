#pragma once
/******************************************************************************

    Copyright (c) Suunto Oy 2016.
    All rights reserved.

******************************************************************************/

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/** This type is used in templatized client API calls to indicate that no parameter is passed to API */
struct NoType
{
    WB_API static const NoType NoValue;
};

} // namespace whiteboard
