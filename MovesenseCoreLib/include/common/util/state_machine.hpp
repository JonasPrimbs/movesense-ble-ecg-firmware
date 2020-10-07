#pragma once

#include <stdint.h>
#include "common/core/debug.h"
#include "common/core/dbgassert.h"

/******************************************************************************
    Copyright (c) Suunto Oy 2018.
    All rights reserved.
******************************************************************************/

#if !defined(FSM_TRACE) && defined(FSM_TRACE_ENABLED)
#define FSM_TRACE DEBUGLOG
#else
#define FSM_TRACE(...)                                                                                                            \
    do                                                                                                                           \
    {                                                                                                                            \
    } while (0)
#endif //FSM_TRACE_ENABLED

// This file defines macros that help to create a finite state machine class.
// Define the state machine with the macros and then inherit from the generated class and
// Override the transition handlers "onTransition_xxx_yyy(EVENT_TYPE eventType, EVENT_PARAM event_param)"
// Override the transition handlers "onTransition_xxx_yyy(EVENT_TYPE eventType, EVENT_PARAM event_param)"
// Override the transition handlers "onTransition_xxx_yyy(EVENT_TYPE eventType, EVENT_PARAM event_param)"

// FSM_BEGIN & FSM_END frame the state machine definition
#define FSM_BEGIN(name) class FSMBase_##name {\
protected:\
    volatile bool mIsInsideHandleEvent;\
    FSMBase_##name() : mIsInsideHandleEvent(false), mNextEvtType(__UNDEFINED), mNextEvtParam(0) {}\
    virtual void onReset() {}\
public:\
    typedef uint32_t EVENT_PARAM;\
    enum EVENT_TYPE {__UNDEFINED, __RESET, GET,POST,PUT,DELETE,SUBSCRIBE,UNSUBSCRIBE,TIMER,OTHER};

// State block macros. Define the states and defaul state of the state machine
#define FSM_STATES_BEGIN \
public:\
    typedef uint8_t __STATE_DEF;\
    static const __STATE_DEF __INVALID_STATE = (__COUNTER__);

#define FSM_DEFINE_STATE(stateName) \
    static const __STATE_DEF stateName = (__COUNTER__);\
    virtual void onEnter_##stateName(EVENT_TYPE eventType, EVENT_PARAM event_param) {FSM_TRACE(__FUNCTION__);}\
    virtual void onLeave_##stateName(EVENT_TYPE eventType, EVENT_PARAM event_param) {FSM_TRACE(__FUNCTION__);}

#define FSM_STATES_END(firstState) \
private:\
    __STATE_DEF mCurrentState = firstState;\
    volatile EVENT_TYPE mNextEvtType;\
    EVENT_PARAM mNextEvtParam;\
protected:\
    inline bool popNextEvt(EVENT_TYPE &evt_type_out, uint32_t &evt_param_out) {if (mNextEvtType == __UNDEFINED) return false; evt_param_out = mNextEvtParam; evt_type_out = mNextEvtType; mNextEvtType = __UNDEFINED; return true;}\
    inline void __setCurrentState(__STATE_DEF newState) {mCurrentState=newState;}\
public:\
    const __STATE_DEF FIRST_STATE = firstState;\
    inline __STATE_DEF getCurrentState() const {return mCurrentState;}\
    virtual void reset() FINAL {FSM_TRACE(__FUNCTION__); postEvent(__RESET, 0);}\
    virtual void postEvent(EVENT_TYPE eventType, EVENT_PARAM event_param = 0)\
    {\
        if (!mIsInsideHandleEvent)\
            __handleEvent(eventType, event_param);\
        else {\
            ASSERT(mNextEvtType == __UNDEFINED);\
            mNextEvtType = eventType; mNextEvtParam = event_param;\
        }\
    }\
private:



#define FSM_TRANSITION_DEF(from, to) \
protected:\
    virtual void onTransition_##from##_##to(EVENT_TYPE evt, EVENT_PARAM event_param) {FSM_TRACE(__FUNCTION__);}

#define FSM_ABSTRACT_TRANSITION_TRIGGERS \
protected:\
virtual bool __handleEvent(EVENT_TYPE eventType, uint32_t event_param) = 0;

#define FSM_TRANSITION_TRIGGERS_BEGIN \
private:\
    virtual bool __handleEvent(EVENT_TYPE eventType, uint32_t event_param) {\
        mIsInsideHandleEvent = true;\
        FSM_TRACE("handleEvent: event=%u, param: %u", eventType, event_param); \
        bool stateChanged = false;\
        __STATE_DEF nextState = __INVALID_STATE;\
        if(eventType==__RESET) {\
            nextState=FIRST_STATE;\
            onReset();\
            stateChanged = true;\
        }

#define FSM_TRANSITION_TRIGGER(from, to, evt, condition) \
        if(!stateChanged && (from)==getCurrentState() && (evt)==eventType && (condition)) {\
            onLeave_##from(evt, event_param);\
            onTransition_##from##_##to(evt, event_param);\
            onEnter_##to(evt, event_param);\
            nextState=(to);\
            stateChanged = true;\
        }

#define FSM_TRANSITION_TRIGGERS_END \
        if (stateChanged) {__setCurrentState(nextState);}\
        else {FSM_TRACE("no transition from state [%u] for: event %u, param %u", getCurrentState(), eventType, event_param);}\
        mIsInsideHandleEvent = false;\
        EVENT_TYPE evt;uint32_t param;\
        if (popNextEvt(evt, param)) { __handleEvent(evt, param); }\
        return stateChanged;\
    };


#define FSM_END };
