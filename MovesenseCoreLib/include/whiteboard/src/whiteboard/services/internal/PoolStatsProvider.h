// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"

namespace whiteboard
{
namespace services
{

/** Pool usage provider */
class PoolStatsProvider FINAL : private ResourceProvider
{
public:
    /** Initializes a new instance of PoolStatsProvider
    */
    PoolStatsProvider();

    /** Destructor
    */
    ~PoolStatsProvider();

    /// Called by the MetricsServices launchable module
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
