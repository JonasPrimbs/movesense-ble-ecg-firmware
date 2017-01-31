// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceProvider.h"

namespace whiteboard
{
namespace services
{

/** Provider whiteboard version information */
class WbInfoProvider FINAL : private ResourceProvider
{
public:

    /** Ctor */
    WbInfoProvider();

    /** Destructor */
    virtual ~WbInfoProvider();

    /// Called by the WhiteboardServices launchable module
    void initSubmodule();
    void deinitSubmodule();

protected:

    /** @see whiteboard::ResourceProvider::onGetRequest */
    void onGetRequest(
        const Request& rRequest,
        const ParameterList& rParameters) OVERRIDE;

private:

    /** Prevent use of copy constructor */
    WbInfoProvider(const WbInfoProvider&) DELETED;
    /** Prevent use of assigment operator */
    const WbInfoProvider& operator=(const WbInfoProvider&) DELETED;
};

} // namespace services
} // namespace whiteboard
