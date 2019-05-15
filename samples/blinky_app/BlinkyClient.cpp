#include "movesense.h"

#include "BlinkyClient.h"
#include "common/core/debug.h"

#include "ui_ind/resources.h"


const size_t BLINK_PERIOD_MS = 800;

const char* const BlinkyClient::LAUNCHABLE_NAME = "Blinky";

BlinkyClient::BlinkyClient()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      mTimer(wb::ID_INVALID_TIMER)
{
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

bool BlinkyClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    mTimer = startTimer(BLINK_PERIOD_MS, true); // Start LED timer. true = trigger repeatedly
    return true;
}

void BlinkyClient::stopModule()
{
    stopTimer(mTimer); // Stop LED timer
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void BlinkyClient::onTimer(wb::TimerId /*timerId*/)
{
    // if only one timer started, no need to check the ID
    const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION; // defined in ui/ind.yaml
    asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type); // PUT request to trigger led blink
    // ignore the immediate and asynchronous Results as this is not critical
}
