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
    StateId id;
    StateOwner* owner;
    bool isActive;

    StateSlot(StateId id) : id(id), owner(nullptr), isActive(false) {}
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

    StateSlot* const slots;
    const uint8_t slotsCount;

    StateSlot* getStateSlot(const StateId& stateId)
    {
        for (uint8_t i = 0; i < slotsCount; ++i)
        {
            if (slots[i].id == stateId)
            {
                return &slots[i];
            }
        }
        return nullptr;
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
