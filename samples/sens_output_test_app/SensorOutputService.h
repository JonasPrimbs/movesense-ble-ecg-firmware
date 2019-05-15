#pragma once

#include "app-resources/resources.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

class SensorOutputService FINAL : private wb::ResourceClient,
                                  private wb::ResourceProvider,
                                  public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    SensorOutputService();
    ~SensorOutputService();

protected:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceProvider::onSubscribe */
    virtual void onSubscribe(const wb::Request& request,
                             const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onUnsubscribe */
    virtual void onUnsubscribe(const wb::Request& request,
                               const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onGetRequest */
    void onGetRequest(const wb::Request& request,
                      const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onPutRequest */
    void onPutRequest(const wb::Request& request,
                      const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceClient::onUnsubscribeResult */
    void onUnsubscribeResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode,
                             const wb::Value& rResultData);

    /** @see whiteboard::ResourceClient::onSubscribeResult */
    void onSubscribeResult(wb::RequestId requestId,
                           wb::ResourceId resourceId,
                           wb::Result resultCode,
                           const wb::Value& rResultData);

    /** @see whiteboard::ResourceProvider::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

    /** @see whiteboard::ResourceClient::onNotify */
    void onNotify(wb::ResourceId resourceId,
                  const wb::Value& value,
                  const wb::ParameterList& parameters);

    /**
    *	Checking the difference in the timestamp to search for the gaps
    *
    *	@param currentTimestamp current timestamp which needs to be checked
    */
    void timeDiffCheck(const uint32_t currentTimestamp);

    /**
    *	Counts the probes and puts them in the private array
    *
    *	@param arrayData received array of probes after the sensor notify
    */
    void probesCheck(const wb::Array<wb::FloatVector3D>& arrayData);

    /**
    *	Parse the input string and checks which sensor is selected by user
    *
    *	@param sensorPathSpecifiedByUser path to sensor which user gave in the PUT command
    *   @return Boolean status of path correctness
    */
    bool getSensorRequestedByUser(const char* sensorPathSpecifedByUser);

    /**
    *	Checks if specified sample rate is correct
    *
    *	@param sampleRateSpecifiedByUser path to sensor which user gave in the PUT command
    *   @return Boolean status of sample rate correctness correctness
    */
    bool checkSampleRateRequestedByUser(int32_t sampleRateSpecifiedByUser);

    /**
    *	Resets the current measurements
    */
    void resetCurrentMeasurement(void);

private:
    const uint32_t timerIntervalInMs = 1000;

    // Required TestResult variables - see SensorOutputSample.yaml
    uint32_t mTimestampGaps;
    uint32_t mBadSensorSamples;
    uint32_t mGoodSensorSamples;
    WB_RES::TestConfig mTestConfig;

    // ID of sensor subscribed by the user
    wb::ResourceId subscribedSensor;

    // Timestamp differences needed for gaps searching
    uint32_t lastTimestamp;
    uint32_t lastDiff;

    wb::TimerId mTestPeriodTimer; // Private timer for the samples gathering
};
