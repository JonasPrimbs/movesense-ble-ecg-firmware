// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"

namespace whiteboard
{
namespace services
{

/** PING service provider */
class PingProvider : private ResourceProvider
{
public:
    /** Initializes a new instance of PingProvider
    */
    PingProvider();

    /** Destructor */
    virtual ~PingProvider();

    /// Called by the CommServices launchable module
    void initSubmodule();
    void deinitSubmodule();

protected:

    /// @see whiteboard::ResourceProvider::onGetRequest
    virtual void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;
};

} // namespace services
} // namespace whiteboard
