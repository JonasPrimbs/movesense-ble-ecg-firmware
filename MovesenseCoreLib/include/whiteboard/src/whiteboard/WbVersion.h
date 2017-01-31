#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"

#ifndef BUILD_VCS_NUMBER
#define BUILD_VCS_NUMBER "00000000"
#endif

#define WB_VERSION "2.4.2-" BUILD_VCS_NUMBER

/** Minimum supported whiteboard communication protocol version */
#ifndef WB_COMM_PROTO_MIN_VERSION
#define WB_COMM_PROTO_MIN_VERSION 9
#endif

/** Current whiteboard communication protocol version */
#ifndef WB_COMM_PROTO_CURRENT_VERSION
#define WB_COMM_PROTO_CURRENT_VERSION 9
#endif

WB_STATIC_VERIFY(WB_COMM_PROTO_CURRENT_VERSION >= WB_COMM_PROTO_MIN_VERSION, InvalidProtocolVersionConfiguration);
