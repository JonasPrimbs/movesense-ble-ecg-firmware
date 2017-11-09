// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include "StatesService.hpp"
#include "system_states/resources.h"


class StateOwner
{
public:

    /**
    * Called when new subscription related with registered event source appears.
    * May return false if event source cannot be activated, otherwise it should return true.
    */
    virtual bool onStateSubscribed(const StateId& stateId) { return true; }

    /** Called when there is no more subscribers related to registered event source. */
    virtual void onStateUnsubscribed(const StateId& stateId) {}

    /** Needed by StatesService. */
    whiteboard::ExecutionContextId getExecutionContextId() { return mExecutionContextId; }

    /** Get current state. */
    State getCurrentState() const
    {
        return mCurrentState;
    }

protected:

    StateOwner(whiteboard::ExecutionContextId contextId, const State& initialState) :
        mExecutionContextId(contextId),
        mCurrentState(initialState)
    {}

    StateOwner(whiteboard::ExecutionContextId contextId) : StateOwner(contextId, 0) {}

    virtual ~StateOwner() {}


    /** Update state. If state differs, generate an event and notify subscribers. */
    void updateState(const StateId& stateId, const State& newState)
    {
        if (mCurrentState != newState)
        {
            mCurrentState = newState;
            StateChangeParams params = {this, stateId, newState};

            StatesService::getInstance()->notifyStateChange(params);
        }
    }

    /** Attach to an existing StatesService. */
    void attachToStatesService(const StateId &stateId)
    {
        StatesService::getInstance()->attachStateOwner(this, stateId);
    }

    /** Detach from an existing StatesService. */
    void detachFromStatesService(const StateId &stateId)
    {
        StatesService::getInstance()->detachStateOwner(this, stateId);
    }

private:
    whiteboard::ExecutionContextId mExecutionContextId;
    State mCurrentState;
};
