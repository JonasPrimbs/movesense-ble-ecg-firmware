#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

// Forward declarations
class LocalExecutionContext;
class DynamicExecutionContext;

/** Map of execution contexts */
class ExecutionContextMap FINAL
{
public:
    /**
    *	Initializes a new instance of the ExecutionContextMap class
    *
    *	@param numberOfExecutionContexts Number of WBRES generated execution contexts
    *	@param pExecutionContexts List of WBRES generated execution contexts
    */
    ExecutionContextMap(size_t numberOfExecutionContexts, const metadata::ExecutionContextInfo* pExecutionContexts);

    /** Destructor */
    ~ExecutionContextMap();

    /** Gets number of execution contexts
    *
    * @return Number of execution contexts
    */
    inline size_t getNumberOfExecutionContexts() const
    {
        return mNumberOfExecutionContexts + mNumberOfDynamicExecutionContexts;
    }

    /** Returns a execution context by ID
    *
    * @param id ID of the execution context
    * @return Execution context or NULL if invalid id is given
    */
    LocalExecutionContext* getExecutionContextById(ExecutionContextId id) const;

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
        size_t mIndex;

        /** Current dynamic execution context */
        DynamicExecutionContext* mpCurrentDynamic;
    };

    /** Gets new iterator instance
    *
    * @return New iterator that points to first execution context
    */
    Iterator begin();

private:
    /** Number of WBRES generated execution contexts */
    size_t mNumberOfExecutionContexts;

    /** List of WBRES generated execution contexts */
    const metadata::ExecutionContextInfo* mpExecutionContexts;
    
    /** List of constructed execution contexts */
    LocalExecutionContext* mpExecutionContextInstances;

    /** Number of dynamic execution contexts added */
    size_t mNumberOfDynamicExecutionContexts;

    /** First registered dynamic execution context */
    DynamicExecutionContext* mpFirstDynamicExecutionContext;
};

} // namespace whiteboard
