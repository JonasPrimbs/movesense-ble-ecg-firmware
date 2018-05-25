#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/internal/services/RoutingTableProvider.h"

// Forward declarations
namespace whiteboard
{
class WhiteboardCommunication;
}

class TestRoutingTableProvider
{
public:
    TestRoutingTableProvider(whiteboard::WhiteboardCommunication& rCommunication);
    ~TestRoutingTableProvider();

protected:
    whiteboard::services::RoutingTableProvider mRoutingTableProvider;
};

