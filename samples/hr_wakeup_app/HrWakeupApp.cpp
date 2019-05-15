#include "movesense.h"

#include "HrWakeupApp.h"
#include "common/core/debug.h"

#include "component_led/resources.h"
#include "component_max3000x/resources.h"
#include "ui_ind/resources.h"
#include "system_mode/resources.h"

const char* const HrWakeupApp::LAUNCHABLE_NAME = "HrWakeupApp";

// Time between wake-up and going to power-off mode
#define AVAILABILITY_TIME 60000

// Time between turn on AFE wake circuit to power off
// (must be LED_BLINKING_PERIOD multiple)
#define WAKE_PREPARATION_TIME 5000

// LED blinking period in adertsing mode
#define LED_BLINKING_PERIOD 5000

HrWakeupApp::HrWakeupApp():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mTimer(wb::ID_INVALID_TIMER),
    counter(0)
{
}

HrWakeupApp::~HrWakeupApp()
{
}

bool HrWakeupApp::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void HrWakeupApp::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool HrWakeupApp::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    setShutdownTimer();

    // Subscribe to Whiteboard routing table changes
    asyncSubscribe(WB_RES::LOCAL::NET());

    return true;
}

void HrWakeupApp::stopModule()
{
    // Stop LED timer
    stopTimer(mTimer);
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void HrWakeupApp::onNotify(wb::ResourceId resourceId,
                           const wb::Value& value,
                           const wb::ParameterList& parameters)
{
    // Get whiteborad routing table notification
    uint8_t data = WB_RES::LOCAL::NET::EVENT::ParameterListRef(parameters).getNotificationType();

    // if there is whiteboard connection, stop timer
    if (data == WB_RES::RoutingTableNotificationTypeValues::ROUTE_NOTIFICATION_NEW)
    {
        stopTimer(mTimer);
        mTimer = wb::ID_INVALID_TIMER;
        return;
    }

    // if whiteboard connection lost, prepare to shutdown
    if (data == WB_RES::RoutingTableNotificationTypeValues::ROUTE_NOTIFICATION_LOST)
    {
        setShutdownTimer();
    }
}

void HrWakeupApp::setShutdownTimer()
{
    // Start timer
    mTimer = startTimer(LED_BLINKING_PERIOD, true);

    // Reset timeout counter
    counter = 0;
}

void HrWakeupApp::onTimer(wb::TimerId timerId)
{
    counter = counter + LED_BLINKING_PERIOD;

    if (counter < AVAILABILITY_TIME)
    {
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty,
                 WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION);
        return;
    }

    if (counter == AVAILABILITY_TIME)
    {
        // Prepare AFE to wake-up mode
        asyncPut(WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP(),
                 AsyncRequestOptions(NULL, 0, true), (uint8_t)1);

        // Make PUT request to switch LED on
        asyncPut(WB_RES::LOCAL::COMPONENT_LED(), AsyncRequestOptions::Empty, true);

        // Make PUT request to eneter power off mode
        asyncPut(WB_RES::LOCAL::SYSTEM_MODE(), AsyncRequestOptions(NULL, 0, true), // true = Force async
                 WB_RES::SystemModeValues::FULLPOWEROFF);
    }
}
