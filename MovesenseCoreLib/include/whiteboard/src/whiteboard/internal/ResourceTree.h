#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/internal/ResourceSubtree.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/Result.h"

namespace whiteboard
{
namespace internal
{

/**
*	Resource tree.
*/
class ResourceTree FINAL
{
public:

    /**
    *	Constructor.
    */
    ResourceTree();

    /**
    *	Destructor.
    **/
    ~ResourceTree();

    /** Clears the resource tree and deletes all subtrees */
    void clearAndDelete();

    /**
    * Adds a resource subtree to the given mount point.
    *
    * @param parentResourceId Id of the parent resource a.k.a. the mount point
    * @param rSubtree Subtree to add
    * @return Result of the operation
    */
    Result addSubtree(LocalResourceId parentResourceId, ResourceSubtree& rSubtree);

    /**
    * Removes subtree from resource tree
    *
    * @param rpSubtree Subtree to remove
    * @return Result of the operation
    */
    Result removeSubtree(ResourceSubtree& rSubtree);

    /**
    * Removes all subtrees from given mount point and underneath it.
    *
    * @param parentResourceId Id of the parent resource a.k.a. the mount point
    * @return Result of the operation
    */
    Result removeSubtrees(LocalResourceId parentResourceId);

    /** Gets first resource subtree
     *
     * @return Pointer to first mounted resource subtree
     */
    ResourceSubtree* getFirstResourceSubtree();

    /** Finds resource subtree with specified metadata map instance
    *
    * @param rMetadataMap Metadata map of the resource tree instance
    * @return Pointer to subtree with given metadata
    */
    ResourceSubtree* findSubtree(const MetadataMap& rMetadataMap);

    /**
    * Gets resource metadata with given ID
    *
    * @param localResourceId ID of the local resource
    * @param rMetadata On output contains resource metadata
    * @param rSubscriptionData On output contains resource's subscription information
    * @return Result of the operation
    */
    Result getResourceById(
        LocalResourceId localResourceId, ResourceMetadata& rMetadata, ResourceSubscriptionData& rSubscriptionData) const;

    /** Gets full path of a resource
    *
    * @param localResourceId ID of the resource which path should be formatted
    * @param path On output contains full path of the resource
    * @return A value indicating whether the resource was found
    */
    bool getFullResourcePath(LocalResourceId localResourceId, char path[WB_MAX_RESOURCE_PATH_LEN]);

    /**
    *	Gets a resource ID by full resource path
    *
    *	@param pFullPath Full path of the resource
    *   @param clientId In case of path request originating from clientId should be
    *          provided so that the client pathVariables are cached at the same time.
    *	@return Resource ID with given path or ID_INVALID_RESOURCE if it is not found.
    */
    ResourceId getResourceByPath(const char* pFullPath, ClientId clientIdForPathParameterCaching = ID_INVALID_CLIENT);

    /** Iterator class for iterating resources */
    class Iterator
    {
    public:
        /** Constructor
         *
         * @param pFirstResourceSubtree First subtree for iteration
         */
        Iterator(ResourceSubtree* pFirstResourceSubtree);

        /** Constructor
        *
        * @param pResourceSubtree Subtree for iteration
        * @param resourceIndex Index of the resource in the subtree
        */
        Iterator(ResourceSubtree* pResourceSubtree, size_t resourceIndex);

        /** Gets ID of the resource the iterator is pointing at
        *
        * @return ID of the resource or ID_INVALID_LOCAL_RESOURCE if iterator is invalid
        */
        LocalResourceId operator*() const;

        /** Prefix increment operator for advancing the iterator
         *
         * return Reference to this iterator
         */
        Iterator& operator++();

        /**
        * Gets resource metadata with given ID
        *
        * @param rMetadata On output contains resource metadata
        * @param rSubscriptionData On output contains resource's subscription information
        * @return Result of the operation
        */
        Result getResourceById(
            ResourceMetadata& rMetadata, ResourceSubscriptionData& rSubscriptionData) const;

    private:
        /** Current resource subtree */
        ResourceSubtree* mpResourceSubtree;

        /** Index of current resource */
        size_t mResourceIndex;
    };

    /** Gets new iterator instance
    *
    * @return New iterator that points to first resource
    */
    Iterator begin();

    /** Checks whether subtree has any resources with
    * given resource API ID.
    *
    * @param apiId ID of the API which resources should be checked
    * @return A value indicating whether API exists
    */
    bool hasApiId(metadata::ApiId apiId) const;

private:
    /** Finds ResourceTreeNode with given id from all subtrees
     *
     * @param localResourceId ID of the resource tree node
     * @param rpSubtree On output contains pointer to subtree
     * @return Pointer to ResourceTreeNode or NULL if node was not found
     */
    const metadata::ResourceTreeNode* getNodeById(LocalResourceId localResourceId, ResourceSubtree*& rpSubtree);

    /** Gets mount point pointed by given resource ID.
     *
     * @param localResourceId Id of the mount point resource
     * @param pMountPoint On output contains reference to the mount point
     * @return Result of the operation
     */
    Result getMountPoint(LocalResourceId localResourceId, MountPoint*& rpMountPoint);

private:
    /** Root of the resource tree */
    MountPoint mRootMountPoint;

    /** First registered resource subtree in the tree */
    ResourceSubtree* mpFirstRegistered;
};

} // namespace internal
} // namespace whiteboard
