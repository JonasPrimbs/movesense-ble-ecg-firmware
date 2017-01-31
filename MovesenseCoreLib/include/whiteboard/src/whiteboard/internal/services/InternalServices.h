// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/LaunchableModule.h"

#include "whiteboard/internal/services/StartupProvider.h"
#include "whiteboard/internal/services/MetadataProvider.h"
#include "whiteboard/internal/services/RoutingTableProvider.h"
#include "whiteboard/internal/services/WbInfoProvider.h"
#include "whiteboard/internal/services/SubscriptionsInfoProvider.h"
#include "whiteboard/internal/services/StatusProvider.h"

namespace whiteboard
{
namespace services
{
/**
*	Class to instantiate and start whiteboard services / providers in the primary services context.
*	This class implements launchable module interface (LaunchableModule).
*/
class InternalServices FINAL : private LaunchableModule
{
public:
    // Launchable name comes from WhiteboardServicesInstance

    /** Constructor */
    InternalServices();

    /** Destructor */
    ~InternalServices();

private:
    /// @see whiteboard::ILaunchableModule::initModule
    virtual bool initModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::deinitModule
    virtual void deinitModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::startModule
    virtual bool startModule() OVERRIDE;
    /// @see whiteboard::ILaunchableModule::stopModule 
    virtual void stopModule() OVERRIDE;

private:
    MetadataProvider mMetadataProvider;
    RoutingTableProvider mRoutingTableProvider;
    WbInfoProvider mWbInfoProvider;
#ifdef WB_HAVE_STATUS
    StatusProvider mStatusProvider;
#endif
#ifdef WB_HAVE_SUBSCRIPTIONS_INFO
    SubscriptionsInfoProvider mSubscriptionsInfoProvider;
#endif
};

} // namespace services
} // namespace whiteboard
