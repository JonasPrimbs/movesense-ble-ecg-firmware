#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/metadata/MetadataBuilder.h"
#include "whiteboard/Initialization.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

// Forward declarations
class Whiteboard;
class WhiteboardMockup;

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
    /** Constructor
    *
    * @param executionContextId ID of the execution context that owns the resource
    * @param rWhiteboard Whiteboard instance where this resource is binded to
    */
    ScopedResourceImpl(
        ExecutionContextId executionContextId,
        Whiteboard& rWhiteboard);

    /** Constructor
    *
    * @param rWhiteboard WhiteboardMockup instance where this resource is binded to
    */
    ScopedResourceImpl(
        WhiteboardMockup& rWhiteboard);

    /** Constructor
    *
    * @param executionContextId ID of the execution context that owns the resource
    * @param localResourceId Local resource ID
    * @param rWhiteboard Whiteboard instance where this resource is binded to
    */
    ScopedResourceImpl(
        ExecutionContextId executionContextId,
        LocalResourceId localResourceId,
        Whiteboard& rWhiteboard);

    /** Constructor
    *
    * @param executionContextId ID of the execution context that owns the resource
    * @param localResourceId Local resource ID
    * @param rWhiteboard WhiteboardMockup instance where this resource is binded to
    */
    ScopedResourceImpl(
        ExecutionContextId executionContextId,
        LocalResourceId localResourceId,
        WhiteboardMockup& rWhiteboard);

    /** Intializes the resource 
     *
     * @param name Name of the resource
     * @param accessRights Access rights to the resource
     * @param localDataTypeId ID of the data type
     * @param rMetadataCreator Metadata creator that will be used to create metadata for the resource
     */
    void initialize(
        const char* name,
        metadata::SecurityMask accessRights,
        LocalDataTypeId localDataTypeId,
        IMetadataCreator& rMetadataCreator);

public:
    /** Destructor */
    virtual ~ScopedResourceImpl();

    /** Gets ID of the resource */
    ResourceId getId() const;

private:
    /** Associated Whiteboard instance */
    Whiteboard& mrWhiteboard;

    /** Execution context of the resource */
    ExecutionContextId mExecutionContextId;

    /** ID of the resource */
    LocalResourceId mLocalResourceId;

    /** Dynamically constructed MetadataMap instance that contains metadata for this resource */
    MetadataMap* mpMetadataMap;
};

/**
*	ScopedResourceImpl wrapper for dummy resource.
*/
class ScopedResourceInstantiator : public ScopedResourceImpl
{
public:
    /** Constructor
    *
    * @param executionContextId ID of the execution context that owns the resource
    * @param localResourceId Local resource ID
    * @param rWhiteboard Whiteboard instance where this resource is binded to
    */
    ScopedResourceInstantiator(
        ExecutionContextId executionContextId,
        LocalResourceId localResourceId,
        Whiteboard& rWhiteboard)
        : ScopedResourceImpl(executionContextId,
            localResourceId,
            rWhiteboard)
    {
    }

    /** Constructor
    *
    * @param executionContextId ID of the execution context that owns the resource
    * @param localResourceId Local resource ID
    * @param rWhiteboard WhiteboardMockup instance where this resource is binded to
    */
    ScopedResourceInstantiator(
        ExecutionContextId executionContextId,
        LocalResourceId localResourceId,
        WhiteboardMockup& rWhiteboard)
        : ScopedResourceImpl(executionContextId,
            localResourceId,
            rWhiteboard)
    {
    }
};

/**
*	Test resource class with template parameter T used for variable type in provider operations.
*/
template <typename T, typename METADATA_CREATOR = MetadataCreator>
class ScopedResource : public ScopedResourceImpl
{
public:
    /** Constructor
    *
    * @param name Name of the resource
    * @param executionContextId ID of the execution context that owns the resource
    * @param accessRights Access rights to the resource
    * @param rWhiteboard Whiteboard instance where this resource is binded to
    */
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

    /** Constructor
     *
     * @param executionContextId ID of the execution context that owns the resource
     * @param localResourceId Local resource ID
     * @param rWhiteboard Whiteboard instance where this resource is binded to
     */
    ScopedResource(
        ExecutionContextId executionContextId,
        LocalResourceId localResourceId,
        Whiteboard& rWhiteboard)
        : ScopedResourceImpl(
            executionContextId,
            localResourceId,
            rWhiteboard)
    {
    }

    /** Constructor
    *
    * @param executionContextId ID of the execution context that owns the resource
    * @param localResourceId Local resource ID
    * @param rWhiteboard WhiteboardMockup instance where this resource is binded to
    */
    ScopedResource(
        ExecutionContextId executionContextId,
        LocalResourceId localResourceId,
        WhiteboardMockup& rWhiteboard)
        : ScopedResourceImpl(
            executionContextId,
            localResourceId,
            rWhiteboard)
    {
    }

    /** Constructor
    *
    * @param localResourceId
    */
    /** Constructor
    *
    * @param name Name of the resource
    * @param accessRights Access rights to the resource
    * @param rWhiteboard WhiteboardMockup instance where this resource is binded to
    */
    ScopedResource(
        const char* name,
        metadata::SecurityMask accessRights,
        WhiteboardMockup& rWhiteboard)
        : ScopedResourceImpl(
            rWhiteboard)
    {
        METADATA_CREATOR metadataCreator;
        initialize(name, accessRights, Value::DataTypeId<typename RemoveAll<T>::type>::value, metadataCreator);
    }

    /** Destructor */
    virtual ~ScopedResource()
    { 
    }
};

} // namespace whiteboard

#endif
