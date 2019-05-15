#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "wb-resources/resources.h"

#define BLINK_DELAY 1000
#define BLINK_TIME 1000
#define HR_TIMEOUT 5000

class ConnectionScanner FINAL : private wb::ResourceClient, public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    ConnectionScanner();
    ~ConnectionScanner();

    // Some States are not used or exist only for better understanding.
    // FLOW:
    //              |          PRE         |
    //               ----------------------
    //                         |  asyncSubscribe
    //                         |  SYSTEM_STATES_STATEID
    //              -----------------------
    //              | DEVICE_DISCONNECTED  |
    //              -----------------------
    //                         | onNotify
    //                         | SYSTEM_STATES_STATEID == 1 (Something connected)
    //              -----------------------
    //              |   DEVICE_CONNECTED   |
    //              | -> GEAR_DETECTING    |
    //              -----------------------
    //                         | asyncGet WB_RES::LOCAL::MISC_GEAR_ID()
    //                         |          is blocking the system state
    //                         |          short time (SYSTEM_STATES_STATEID == 2)
    //                         .
    //                         . onGetResult                  -----------------------------
    //                         | MISC_GEAR_ID                 = GOTO: DEVICE_DISCONNECTED =
    //                 0      /\    ELSE                      -----------------------------
    //              ---------    ----------                                  | onNotify
    //             |                       |                                || SYSTEM_STATES_STATEID == 0
    //     ------------------       ----------------                        ||
    //     | GEAR_NOT_FOUND |      |  GEAR_DETECTED | ---------- ( GEAR ID CONNECTED! )
    //     | -> HR_DETECTING|      ----------------                         ||
    //     -----------------                                                ||
    //             | * asyncSubscribe MEAS_HR                               ||
    //             |        is blocking the system state until              ||
    //             |        unsubscribe (SYSTEM_STATES_STATEID == 2)        ||
    //             | * start timeout timer                                  ||
    //             -------------------------                                ||
    //             |                       | onNotify - MEAS_HR             ||
    //             | TIMEOUT               | SAMPLES OK                     ||
    //             |                       | unsubscribe MEAS_HR            ||
    //     --------------------     -----------------                       ||
    //     | HR_NOT_FOUND     |    | HR_DETECTED    |----------- ( HR BETL CONNECTED! )
    //     | -> UNKNOWN_DEVICE|    ----------------                         ||
    //     -------------------                                              ||
    //            |                                                         ||
    //            ----------------------------------------------- | UNKNOWN DEVICE |

    enum DetectionState {
        DEVICE_DISCONNECTED = 0,
        DEVICE_CONNECTED = 1,
        GEAR_DETECTING = 2,
        GEAR_NOT_FOUND = 3,
        GEAR_DETECTED = 4,
        HR_DETECTING = 5,
        HR_NOT_FOUND = 6, 
        HR_DETECTED = 7,
        WAITING = 8,
        UNKNOWN_DEVICE = 9
    };

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceClient::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId,
                          const wb::Value& value,
                          const wb::ParameterList& parameters);

    /** @see whiteboard::ResourceClient::onGetResult */
    virtual void onGetResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode,
                             const wb::Value& result);

private:
    /**
     *  Init function
     */
    void startRunning(void);

    /**
     *  Init function
     */
    void stopRunning(void);

    /**
    *	Function used to simplify the flow of the states and keep it in one place.
    *
    *	@param newState
    */
    void updateDetectionState(DetectionState newState);

    /**
    *	Blinking function
    *
    *	@param times how many times LED will blink.
    */
    inline void startBlinking(uint8_t times)
    {
        mBlinkCount = times;
        mLedBlinkTimer = startTimer(BLINK_DELAY, false);
    }

    wb::ResourceId mSystemStateResourceId;

    wb::TimerId mLedBlinkTimer;
    wb::TimerId mHRCheckOffTimer;
    uint8_t mSamplesCounter;
    uint8_t mBlinkCount;
    DetectionState mDetectionState;
};
