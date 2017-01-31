#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/ResourceProvider.h"

namespace whiteboard
{

/** Wrapper class to provide a more priviledged access to ResourceProvider class */
class ResourceProviderAccessor
{
public:

    /** Sets the given provider as a provider that bypasses a resource; security checks for PUT and typechecks are then skipped.
    *
    * @param rProvider [in] Parameter to set as Provider that bypasses a resource.
    */
    inline static void setBypass(ResourceProvider& rProvider) { rProvider.mIsBypass = true; }

    /** Checks if the given provider is bypass provider.
    *
    * @param rProvider [in] Provider to check.
    * @return true if the given provider bypasses a resource, else false.
    * @see  whiteboard::ResourceProviderAccessor::setBypass
    */
    inline static bool isBypass(const ResourceProvider& rProvider) { return rProvider.mIsBypass; }
};

} // namespace whiteboard
