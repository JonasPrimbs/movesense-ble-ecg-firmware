#include "movesense.h"


#include "BlinkyClient.h"
#include "common/core/debug.h"

#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "app-resources/resources.h"
#include "suunto_device_component_led/resources.h"

#include <float.h>
#include <math.h>

const size_t BLINK_TIME_MS  = 400;


const char* const BlinkyClient::LAUNCHABLE_NAME = "Blinky";



BlinkyClient::BlinkyClient()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION)
{
    mCounter = 0;
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
    mTimer = startTimer(BLINK_TIME_MS, true);
    
    return true;
}

/** @see whiteboard::ILaunchableModule::startModule */
void BlinkyClient::stopModule() 
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer == whiteboard::ID_INVALID_TIMER;
}


void BlinkyClient::onTimer(whiteboard::TimerId timerId)
{
    if (timerId != mTimer) {
        return;
    }
    
    // increment counter and switch led on/off based on odd / evenness of it
    mCounter++;

    bool bLedOn = (mCounter & 0x1) != 0;
    // Make PUT request to switch on/off led. bool is converted automatically to whiteboard::Value. 
    asyncPut(WB_RES::LOCAL::DEVICE_COMPONENT_LED::ID, AsyncRequestOptions::Empty, bLedOn); 
}


