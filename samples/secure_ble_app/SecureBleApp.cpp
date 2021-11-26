#include "movesense.h"

#include "SecureBleApp.hpp"
#include "common/core/debug.h"

#include "component_led/resources.h"
#include "ui_ind/resources.h"
#include "component_max3000x/resources.h"
#include "system_mode/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "wb-resources/resources.h"

#include <float.h>
#include <math.h>


const char* const SecureBleApp::LAUNCHABLE_NAME = "SecureBleApp";

// Time between wake-up and going to power-off mode
#define AVAILABILITY_TIME 60000

// Time between turn on AFE wake circuit to power off
// (must be LED_BLINKING_PERIOD multiple)
#define WAKE_PREPARATION_TIME 5000

// LED blinking period in adertsing mode
#define LED_BLINKING_PERIOD 5000


SecureBleApp * SecureBleApp::spInstance = NULL;


SecureBleApp::SecureBleApp()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      mTimer(whiteboard::ID_INVALID_TIMER),
      mWbConnected(false),
      mCounter(0)
{
    spInstance = this;
}


SecureBleApp::~SecureBleApp()
{
}


bool SecureBleApp::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}


void SecureBleApp::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}


/** @see whiteboard::ILaunchableModule::startModule */
bool SecureBleApp::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    startShutdownTimer();

    // Subscribe to Whiteboard routing table changes
    asyncSubscribe(WB_RES::LOCAL::NET());

    return true;
}


/** @see whiteboard::ILaunchableModule::startModule */
void SecureBleApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = whiteboard::ID_INVALID_TIMER;
}


void SecureBleApp::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                                   const whiteboard::ParameterList& parameters)
{
    // WB routing table notification
    if (whiteboard::ResourceId::Value(resourceId) == WB_RES::LOCAL::NET::ID)
    {
        // Get whiteborad routing table notification
        uint8_t data = WB_RES::LOCAL::NET::EVENT::ParameterListRef(parameters)
            .getNotificationType();

        // if there is whiteboard connection, stop timer
        if (data == WB_RES::RoutingTableNotificationTypeValues::ROUTE_NOTIFICATION_NEW)
        {
            stopShutdownTimer();
            mWbConnected = true;
            return;
        }

        // if whiteboard connection lost, prepare to shutdown
        if (data == WB_RES::RoutingTableNotificationTypeValues::ROUTE_NOTIFICATION_LOST)
        {
            mWbConnected = false;
            startShutdownTimer();
        }

        return;
    }
}


void SecureBleApp::startShutdownTimer()
{
    DEBUGLOG("Start shutdown timer");

    if (mTimer != whiteboard::ID_INVALID_TIMER)
        stopTimer(mTimer);

    // Start timer
    mTimer = startTimer(LED_BLINKING_PERIOD, true);

    // Reset timeout counter
    mCounter = 0;
}


void SecureBleApp::stopShutdownTimer()
{
    DEBUGLOG("Stop shutdown timer");

    if (mTimer == whiteboard::ID_INVALID_TIMER)
        return;

    stopTimer(mTimer);
    mTimer = whiteboard::ID_INVALID_TIMER;
}


void SecureBleApp::onTimer(whiteboard::TimerId timerId)
{
//    mCounter = mCounter + LED_BLINKING_PERIOD;

    if (mCounter < AVAILABILITY_TIME)
    {
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(),
            AsyncRequestOptions::Empty, 
            WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION);
        return;
    }

    if (mCounter < AVAILABILITY_TIME + WAKE_PREPARATION_TIME)
    {
        // Prepare AFE to wake-up mode
        asyncPut(WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP(),
            AsyncRequestOptions(NULL, 0, true), (uint8_t) 1);
        return;
    }

    // Make PUT request to switch LED on
    asyncPut(WB_RES::LOCAL::COMPONENT_LED(), AsyncRequestOptions::Empty, true);

    // Make PUT request to enter power off mode
    asyncPut(WB_RES::LOCAL::SYSTEM_MODE(),
        AsyncRequestOptions(NULL, 0, true), // Force async
        WB_RES::SystemModeValues::FULLPOWEROFF);
}
