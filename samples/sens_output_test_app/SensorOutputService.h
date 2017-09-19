#pragma once

#include "app-resources/resources.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

class SensorOutputService FINAL : private whiteboard::ResourceProvider,
                                  private whiteboard::ResourceClient,
                                  public whiteboard::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    SensorOutputService();
    ~SensorOutputService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    /**
    *	Subscribe notification callback.
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    virtual void onSubscribe(const whiteboard::Request& request,
                             const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *	Unsubscribe notification callback.
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    virtual void onUnsubscribe(const whiteboard::Request& request,
                               const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *	Callback for asynchronous UNSUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    void onUnsubscribeResult(whiteboard::RequestId requestId,
                             whiteboard::ResourceId resourceId,
                             whiteboard::Result resultCode,
                             const whiteboard::Value& rResultData);

    /**
    *	Callback for asynchronous SUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    void onSubscribeResult(whiteboard::RequestId requestId,
                           whiteboard::ResourceId resourceId,
                           whiteboard::Result resultCode,
                           const whiteboard::Value& rResultData);

    /**
    *	GET request handler for the config
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    void onGetRequest(const whiteboard::Request& request,
                      const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *	PUT request handler for the config
    *
    *	@param request Request information
    *	@param parameters List of parameters
    */
    void onPutRequest(const whiteboard::Request& request,
                      const whiteboard::ParameterList& parameters) OVERRIDE;

    // Required TestResult variables - see SensorOutputSample.yaml
    uint32_t mTimestampGaps;
    uint32_t mBadSensorSamples;
    uint32_t mGoodSensorSamples;
    WB_RES::TestConfig mTestConfig;

    /**
    *	This callback is called when the sensor resource notifies us of new data.
    *
    *	@param resourceId Resource id associated with the update
    *	@param value Current value of the resource
    */
    void onNotify(whiteboard::ResourceId resourceId,
                  const whiteboard::Value& value,
                  const whiteboard::ParameterList& parameters);

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
    void probesCheck(const whiteboard::Array<whiteboard::FloatVector3D>& arrayData);

    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

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

    const uint32_t timerIntervalInMs = 1000;

    // ID of sensor subscribed by the user
    whiteboard::ResourceId subscribedSensor;

    // Timestamp differences needed for gaps searching
    uint32_t lastTimestamp;
    uint32_t lastDiff;

    whiteboard::TimerId mTestPeriodTimer; // Private timer for the samples gathering
};
