// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include <whiteboard/ResourceProvider.h>

namespace nea
{

/** Device information provider. */
class InfoProvider : private whiteboard::ResourceProvider
{
public:
    /** Constructor */
    InfoProvider();

    /** Destructor */
    virtual ~InfoProvider();

    // called from the launchable module
    bool wbRegister();
    void wbUnregister();

    /// The version number. For example 1.0.1. Defined in VERSION.txt
    static const char* DeviceFirmwareVersion;

private:
    /**
    *	GET request handler is used to query the information.
    *
    *	@param requestId ID of the request
    *	@param clientId ID of the client that should receive the result
    *	@param resourceId ID of the associated resource
    *	@param parameters empty List of parameters
    */
    virtual void onGetRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
};

} // namespace Nea
