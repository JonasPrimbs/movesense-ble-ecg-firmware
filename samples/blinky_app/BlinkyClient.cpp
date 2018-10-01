#include "movesense.h"

#include "BlinkyClient.h"
#include "common/core/debug.h"

#include "ui_ind/resources.h"


const size_t BLINK_PERIOD_MS = 800;

const char* const BlinkyClient::LAUNCHABLE_NAME = "Blinky";

BlinkyClient::BlinkyClient()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION)
{
    mTimer = whiteboard::ID_INVALID_TIMER;
}

BlinkyClient::~BlinkyClient()
{
}

bool BlinkyClient::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void BlinkyClient::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

/** @see whiteboard::ILaunchableModule::startModule */
bool BlinkyClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Start LED timer. true = trigger repeatedly
    mTimer = startTimer(BLINK_PERIOD_MS, true);

    return true;
}

/** @see whiteboard::ILaunchableModule::startModule */
void BlinkyClient::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = whiteboard::ID_INVALID_TIMER;
}

void BlinkyClient::onTimer(whiteboard::TimerId timerId)
{
    if (timerId != mTimer)
    {
        return;
    }

    uint16_t indicationType = 2; // SHORT_VISUAL_INDICATION, defined in ui/ind.yaml

    // Make PUT request to trigger led blink
    asyncPut(WB_RES::LOCAL::UI_IND_VISUAL::ID, AsyncRequestOptions::Empty, indicationType);
}
