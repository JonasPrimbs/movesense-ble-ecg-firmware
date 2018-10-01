// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include "StatesService.hpp"
#include "system_states/resources.h"


class StateOwner
{
    // Note: These methods have the stateId as a parameter to make multi-inheritance possible
public:
    /**
    * Called when new subscription related with registered event source appears.
    * May return false if event source cannot be activated, otherwise it should return true.
    */
    virtual bool onStateSubscribed(StateId stateId) =0;

    /** Called when there is no more subscribers related to registered event source. */
    virtual void onStateUnsubscribed(StateId stateId) =0;

    /** Called when there is get request to registered event source. */
    virtual void onStateGet(StateId stateId) =0;

    /** Get current state. */
    virtual State getCurrentState(StateId stateId) const =0;
};

template <StateId STATE_ID, bool IS_TRANSIENT = false>
class StateOwnerImpl : public StateOwner
{
public:

    /** Get state ID assigned to this state owner. */
    StateId getStateId() const
    {
        return STATE_ID;
    }

    virtual ~StateOwnerImpl<STATE_ID, IS_TRANSIENT>() {}

    /** Attach to an existing StatesService. */
    void attachToStatesService()
    {
        StatesService* ss = StatesService::getInstance();
        if (ss) ss->attachStateOwner(this, STATE_ID);
    }

    /** Detach from an existing StatesService. */
    void detachFromStatesService()
    {
        StatesService* ss = StatesService::getInstance();
        if (ss) ss->detachStateOwner(this, STATE_ID);
    }

    /** Update state. If state differs, generate an event and notify subscribers. */
    void updateState(const State& newState)
    {
        if (IS_TRANSIENT || mCurrentState != newState)
        {
            mCurrentState = newState;
            DEBUGLOG("updateState [%u]: %u", getStateId(), mCurrentState);
            
            StateChangeParams params = {this, STATE_ID, newState};

            StatesService* ss = StatesService::getInstance();
            if (ss) ss->notifyStateChange(params);
        }
    }

protected:
    StateOwnerImpl<STATE_ID, IS_TRANSIENT>() : StateOwnerImpl<STATE_ID, IS_TRANSIENT>(0) {}

    StateOwnerImpl<STATE_ID, IS_TRANSIENT>(const State& initialState) :
        mCurrentState(initialState)
    {}

    /** Get current state. */
    State mCurrentState;
    State getCurrentState() const {return mCurrentState;}
public:

    /** Get current state (from StateOwner). 
        NOTE: If you use multi-inheritance of StateOwnerImpl, you MUST override this in your class
    */
    virtual State getCurrentState(StateId stateId) const OVERRIDE {return mCurrentState;}

};
