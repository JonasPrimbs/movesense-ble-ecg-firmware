#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Type of the buffer that is used for resource ID serialization */
typedef uint8 ResourceIdSerializationBuffer[3];

/** Type of the buffer that is used for client ID serialization */
typedef uint8 ClientIdSerializationBuffer[3];

} // protocol_v9
} // namespace whiteboard
