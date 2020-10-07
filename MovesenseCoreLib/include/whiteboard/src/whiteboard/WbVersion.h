#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/integration/port.h"

#ifndef BUILD_VCS_NUMBER
#define BUILD_VCS_NUMBER "00000000"
#endif

#define WB_VERSION_MAJOR 4
#define WB_VERSION_MINOR 1
#define WB_VERSION_PATCH 2

#define WB_VERSION WB_STRINGIFY(WB_VERSION_MAJOR) "." WB_STRINGIFY(WB_VERSION_MINOR) "." WB_STRINGIFY(WB_VERSION_PATCH) "-" BUILD_VCS_NUMBER

/** Minimum supported whiteboard communication protocol version */
#ifndef WB_COMM_PROTO_MIN_VERSION
#define WB_COMM_PROTO_MIN_VERSION 9
#endif

/** Current whiteboard communication protocol version */
#if WB_HAVE_OPERATION_TIMESTAMP
#ifndef WB_COMM_PROTO_CURRENT_VERSION
#define WB_COMM_PROTO_CURRENT_VERSION 10
#endif
#else
#ifndef WB_COMM_PROTO_CURRENT_VERSION
#define WB_COMM_PROTO_CURRENT_VERSION 9
#endif
#endif

WB_STATIC_VERIFY(WB_COMM_PROTO_CURRENT_VERSION >= WB_COMM_PROTO_MIN_VERSION, InvalidProtocolVersionConfiguration);
