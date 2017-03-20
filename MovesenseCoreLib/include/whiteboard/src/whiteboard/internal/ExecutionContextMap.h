#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/ResourceTree.h"
#include "whiteboard/Result.h"
#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

// Forward declarations
class LocalExecutionContext;
class DynamicExecutionContext;
class MetadataMap;

typedef ExecutionContextId ExecutionContextCount;

/** Map of execution contexts */
class ExecutionContextMap FINAL
{
public:
    /**
    *	Initializes a new instance of the ExecutionContextMap class
    */
    ExecutionContextMap();

    /** Destructor */
    ~ExecutionContextMap();

    /** Gets number of execution contexts
    *
    * @return Number of execution contexts
    */
    inline ExecutionContextCount getNumberOfExecutionContexts() const
    {
        return 
            mNumberOfExecutionContexts 
#ifdef WB_UNITTEST_BUILD
            + mNumberOfDynamicExecutionContexts
#endif
            ;
    }

    /** Returns a execution context by ID
    *
    * @param id ID of the execution context
    * @return Execution context or NULL if invalid id is given
    */
    LocalExecutionContext* getExecutionContextById(ExecutionContextId id) const;

    /** Adds WBRES generated execution contexts to the map
    *
    * @param numberOfExecutionContexts Number of WBRES generated execution contexts
    * @param pExecutionContexts List of WBRES generated execution contexts
    * @param hookInstaller Optional. Hook installer function that installs execution context hooks.
    * @return Result of the operation
    */
    Result add(const MetadataMap& rMetadataMap, ResourceTree::HookInstallerFunc* hookInstaller);

    /** Removes WBRES generated execution contexts from the map
    *
    * @param numberOfExecutionContexts Number of WBRES generated execution contexts
    * @param pExecutionContexts List of WBRES generated execution contexts
    * @return Result of the operation
    */
    Result remove(size_t numberOfExecutionContexts, const metadata::ExecutionContext* pExecutionContexts);

#ifdef WB_UNITTEST_BUILD
    /** Adds execution context to the map
    *
    * @param pExecutionContext that should be added to map
    * @return The new executionContextId for the context.
    */
    ExecutionContextId add(DynamicExecutionContext* pExecutionContext);

    /** Removes execution context from the map
    *
    * @param pExecutionContext that should be removed from the map
    */
    void remove(DynamicExecutionContext* pExecutionContext);
#endif

    /** Iterator class for iterating execution contexts */
    class Iterator
    {
    public:
        /** Constructor
        *
        * @param rOwner Owning exectuion context map instance
        */
        Iterator(ExecutionContextMap& rOwner);

        /** Gets ID of the resource the iterator is pointing at
        *
        * @return Pointer to current execution context or NULL if iterator is invalid
        */
        LocalExecutionContext* operator*() const;

        /** Prefix increment operator for advancing the iterator
        *
        * return Reference to this iterator
        */
        Iterator& operator++();

    private:
        /** Current resource subtree */
        ExecutionContextMap& mrOwner;

        /** Current execution context index (used for WBRES generated execution contexts) */
        ExecutionContextCount mIndex;

#ifdef WB_UNITTEST_BUILD
        /** Current dynamic execution context */
        DynamicExecutionContext* mpCurrentDynamic;
#endif
    };

    /** Gets new iterator instance
    *
    * @return New iterator that points to first execution context
    */
    Iterator begin();

private:
    /** Number of WBRES generated execution contexts */
    ExecutionContextCount mNumberOfExecutionContexts;
    
    /** List of constructed execution contexts */
    LocalExecutionContext** mpExecutionContextInstances;

#ifdef WB_UNITTEST_BUILD
    /** Number of dynamic execution contexts added */
    ExecutionContextCount mNumberOfDynamicExecutionContexts;

    /** First registered dynamic execution context */
    DynamicExecutionContext* mpFirstDynamicExecutionContext;
#endif
};

} // namespace whiteboard
