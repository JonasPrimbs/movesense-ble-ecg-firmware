#include "ConnectionScanner.h"
#include "common/core/debug.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "whiteboard/integration/bsp/shared/debug.h"

#include "meas_hr/resources.h"
#include "misc_gear/resources.h"
#include "system_states/resources.h"
#include "ui_ind/resources.h"

const char* const ConnectionScanner::LAUNCHABLE_NAME = "ConnectionScanner";

ConnectionScanner::ConnectionScanner()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      mSamplesCounter(0),
      mLedBlinkTimer(whiteboard::ID_INVALID_TIMER),
      mHRCheckOffTimer(whiteboard::ID_INVALID_TIMER),
      mBlinkCount(0),
      mDetectionState(DEVICE_DISCONNECTED)
{
}

ConnectionScanner::~ConnectionScanner()
{
}

bool ConnectionScanner::initModule()
{
    DEBUGLOG("ConnectionScanner::initModule");

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void ConnectionScanner::deinitModule()
{
    DEBUGLOG("ConnectionScanner::deinitModule");

    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool ConnectionScanner::startModule()
{
    DEBUGLOG("ConnectionScanner::startModule");

    mModuleState = WB_RES::ModuleStateValues::STARTED;
    startRunning();
    return true;
}

void ConnectionScanner::onTimer(whiteboard::TimerId timerId)
{
    DEBUGLOG("ConnectionScanner::onTimer");

    if (timerId == mLedBlinkTimer)
    {
        mLedBlinkTimer = whiteboard::ID_INVALID_TIMER;
        uint16_t indicationType = 2; // SHORT_VISUAL_INDICATION, defined in ui/ind.yaml

        // Make PUT request to trigger led blink
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL::ID, AsyncRequestOptions::Empty, indicationType);
        if (--mBlinkCount)
        {
            mLedBlinkTimer = startTimer(BLINK_TIME, false);
        }
    }

    else if (timerId == mHRCheckOffTimer)
    {
        //TIMEOUT ON HR
        mHRCheckOffTimer = whiteboard::ID_INVALID_TIMER;
        asyncUnsubscribe(WB_RES::LOCAL::MEAS_HR());
        updateDetectionState(UNKNOWN_DEVICE);
    }
}

// This callback is called when the acceleration resource notifies us of new data
void ConnectionScanner::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                                 const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("ConnectionScanner::onNotify");

    // Confirm that it is the correct resource
    switch (resourceId.localResourceId)
    {

    case WB_RES::LOCAL::SYSTEM_STATES_STATEID::LID:
    {
        const WB_RES::StateChange& stateChange = value.convertTo<const WB_RES::StateChange&>();
        if (stateChange.newState == 0)
        {
            updateDetectionState(DEVICE_DISCONNECTED);
        }
        else if (stateChange.newState == 1)
        {
            updateDetectionState(DEVICE_CONNECTED);
        }
    }
    break;

    case WB_RES::LOCAL::MEAS_HR::LID:
    {
        mSamplesCounter++;
        DEBUGLOG("ConnectionScanner::onNotify, HRBelt connected (or noise)");

        if (mSamplesCounter > 2)
        {
            stopTimer(mHRCheckOffTimer);
            mHRCheckOffTimer = whiteboard::ID_INVALID_TIMER;
            asyncUnsubscribe(WB_RES::LOCAL::MEAS_HR());
            updateDetectionState(HR_DETECTED);
        }
    }
    break;

    default:
    {
        break;
    }
    }
}

void ConnectionScanner::onGetResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& result)
{
    DEBUGLOG("ConnectionScanner::onGetResult");
    if (resultCode != wb::HTTP_CODE_OK)
    {
        DEBUGLOG("ConnectionScanner::onGetResult, Wrong result code");
        return;
    }

    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::MISC_GEAR_ID::LID:
    {
        uint64_t gearID = result.convertTo<uint64_t>();
        DEBUGLOG("ConnectionScanner::onGetResult, GearID: %u", gearID);
        if (gearID != 0)
        {
            updateDetectionState(GEAR_DETECTED);
        }
        else
        {
            updateDetectionState(GEAR_NOT_FOUND);
        }
    }
    break;

    default:
        break;
    }
}

void ConnectionScanner::startRunning(void)
{
    DEBUGLOG("ConnectionScanner::startRunning");

    whiteboard::Result result = getResource("System/States/2", mSystemStateResourceId); // 2 = Connector
    if (result == wb::HTTP_CODE_OK)
    {
        asyncSubscribe(mSystemStateResourceId, NULL);
    }
    else {
        DEBUGLOG("ConnectionScanner::startRunning ERROR");
    }
}

void ConnectionScanner::stopRunning(void)
{
    DEBUGLOG("ConnectionScanner::stopRunning");

    asyncUnsubscribe(mSystemStateResourceId, NULL);
    whiteboard::Result result = releaseResource(mSystemStateResourceId); // 2 = Connector
    if (result != wb::HTTP_CODE_OK)
    {
        DEBUGLOG("ConnectionScanner::stopRunning ERROR");
    }
}

void ConnectionScanner::updateDetectionState(DetectionState newState)
{
    switch (newState)
    {
    case DEVICE_CONNECTED:
        DEBUGLOG("ConnectionScanner::updateDetectionState DEVICE_CONNECTED");
        //React only if was disconnected
        if (mDetectionState != DEVICE_DISCONNECTED)
        {
            //skip
            return;
        }
        newState = GEAR_DETECTING;
    case GEAR_DETECTING:
        //Check GEARID
        DEBUGLOG("ConnectionScanner::updateDetectionState GEAR_DETECTING");
        asyncGet(WB_RES::LOCAL::MISC_GEAR_ID()); //GEAR_DETECTED, GEAR_NOT_FOUND
        break;
    case GEAR_DETECTED:
        DEBUGLOG("ConnectionScanner::updateDetectionState GEAR_DETECTED");
        startBlinking(2);
        newState = WAITING;
        break;
    case GEAR_NOT_FOUND:
        DEBUGLOG("ConnectionScanner::updateDetectionState GEAR_NOT_FOUND");
        newState = HR_DETECTING;
    case HR_DETECTING:
        mSamplesCounter = 0;
        DEBUGLOG("ConnectionScanner::updateDetectionState HR_DETECTING");
        asyncSubscribe(WB_RES::LOCAL::MEAS_HR()); //HR_DETECTED HR_NOT_FOUND
        mHRCheckOffTimer = startTimer(HR_TIMEOUT, false);
        break;
    case HR_DETECTED:
        DEBUGLOG("ConnectionScanner::updateDetectionState HR_DETECTED");
        startBlinking(4);
        break;
    case HR_NOT_FOUND:
        DEBUGLOG("ConnectionScanner::updateDetectionState HR_NOT_FOUND");
        newState = UNKNOWN_DEVICE;
    case UNKNOWN_DEVICE:
        DEBUGLOG("ConnectionScanner::updateDetectionState UNKNOWN_DEVICE");
        startBlinking(5);
        break;
    case WAITING:
        DEBUGLOG("ConnectionScanner::updateDetectionState WAITING");
        break;
    case DEVICE_DISCONNECTED:
        DEBUGLOG("ConnectionScanner::updateDetectionState DEVICE_DISCONNECTED");
    }
    mDetectionState = newState;
}
