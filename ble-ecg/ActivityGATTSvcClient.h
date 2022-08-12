#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

#include "SeriesBuffer.h"
#include "config.h"


// Activity GATT Service:

class ActivityGATTSvcClient FINAL :
    private wb::ResourceClient,
    public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor for Activity GATT Service. */
    ActivityGATTSvcClient();
    /** Destructor for Activity GATT Service. */
    ~ActivityGATTSvcClient();

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

    /** The handle of the Movement GATT Characteristics. */
    int32_t mMovCharHandle;
    /** The resource ID of the Movement GATT Characteristics. */
    wb::ResourceId mMovCharResource;

    /** The handle of the Movement Measurement Interval GATT Characteristics. */
    int32_t mMovMeasurementIntervalCharHandle;
    /** The resource ID of the Movement Measurement Interval GATT Characteristics. */
    wb::ResourceId mMovMeasurementIntervalCharResource;


    // Buffers:

    /** Buffer to hold ECG samples. */
    SeriesBuffer<ecg_t>* ecgBuffer;
    /** Buffer to hold Movement samples. */
    SeriesBuffer<mov_t>* movBuffer;

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


    // Movement Samples:

    /** Movement samples which are currently built. */
    mov_t* currentMovementSamples;
    /** Number of stored Acceleration samples. */
    size_t movAccCount;
    /** Number of stored Gyroscope samples. */ 
    size_t movGyrCount;
    /** Number of stored Magnetic Field samples. */
    size_t movMagCount;

    /**
     * @brief Copys the cached samples from `currentMovementSamples` to the movement buffer, adds the timestamp, and sends data.
     * 
     * @param t Timestamp of last sample.
     * @param n Number of samples.
     * @param i Index of last sample.
     */
    void finishMovementSeries(timestamp_t t, size_t n, size_t i);

    /**
     * @brief Converts the sensor's Acceleration vector to a BLE Acceleration vector.
     * 
     * @param accVector Sensor's Acceleration vector to convert.
     * @return acc_vec_t Converted BLE Acceleration vector.
     */
    acc_vec_t convertAccSample(whiteboard::FloatVector3D accVector);
    /**
     * @brief Converts the sensor's Gyroscope vector to a BLE Gyroscope vector.
     * 
     * @param gyrVector Sensor's Gyroscope vector to convert.
     * @return gyr_vec_t Converted BLE Gyroscope vector.
     */
    gyr_vec_t convertGyrSample(whiteboard::FloatVector3D gyrVector);
    /**
     * @brief Converts the sensor's Magnetic Field vector to a BLE Magnetic Field vector.
     * 
     * @param magVector Sensor's Magnetic Field vector to convert.
     * @return mag_vec_t Converted BLE Magnetic Field vector.
     */
    mag_vec_t convertMagSample(whiteboard::FloatVector3D magVector);

    /**
     * @brief Sends the Movement buffer via BLE.
     * 
     * @return true Sending was successful.
     * @return false Sending was not successful since Movement buffer is not initialized.
     */
    bool sendMovBuffer();


    // ECG Measurement Interval:

    /**
     * @brief Interval between two ECG measurements in milliseconds.
     * Must be one of the following: 
     *  - 2 ms =  500 Hz
     *  - 4 ms =  250 Hz
     *  - 5 ms =  200 Hz
     *  - 8 ms =  125 Hz
     */
    uint16_t ecgMeasurementInterval;
    /**
     * @brief Gets the desired ECG sampling rate.
     *  -  500 Hz = 2 ms
     *  -  250 Hz = 4 ms
     *  -  200 Hz = 5 ms
     *  -  125 Hz = 8 ms
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


    // Movement Measurement Interval:

    /**
     * @brief Interval between two Movement measurements in milliseconds.
     * Must be one of the following:
     *  -  5 ms = 208 Hz
     *  - 10 ms = 104 Hz
     *  - 20 ms =  52 Hz
     *  - 40 ms =  26 Hz
     */
    uint16_t movMeasurementInterval;
    /**
     * @brief Gets the desired Movement sampling rate.
     *  - 208 Hz =  5 ms
     *  - 104 Hz = 10 ms
     *  -  52 Hz = 20 ms
     *  -  26 Hz = 40 ms
     * 
     * @return uint32_t Desired Movement sampling rate.
     */
    uint32_t getMovSampleRate();
    /**
     * @brief Set the interval between two Movement measurements in milliseconds.
     * 
     * @param value Movement measurement interval in milliseconds.
     */
    void setMovMeasurementInterval(uint16_t value);

    /**
     * @brief Subscribes to Acceleration samples.
     */
    void subscribeToAccSamples();
    /**
     * @brief Unsubscribes from Acceleration samples.
     */
    void unsubscribeFromAccSamples();

    /**
     * @brief Subscribes to Gyroscope samples.
     */
    void subscribeToGyrSamples();
    /**
     * @brief Unsubscribes from Gyroscope samples.
     */
    void unsubscribeFromGyrSamples();

    /**
     * @brief Subscribes to Magnetic Field samples.
     */
    void subscribeToMagSamples();
    /**
     * @brief Unsubscribes from Magnetic Field samples.
     */
    void unsubscribeFromMagSamples();
};
