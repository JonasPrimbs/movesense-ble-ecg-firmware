#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

// Forward declarations
class ResourceMetadata;
struct ResourceSubscriptionData;
class ResourceSubtree;
class ResourceTree;
struct MetadataMap;

/** Mount point */
struct MountPoint
{
public:
    /** First mounted subtree */
    ResourceSubtree* pFirstSubtree;
};

/** Resource subtree */
class ResourceSubtree
{
public:
    /**
    * Initializes a new instance of the ResourceSubtree class
    *
    * @param rMetadataMap Metadata map to use
    * @param rResourceTree Resource tree metadata
    */
    ResourceSubtree(
        const MetadataMap& rMetadataMap,
        const metadata::ResourceTree& rResourceTree);

    /**
    * Initializes a new instance of the ResourceSubtree class
    *
    * @param rMetadataMap Metadata map to use
    * @param numberOfResources Number of resources.
    * @param pTreeNodes Array of metadata tree nodes.
    * @param pResourceSubscriptionData Array of subscription data.
    * @param numberOfSparseMapEntries Number of entries in the sparse map of resources
    * @param pSparseMap Sparse map that maps resource IDs to indexes of pTreeNode array
    * @param numberOfMountPoints Nmber of mount points
    * @param pMountPoints Array of mount points in the subtree
    */
    ResourceSubtree(
        const MetadataMap& rMetadataMap,
        LocalResourceId numberOfResources,
        const metadata::ResourceTreeNode* pTreeNodes,
        size_t numberOfSparseMapEntries,
        const LocalResourceId* pSparseMap,
        size_t numberOfMountPoints,
        const metadata::MountPoint* pMountPoints);

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

    /** Gets number of resources in the tree
     *
     * @return Number of resources
     */
    inline size_t getNumberOfResources() const
    {
        return mNumberOfResources;
    }

    /** Checks whether subtree has any resources with
     * given resource API ID.
     *
     * @param apiId ID of the API which resources should be checked
     * @return A value indicating whether API exists
     */
    inline bool hasApiId(metadata::ApiId apiId) const
    {
        const size_t apiIdPlusOne = static_cast<size_t>(apiId) + 1;
        if (apiIdPlusOne >= mNumberOfSparseMapEntries)
        {
            return false;
        }
    
        return mpSparseMap[apiId] != mpSparseMap[apiIdPlusOne];
    }

    /** Finds ResourceTreeNode with given id
    *
    * @param localResourceId ID of the resource tree node
    * @return Pointer to ResourceTreeNode or NULL if node was not found
    */
    const metadata::ResourceTreeNode* getNodeById(LocalResourceId localResourceId) const;

    /** Returns a local resource by index
    *
    * @param index ID of the object
    * @param rLocalResourceId On output contains ID of the local resource
    * @return Object or NULL if invalid id is given
    */
    const metadata::ResourceTreeNode* getNodeByIndex(size_t index, LocalResourceId& rLocalResourceId) const;

    /**
    * Gets resource metadata with given ID
    *
    * @param localResourceId ID of the local resource
    * @param rLocalResourceId On output contains ID of the local resource
    * @param rMetadata On output contains resource metadata
    * @param rpSubscriptionData On output contains pointer to resource's subscripton information
    * @return Result of the operation
    */
    Result getResourceMetadataByIndex(
        size_t index,
        LocalResourceId& rLocalResourceId,
        ResourceMetadata& rMetadata,
        ResourceSubscriptionData*& rpSubscriptionData) const;

private:
    /** ResourceTree accesses these member variables directly */
    friend class ResourceTree;

    /** Finds ResourceTreeNode with given id without any boundary checking
    *
    * @param localResourceId ID of the resource tree node
    * @return Pointer to ResourceTreeNode or NULL if node was not found
    */
    const metadata::ResourceTreeNode* getNodeByIdUnsafe(LocalResourceId localResourceId) const;

    /** Metadata map instance */
    const MetadataMap& mrMetadataMap;

    /** Number of resources */
    LocalResourceId mNumberOfResources;

    /** Array of metadata tree nodes. */
    const metadata::ResourceTreeNode* mpTreeNodes;

    /** Array of subscription data for id paths. */
    ResourceSubscriptionData* mpResourceSubscriptionData;

    /** Number of entries in the sparse map of resources */
    size_t mNumberOfSparseMapEntries;
    
    /** Sparse map that maps resource IDs to indexes of pTreeNode array */
    const LocalResourceId* mpSparseMap;

    /** Parent resource */
    LocalResourceId mParentResourceId;

    /** Number of mount points */
    uint16 mNumberOfMountPoints;

    /** Array of mount points in the subtree. */
    const metadata::MountPoint* mpMountPoints;

    /** Array of instantiated mount points in the subtree. */
    MountPoint* mpMountPointInstances;

    /** Next resource tree in the same mount point */
    ResourceSubtree* mpNextMountPointSubtree;

    /** Next resource tree in the whole tree */
    ResourceSubtree* mpNextSubtree;
};

} // namespace whiteboard
