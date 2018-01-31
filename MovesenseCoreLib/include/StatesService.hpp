// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include "common/core/dbgassert.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceProvider.h>

#include "system_states/resources.h"


class StateOwner;
typedef WB_RES::StateId::Type StateId;
typedef WB_RES::State State;

struct StateChangeParams
{
    StateOwner* owner;
    StateId stateId;
    State newState;
};

class StateSlot
{
public:
    StateOwner* owner;
    bool isActive;

    StateSlot() : owner(nullptr), isActive(false) {}
};


class StatesService FINAL :
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    StatesService();
    ~StatesService();

    static StatesService* getInstance() { return spInstance; }


    /** Called by StateOwner: produces an event that is then passed to subscribers. */
    void notifyStateChange(const StateChangeParams& params);


    void attachStateOwner(StateOwner* owner, const StateId& stateId)
    {
        StateSlot* slot = getStateSlot(stateId);
        ASSERT(slot);

        if (slot) slot->owner = owner;
    }

    void detachStateOwner(StateOwner* owner, const StateId& stateId)
    {
        StateSlot* slot = getStateSlot(stateId);
        ASSERT(slot);

        if (slot && slot->owner == owner) slot->owner = nullptr;
    }

private:

    /** List of available event slots.
        Slots defined here should match API spec from system/states.yaml
        and be in sync with method getStateSlot() and forEachSlot().

        When a new StateId item is defined in states.yaml, these steps should be made:
        - add new entry in struct slots,
        - add new case to switch inside getStateSlot(),
        - add new function() call inside forEachSlot(). */
    struct
    {
        StateSlot movement;
        StateSlot batteryStatus;
        StateSlot connector;

    } slots;


    StateSlot* getStateSlot(const StateId& stateId)
    {
        switch (stateId)
        {
        case StateId::MOVEMENT:
            return &slots.movement;

        case StateId::BATTERYSTATUS:
            return &slots.batteryStatus;

        case StateId::CONNECTOR:
            return &slots.connector;

        default:
            return nullptr;
        }
    }

    typedef void (*StateSlotFunctor)(StateSlot* slot, const StateId& sid);

    /** Used to perform action on every available slot. */
    void forEachSlot(StateSlotFunctor function)
    {
        function(&slots.movement, StateId::MOVEMENT);
        function(&slots.batteryStatus, StateId::BATTERYSTATUS);
        function(&slots.connector, StateId::CONNECTOR);
    }


    // singleton instance to allow loose coupling with StateOwners
    static StatesService* spInstance;


    void queueNotifySubscribers(const StateChangeParams& params);
    void notifySubscribers(const StateChangeParams& params);

    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    virtual void onGetRequest(const wb::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
    virtual void onSubscribe(const wb::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
    virtual void onUnsubscribe(const wb::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onRemoteWhiteboardDisconnected(whiteboard::WhiteboardId whiteboardId) OVERRIDE;

    virtual void onClientUnavailable(whiteboard::ClientId clientId) OVERRIDE;

    void checkAndDeactivateSlots();

};
