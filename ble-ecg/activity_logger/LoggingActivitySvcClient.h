#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

#include "EcgConstants.h"
#include "GattConfig.h"
#include "SeriesBuffer.h"

// Activity GATT Service:

class LoggingActivitySvcClient FINAL : private wb::ResourceClient, public wb::LaunchableModule
{
  public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor for Activity GATT Service. */
    LoggingActivitySvcClient();
    /** Destructor for Activity GATT Service. */
    ~LoggingActivitySvcClient();

  private:
    // Module:

    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    // Subscriptions:

    /** @see whiteboard::ResourceClient::onGetResult */
    virtual void onGetResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode,
                             const wb::Value& rResultData) OVERRIDE;
    /** @see whiteboard::ResourceClient::onPostResult */
    virtual void onPostResult(wb::RequestId requestId,
                              wb::ResourceId resourceId,
                              wb::Result resultCode,
                              const wb::Value& rResultData) OVERRIDE;
    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId,
                          const wb::Value& rValue,
                          const wb::ParameterList& rParameters) OVERRIDE;

  private:
    // GATT Service:

    /** The handle of the Activity GATT Service. */
    int32_t mActivitySvcHandle;

    /**
     * @brief Configures the GATT Service.
     */
    void configGattSvc();

    // GATT Characteristics:

    /** The handle of the ECG Voltage GATT Characteristics. */
    int32_t mEcgVoltageCharHandle;
    /** The resource ID of the ECG Voltage GATT Characteristics. */
    wb::ResourceId mEcgVoltageCharResource;

    /** The handle of the ECG Measurement Interval GATT Characteristics. */
    int32_t mEcgMeasurementIntervalCharHandle;
    /** The resource ID of the ECG Measurement Interval GATT Characteristics. */
    wb::ResourceId mEcgMeasurementIntervalCharResource;

    // Buffers:

    /** Buffer to hold ECG samples. */
    SeriesBuffer<ecg_t>* ecgBuffer;

    // ECG Samples:

    uint16_t ecgSampleCounter;
    uint16_t ecgSampleSkipCount;

    /**
     * @brief Converts the sensor's ECG value to a BLE ECG value.
     *
     * @param ecgValue Sensor's ECG value to convert.
     * @return ecg_t Converted BLE ECG value.
     */
    ecg_t convertEcgSample(int32 ecgValue);

    /**
     * @brief Sends the ECG buffer via BLE.
     *
     * @return true Sending was successful.
     * @return false Sending was not successful since ECG buffer is not
     * initialized.
     */
    bool sendEcgBuffer();

    /**
     * @brief Interval between two ECG measurements in milliseconds.
     * Must be one of the following:
     *  -  2 ms =  500 Hz
     *  -  4 ms =  250 Hz
     *  -  5 ms =  200 Hz
     *  -  8 ms =  125 Hz
     *  - 10 ms =  100 Hz
     */
    uint16_t ecgMeasurementInterval;
    /**
     * @brief Gets the desired ECG sampling rate.
     *  -  500 Hz =  2 ms
     *  -  250 Hz =  4 ms
     *  -  200 Hz =  5 ms
     *  -  125 Hz =  8 ms
     *  -  100 Hz = 10 ms
     *
     * @return uint32_t Desired ECG sampling rate.
     */
    uint32_t getEcgSampleRate();
    /**
     * @brief Set the interval between two ECG measurements in milliseconds.
     *
     * @param value ECG measurement interval in milliseconds.
     */
    void setEcgMeasurementInterval(uint16_t value);

    /**
     * @brief Subscribes to ECG samples.
     */
    void subscribeToEcgSamples();
    /**
     * @brief Unsubscribes from ECG samples.
     */
    void unsubscribeFromEcgSamples();
};
