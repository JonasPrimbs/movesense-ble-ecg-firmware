#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/Result.h"

namespace whiteboard
{

// Forward declarations
class ResourceSubtree;

/** Utility class to register own resource subtrees with Whiteboard */
class WB_API ResourceSubtreeRegistration FINAL
{
public:

    /**
    * Adds a new sub tree to the resource tree.
    *
    * @param parentResourceId Id of the parent resource (sub tree mount point)
    * @param rSubtree The resource sub tree
    * @return Result of the operation
    */
    static Result registerResourceSubtree(ResourceId parentResourceId, ResourceSubtree& rSubtree);

    /**
    * Removes a resource subtree from the resource tree
    *
    * @param rSubtree The resource sub tree
    * @return A value indicating whether the resource was successfully removed
    */
    static Result unregisterResourceSubtree(ResourceSubtree& rSubtree);
};

}