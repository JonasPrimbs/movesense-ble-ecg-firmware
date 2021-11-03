#pragma once

#include <whiteboard/ResourceProvider.h>
#ifndef APP_BOOTLOADER
#include <whiteboard/ResourceClient.h>
#endif
#include <whiteboard/DpcFunctor.h>
#include <whiteboard/LaunchableModule.h>
#include "common/time/TimeZone.hpp"
#include "common/compiler/genType.h"

namespace nea
{

/** Notifies subscribed listeners about current system time once a second. */
class TimeProvider : public whiteboard::ResourceProvider, public whiteboard::LaunchableModule
#if !defined(APP_BOOTLOADER) && !defined(APP_SS2_APPLICATION)
    , public whiteboard::ResourceClient
#endif
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;

    /** Constructor */
    TimeProvider();

    /** Destructor */
    virtual ~TimeProvider();

    bool initModule() OVERRIDE;
    void deinitModule() OVERRIDE;
    bool startModule() OVERRIDE;
    void stopModule() OVERRIDE;

    /** Get the current time in seconds. TODO document format. */
    int64 getSeconds() const { return mSeconds; }

    /** Second tick handler function. Executed in interrupt service routine.
     * Stores the new time to be available in \ref getSeconds call.
     * Queues new DPC in meas context to notify clients about changed time.
     * @param pUserData TimeProvider instance.
     * @param seconds   Current unix time */
    static void secondTickHandler(void* pUserData, const uint32_t seconds);

private:
    friend class TimeProviderTest;

    void onGetRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;

    /// You can PUT new time with WB_RES_DEV_TIME (int64)
    void onPutRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;

    void updateTimeResource();

#if !defined(APP_BOOTLOADER) && !defined(APP_SS2_APPLICATION)
    /** @see whiteboard::ResourceClient::onNotify */
    void onNotify(whiteboard::ResourceId resourceId,
                  const whiteboard::Value& value,
                  const whiteboard::ParameterList& parameters) OVERRIDE;

    // it calculates how much seconds to midnight and start the timer accordingly
    void checkHourAndMidnight(int64 currentTimestamp);

    // it checks if the date has changed due to new time set by user or gps
    void calculateDateChanged(uint32 oldts, uint32 newts);

#endif

private:
    /** Last time value from ChronoTickDelegate */
    // TODO: Volatile int64 is not atomic. Should probably instead use uint32_t
    //       since that is enough past year 2100
    volatile int64 mSeconds;
#if !defined(APP_BOOTLOADER) && !defined(APP_SS2_APPLICATION)
    nea::TimeZone::Id mLocalTime;
    int mTimeZoneOffset;
    int64 mLastHourTimestamp;
#endif
    /** DPC that is used to do client notification */
    whiteboard::DpcFunctor mUpdateDpc;
};

} // namespace nea
