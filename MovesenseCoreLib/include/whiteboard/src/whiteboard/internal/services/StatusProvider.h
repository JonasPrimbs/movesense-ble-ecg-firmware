// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_STATUS)

#ifdef WB_HAVE_STATUS
namespace whiteboard
{
namespace services
{

/** Service provider for Whiteboard internal status */
class StatusProvider FINAL : private ResourceProvider
{
public:
    /** Initializes a new instance of StatusProvider
    */
    StatusProvider();

    /** Destructor */
    virtual ~StatusProvider();

    /// Called by the CommServices launchable module
    void initSubmodule();
    void deinitSubmodule();

protected:

    /// @see whiteboard::ResourceProvider::onGetRequest
    virtual void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;
};

} // namespace services
} // namespace whiteboard
#endif