// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"

namespace whiteboard
{
namespace services
{

/**
    Provider for thread runtime statistics
*/
class ThreadStatsProvider FINAL : private ResourceProvider
{
public:
    /**
        Initializes a new instance of ThreadStatsProvider
    */
    ThreadStatsProvider();

    /**
        Destructor
    */
    ~ThreadStatsProvider();

    void initSubmodule();
    void deinitSubmodule();

private:

    /**
    *  GET request handler is used to query the launchable module name and status.
    *
    *  @param rRequest Request information
    *  @param rParameters List of parameters for the request
    */
    virtual void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;
};

} // namespace services
} // namespace whiteboard
