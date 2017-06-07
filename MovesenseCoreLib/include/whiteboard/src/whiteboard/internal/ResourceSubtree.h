#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/metadata/MetadataStructures.h"
#include "whiteboard/metadata/MetadataMap.h"
#include "whiteboard/Initialization.h"
#include "whiteboard/Result.h"
#include "whiteboard/containers/PackedIntegerArray.h"

#define WB_OPTIMIZE_MEM_USAGE


namespace whiteboard
{

// TODO: Remove this when all of internal code has been moved to internal namespace
namespace internal { class ResourceSubtree;  }
using internal::ResourceSubtree;

// Forward declarations
class ResourceMetadata;

namespace internal
{

// Forward declarations
struct ResourceSubscriptionData;
class ResourceSubtree;
class ResourceTree;

/** Mount point */
struct MountPoint
{
public:
    /** First mounted subtree */
    ResourceSubtree* pFirstSubtree;
};

/** Resource subtree */
class ResourceSubtree : public IDynamicallyAllocatable
{
public:
    /**
    * Initializes a new instance of the ResourceSubtree class
    *
    * @param rMetadataMap Metadata map to use
    * @param providerRegistrySize Size of the provider registry
    * @param subscriptionRegistrySize Size of the subscription registry
    */
    ResourceSubtree(
        const MetadataMap& rMetadataMap,
        uint16 providerRegistrySize,
        uint16 subscriptionRegistrySize);

    /**
    *	Destructor.
    **/
    virtual ~ResourceSubtree();

    /** Gets metadata map associated with this sub tree
     *
     * @return Metadata map instance
     */
    inline const MetadataMap& getMetadataMap() const
    {
        return mrMetadataMap;
    }

    /** Checks whether subtree has any resources with
     * given resource API ID.
     *
     * @param apiId ID of the API which resources should be checked
     * @return A value indicating whether API exists
     */
    bool hasApiId(metadata::ApiId apiId) const;

    /** Finds ResourceTreeNode with given id
    *
    * @param localResourceId ID of the resource tree node
    * @return Pointer to ResourceTreeNode or NULL if node was not found
    */
    const metadata::ResourceTreeNode* getNodeById(LocalResourceId localResourceId) const;

    /** Returns a local resource by index
    *
    * @param index Index of the resource tree node
    * @param rLocalResourceId On output contains ID of the local resource
    * @return Object or NULL if invalid id is given
    */
    const metadata::ResourceTreeNode* getNodeByIndex(size_t index, LocalResourceId& rLocalResourceId) const;

    /**
    * Gets resource metadata with given ID
    *
    * @param index Index of the resource tree node
    * @param rLocalResourceId On output contains ID of the local resource
    * @param rMetadata On output contains resource metadata
    * @param rSubscriptionData On output contains resource's subscription information
    * @return Result of the operation
    */
    Result getResourceMetadataByIndex(
        size_t index,
        LocalResourceId& rLocalResourceId,
        ResourceMetadata& rMetadata,
        ResourceSubscriptionData& rSubscriptionData) const;

    /** Initializes resource subscruption data */
    void initializeResourceSubscriptionData();

    /** Updates resource's subscription data
     *
     * @param index Index of the resource tree node
     * @param rSubscriptionData New information for resource's subscription data
     */
    void updateResourceSubscriptionData(
        const ResourceSubscriptionData& rSubscriptionData);

    /** Finds ResourceTreeNode with given id without any boundary checking
    *
    * @param localResourceId ID of the resource tree node
    * @return Pointer to ResourceTreeNode or NULL if node was not found
    */
    const metadata::ResourceTreeNode* getNodeByIdUnsafe(LocalResourceId localResourceId) const;

private:
    /** ResourceTree is our friend */
    friend class ResourceTree;

    /** Metadata map instance */
    const MetadataMap& mrMetadataMap;

#ifdef WB_OPTIMIZE_MEM_USAGE
    /** Number of bits needed for provider Ids in resource subscription data */
    const uint8 mNumberOfBitsForProviderIds;

    /** Resource subscription data for id paths. */
    PackedIntegerArray mResourceSubscriptionData;
#else
    struct ResourceSubscriptionDataItem
    {
        /** Current resource provider */
        LocalProviderId providerId;

        /** Id of first subscription */
        LocalSubscriptionId firstSubscriptionId;
    };

    /** Array of subscription data for id paths. */
    ResourceSubscriptionDataItem* mpResourceSubscriptionData;
#endif

    /** Parent resource */
    LocalResourceId mParentResourceId;

    /** Array of instantiated mount points in the subtree. */
    MountPoint* mpMountPointInstances;

    /** Next resource tree in the same mount point */
    ResourceSubtree* mpNextMountPointSubtree;

    /** Next resource tree in the whole tree */
    ResourceSubtree* mpNextSubtree;
};

} // namespace internal
} // namespace whiteboard
