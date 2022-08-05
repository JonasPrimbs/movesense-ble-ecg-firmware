#include "movesense.h"

#include "WakeClient.h"
#include "common/core/debug.h"

#include "comm_ble/resources.h"
#include "component_led/resources.h"

#include "ui_ind/resources.h"
#include "component_max3000x/resources.h"
#include "system_mode/resources.h"


const char* const WakeClient::LAUNCHABLE_NAME = "Wake";

WakeClient::WakeClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mBlinkTimer(wb::ID_INVALID_TIMER),
    mShutdownTimer(wb::ID_INVALID_TIMER),
    mPowerOffTimer(wb::ID_INVALID_TIMER)
{
}

WakeClient::~WakeClient()
{
}

bool WakeClient::initModule()
{
    DEBUGLOG("WakeClient::initModule");

    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void WakeClient::deinitModule()
{
    DEBUGLOG("WakeClient::deinitModule");

    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool WakeClient::startModule()
{
    DEBUGLOG("WakeClient::startModule");

    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Start repeating blink timer.
    this->mBlinkTimer = startTimer(BLINK_PERIOD_MS, true);

    // Start the shutdown timer.
    this->startShutdownTimer();

    // Subscribe to changes of BLE peers list.
    this->asyncSubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    return true;
}

void WakeClient::stopModule()
{
    DEBUGLOG("WakeClient::stopModule");

    // Unsubscribe from changes of BLE peers list.
    this->asyncUnsubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    // Stop the shutdown timer.
    this->stopShutdownTimer();

    // Stop repeating blink timer and reset timer ID.
    stopTimer(this->mBlinkTimer);
    this->mBlinkTimer = wb::ID_INVALID_TIMER;

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void WakeClient::onNotify(wb::ResourceId resourceId,
    const wb::Value& value,
    const wb::ParameterList& rParameters)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_PEERS::LID:
        {
            uint8_t state = value.convertTo<const WB_RES::PeerChange&>().state;

            DEBUGLOG("COMM_BLE_PEERS: Peer state: %d", state);
            
            switch (state)
            {
                case WB_RES::PeerStateValues::CONNECTED:
                    this->stopShutdownTimer();
                    break;
                case WB_RES::PeerStateValues::DISCONNECTED:
                    this->startShutdownTimer();
                    break;
            }
            break;
        }
    }
}

void WakeClient::startShutdownTimer() {
    DEBUGLOG("Starting shutdown timer");

    // Stop timer if already active.
    if (this->mShutdownTimer != wb::ID_INVALID_TIMER) {
        stopTimer(this->mShutdownTimer);
    }

    // Start timer.
    this->mShutdownTimer = startTimer(SHUTDOWN_TIME_MS, false);
}

void WakeClient::stopShutdownTimer() {
    DEBUGLOG("Stopping shutdown timer");

    // Cancel if timer is already stopped.
    if (this->mShutdownTimer == wb::ID_INVALID_TIMER) {
        return;
    }

    // Stop timer and reset ID.
    stopTimer(this->mShutdownTimer);
    this->mShutdownTimer = wb::ID_INVALID_TIMER;
}

void WakeClient::startPowerOffTimer() {
    DEBUGLOG("Starting power off timer");

    // Stop timer if already active.
    if (this->mPowerOffTimer != wb::ID_INVALID_TIMER) {
        stopTimer(this->mPowerOffTimer);
    }

    // Start timer.
    this->mPowerOffTimer = startTimer(POWEROFF_TIME_MS, false);
}

void WakeClient::stopPowerOffTimer() {
    DEBUGLOG("Stopping power off timer");

    // Cancel if timer is already stopped.
    if (this->mPowerOffTimer == wb::ID_INVALID_TIMER) {
        return;
    }

    // Stop timer and reset ID.
    stopTimer(this->mPowerOffTimer);
    this->mPowerOffTimer = wb::ID_INVALID_TIMER;
}

void WakeClient::onTimer(wb::TimerId timerId)
{
    if (timerId == this->mBlinkTimer) {
        // Blink timer.

        // Let LED blink.
        const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
        this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type);
    } else if (timerId == this->mShutdownTimer) {
        // Shutdown timer.

        // Shut down the device.
        // const AsyncRequestOptions reqOptsForceAsync(NULL, 0, true);
        this->asyncPut(
            WB_RES::LOCAL::COMPONENT_MAX3000X_WAKEUP(),
            AsyncRequestOptions::Empty,
            // reqOptsForceAsync,
            (uint8_t)1u
        );
        this->asyncPut(
            WB_RES::LOCAL::COMPONENT_LED(),
            AsyncRequestOptions::Empty,
            true
        );
        this->stopShutdownTimer();
        this->startPowerOffTimer();

        // this->asyncPut(WB_RES::LOCAL::SYSTEM_MODE(), reqOptsForceAsync, WB_RES::SystemMode::FULLPOWEROFF);
    } else if (timerId == this->mPowerOffTimer) {
        this->stopPowerOffTimer();
        this->asyncPut(
            WB_RES::LOCAL::SYSTEM_MODE(),
            AsyncRequestOptions::Empty,
            WB_RES::SystemMode::FULLPOWEROFF
        );
    }
}
