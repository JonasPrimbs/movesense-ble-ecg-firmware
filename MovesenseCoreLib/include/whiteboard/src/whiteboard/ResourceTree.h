#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/ExecutionContext.h"
#include "whiteboard/Result.h"

namespace whiteboard
{

// Forward declarations
class MetadataMap;

/** Utility class for resource tree related operations */
class WB_API ResourceTree FINAL
{
public:
    /** Gets full path of a resource
    *
    * @param resourceId ID of resource which path should be formatted
    * @param path On output contains full path of the resource
    * @return Result of the operation
    */
    static Result getFullResourcePath(LocalResourceId localResourceId, char path[WB_MAX_RESOURCE_PATH_LEN]);

    /** Function prototype for functions that can be used to install execution context hook
     * functions
     *
     * @param ExecutionContextId ID of the execution context which hooks should be set
     * @param ExecutionContextStateProcessorFunc [out] Custom event state processor function
     * @param ExecutionContextNotificationFilterFunc [out] Custom update notification filter function
     */
    typedef IFunctor3<void, ExecutionContextId, ExecutionContext::StateProcessorFunc*&,
            ExecutionContext::NotificationFilterFunc*&> HookInstallerFunc;

    /**
    * Adds a new sub tree to the resource tree.
    *
    * @param parentResourceId Id of the parent resource (sub tree mount point)
    * @param rMetadataMap Metadata of the subtree that should be registered
    * @param hookInstaller Optional. Hook installer function.
    * @return Result of the operation
    */
    static Result registerResourceSubtree(
        ResourceId parentResourceId, const MetadataMap& rMetadataMap, HookInstallerFunc* hookInstaller = NULL);

    /**
    * Removes a resource subtree from the resource tree
    *
    * @param rMetadataMap Metadata of the subtree that should be unregistered
    * @return A value indicating whether the resource subtree was successfully removed
    */
    static Result unregisterResourceSubtree(const MetadataMap& rMetadataMap);
};

}
