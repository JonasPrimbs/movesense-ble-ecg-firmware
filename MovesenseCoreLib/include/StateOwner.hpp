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
    virtual bool onStateSubscribed() { return true; }

    /** Called when there is no more subscribers related to registered event source. */
    virtual void onStateUnsubscribed() {}

    /** Called when there is get request to registered event source. */
    virtual void onStateGet() {}

    /** Get current state. */
    State getCurrentState() const
    {
        return mCurrentState;
    }

    /** Get state ID assigned to this state owner. */
    StateId getStateId() const
    {
        return mStateId;
    }

protected:

    StateOwner(const StateId stateId, const State& initialState) :
        mStateId(stateId),
        mCurrentState(initialState)
    {}

    StateOwner(const StateId stateId) : StateOwner(stateId, 0) {}

    virtual ~StateOwner() {}


    /** Update state. If state differs, generate an event and notify subscribers. */
    void updateState(const State& newState)
    {
        if (mCurrentState != newState)
        {
            mCurrentState = newState;
            StateChangeParams params = {this, mStateId, newState};

            StatesService* ss = StatesService::getInstance();
            if (ss) ss->notifyStateChange(params);
        }
    }

    /** Attach to an existing StatesService. */
    void attachToStatesService()
    {
        StatesService* ss = StatesService::getInstance();
        if (ss) ss->attachStateOwner(this, mStateId);
    }

    /** Detach from an existing StatesService. */
    void detachFromStatesService()
    {
        StatesService* ss = StatesService::getInstance();
        if (ss) ss->detachStateOwner(this, mStateId);
    }

private:
    State mCurrentState;
    const StateId mStateId;
};
