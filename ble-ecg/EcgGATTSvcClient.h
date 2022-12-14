#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

#include "SeriesBuffer.h"
#include "config.h"


// ECG GATT Service:

class EcgGATTSvcClient FINAL :
    private wb::ResourceClient,
    public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor for ECG GATT Service. */
    EcgGATTSvcClient();
    /** Destructor for ECG GATT Service. */
    ~EcgGATTSvcClient();

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
                             const wb::Value& rResultData);
    /** @see whiteboard::ResourceClient::onPostResult */
    virtual void onPostResult(wb::RequestId requestId,
                              wb::ResourceId resourceId,
                              wb::Result resultCode,
                              const wb::Value& rResultData) OVERRIDE;
    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId,
                          const wb::Value& rValue,
                          const wb::ParameterList& rParameters);

private:

    // GATT Service:

    /** The handle of the ECG GATT Service. */
    int32_t mEcgSvcHandle;

    /**
     * @brief Configures the GATT Service.
     */
    void configGattSvc();


    // GATT Characteristics:

    /** The handle of the ECG Voltage GATT Characteristics. */
    int32_t mEcgVoltageCharHandle;
    /** The resource ID of the ECG Voltage GATT Characteristics. */
    wb::ResourceId mEcgVoltageCharResource;

    /** The handle of the Measurement Interval GATT Characteristics. */
    int32_t mMeasurementIntervalCharHandle;
    /** The resource ID of the Measurement Interval GATT Characteristics. */
    wb::ResourceId mMeasurementIntervalCharResource;

    /** The handle of the Object Size GATT Characteristics. */
    int32_t mObjectSizeCharHandle;
    /** The resource ID of the Object Size GATT Characteristics. */
    wb::ResourceId mObjectSizeCharResource;


    // Buffers:

    /** Buffer to hold ECG samples. */
    SeriesBuffer<ecg_t>* ecgBuffer;


    // ECG Samples:

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
     * @return false Sending was not successful since ECG buffer is not initialized.
     */
    bool sendEcgBuffer();


    // ECG Measurement Interval:

    /**
     * @brief Interval between two ECG measurements in milliseconds.
     * Must be one of the following: 
     *  - 1 ms = 1000 Hz
     *  - 2 ms =  500 Hz
     *  - 4 ms =  250 Hz
     *  - 8 ms =  125 Hz
     */
    uint16_t measurementInterval;
    /**
     * @brief Gets the desired sampling rate.
     *  - 1000 Hz = 1 ms
     *  -  500 Hz = 2 ms
     *  -  250 Hz = 4 ms
     *  -  125 Hz = 8 ms
     * 
     * @return uint32_t Desired sampling rate.
     */
    uint32_t getSampleRate();
    /**
     * @brief Set the interval between two ECG measurements in milliseconds.
     * 
     * @param value ECG measurement interval in milliseconds.
     */
    void setMeasurementInterval(uint16_t value);

    /**
     * @brief Subscribes to ECG samples.
     */
    void subscribeToEcgSamples();
    /**
     * @brief Unsubscribes from ECG samples.
     */
    void unsubscribeFromEcgSamples();

    /** Number of objects per sample message. */
    uint16_t objectSize;
    /**
     * @brief Sets the number of objects per sample message.
     * 
     * @param value Number of objects per sample message.
     */
    void setObjectSize(uint16_t value);
};
