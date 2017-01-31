#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/metadata/MetadataBuilder.h"
#include "whiteboard/Initialization.h"

namespace whiteboard
{

// Forward declarations
class Whiteboard;

/** Default metadata creator
*
* T = get()
* put(T)
* post(T)
* delete()
* subscribe()
* unsubscribe()
* notify(T)
*/
class IMetadataCreator
{
public:
    /** Creates metadata for ScopedResource using given MetadataBuilder instance
    *
    * @param b Builder instance
    * @param name Name of the resource
    * @param executionContextId ID of the execution context
    * @param accessRights Default access rights for all operations
    * @param localDataTypeId ID of the data type associated with the resource value
    * @return ID of the new resource
    */
    virtual LocalResourceId createResource(
        MetadataBuilder& b,
        const char* name,
        ExecutionContextId executionContextId,
        metadata::SecurityMask accessRights,
        LocalDataTypeId localDataTypeId) const = 0;
};

/** Default metadata creator
 *
 * T = get()
 * put(T)
 * post(T)
 * delete()
 * subscribe()
 * unsubscribe()
 * notify(T)
 */
class MetadataCreator : public IMetadataCreator
{
public:
    /** Default constructor */
    MetadataCreator()
    {
    }

    /** Destructor */
    ~MetadataCreator()
    {
    }

    /** Creates metadata for ScopedResource using given MetadataBuilder instance
     *
     * @param b Builder instance
     * @param name Name of the resource
     * @param executionContextId ID of the execution context
     * @param accessRights Default access rights for all operations
     * @param localDataTypeId ID of the data type associated with the resource value
     * @return ID of the new resource
     */
    LocalResourceId createResource(
        MetadataBuilder& b,
        const char* name,
        ExecutionContextId executionContextId,
        metadata::SecurityMask accessRights,
        LocalDataTypeId localDataTypeId) const;
};

/**
*	Implementation base class for test resource classes
*/
class ScopedResourceImpl
{
protected:
    ScopedResourceImpl(
        ExecutionContextId executionContextId,
        Whiteboard& rWhiteboard);

    void initialize(
        const char* name,
        metadata::SecurityMask accessRights,
        LocalDataTypeId localDataTypeId,
        IMetadataCreator& rMetadataCreator);

public:
    virtual ~ScopedResourceImpl();

    ResourceId getId() const;

private:
    Whiteboard& mrWhiteboard;
    ExecutionContextId mExecutionContextId;
    LocalResourceId mLocalResourceId;
    ResourceSubtree* mpResourceSubtree;
};

/**
*	Test resource class with template parameter T used for variable type in provider operations.
*/
template <typename T, typename METADATA_CREATOR = MetadataCreator>
class ScopedResource : public ScopedResourceImpl
{
public:
    ScopedResource(
        const char* name,
        ExecutionContextId executionContextId,
        metadata::SecurityMask accessRights = 0,
        Whiteboard& rWhiteboard = getWhiteboardInstance())
        : ScopedResourceImpl(
            executionContextId,
            rWhiteboard)
    {
        METADATA_CREATOR metadataCreator;
        initialize(name, accessRights, Value::DataTypeId<typename RemoveAll<T>::type>::value, metadataCreator);
    }

    virtual ~ScopedResource()
    { 
    }
};

} // namespace whiteboard
