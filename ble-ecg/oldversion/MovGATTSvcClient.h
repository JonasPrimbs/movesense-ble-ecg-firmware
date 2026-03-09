#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

#include "SeriesBuffer.h"
#include "config.h"


// Movement GATT Service:

class MovGATTSvcClient FINAL :
    private wb::ResourceClient,
    public wb::LaunchableModule
{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;

    /** Constructor for Movement GATT Service. */
    MovGATTSvcClient();
    /** Destructor for Movement GATT Service. */
    ~MovGATTSvcClient();

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

    /** The handle of the Movement GATT Service. */
    int32_t mMovSvcHandle;

    /**
     * @brief Configures the GATT Service.
     */
    void configGattSvc();


    // GATT Characteristics:

    /** The handle of the Acceleration GATT Characteristics. */
    int32_t mMovAccCharHandle;
    /** The resource ID of the Acceleration GATT Characteristics. */
    wb::ResourceId mMovAccCharResource;

    /** The handle of the Gyroscope GATT Characteristics. */
    int32_t mMovGyrCharHandle;
    /** The resource ID of the Gyroscope GATT Characteristics. */
    wb::ResourceId mMovGyrCharResource;

    /** The handle of the Magnetic Field GATT Characteristics. */
    int32_t mMovMagCharHandle;
    /** The resource ID of the Magnetic Field GATT Characteristics. */
    wb::ResourceId mMovMagCharResource;

    /** The handle of the Measurement Interval GATT Characteristics. */
    int32_t mMeasurementIntervalCharHandle;
    /** The resource ID of the Measurement Interval GATT Characteristics. */
    wb::ResourceId mMeasurementIntervalCharResource;

    /** The handle of the Object Size GATT Characteristics. */
    int32_t mObjectSizeCharHandle;
    /** The resource ID of the Object Size GATT Characteristics. */
    wb::ResourceId mObjectSizeCharResource;


    // Buffers:

    /** Buffer to hold Acceleration samples. */
    SeriesBuffer<acc_vec_t>* accBuffer;
    /** Buffer to hold Gyroscope samples. */
    SeriesBuffer<gyr_vec_t>* gyrBuffer;
    /** Buffer to hold Magnetic Field samples. */
    SeriesBuffer<mag_vec_t>* magBuffer;


    // Acceleration Samples:

    /**
     * @brief Converts the sensor's Acceleration vector to a BLE Acceleration vector.
     * 
     * @param accVector Sensor's Acceleration vector to convert.
     * @return acc_vec_t Converted BLE Acceleration vector.
     */
    acc_vec_t convertAccSample(whiteboard::FloatVector3D accVector);

    /**
     * @brief Sends the Acceleration buffer via BLE.
     * 
     * @return true Sending was successful.
     * @return false Sending was not successful since Acceleration buffer is not initialized.
     */
    bool sendAccBuffer();


    // Gyroscope Samples:

    /**
     * @brief Converts the sensor's Gyroscope vector to a BLE Gyroscope vector.
     * 
     * @param gyrVector Sensor's Gyroscope vector to convert.
     * @return gyr_vec_t Converted BLE Gyroscope vector.
     */
    gyr_vec_t convertGyrSample(whiteboard::FloatVector3D gyrVector);

    /**
     * @brief Sends the Gyroscope buffer via BLE.
     * 
     * @return true Sending was successful.
     * @return false Sending was not successful since Gyroscope buffer is not initialized.
     */
    bool sendGyrBuffer();


    // Magnetic Field Samples:

    /**
     * @brief Converts the sensor's Magnetic Field vector to a BLE Magnetic Field vector.
     * 
     * @param magVector Sensor's Magnetic Field vector to convert.
     * @return mag_vec_t Converted BLE Magnetic Field vector.
     */
    mag_vec_t convertMagSample(whiteboard::FloatVector3D magVector);

    /**
     * @brief Sends the Magnetic Field buffer via BLE.
     * 
     * @return true Sending was successful.
     * @return false Sending was not successful since Magnetic Field buffer is not initialized.
     */
    bool sendMagBuffer();


    // Movement Measurement Interval:

    /**
     * @brief Interval between two Movement measurements in milliseconds.
     * Must be one of the following: 
     *  -  5 ms = 208 Hz
     *  - 10 ms = 104 Hz
     *  - 20 ms =  52 Hz
     *  - 40 ms =  26 Hz
     */
    uint16_t measurementInterval;
    /**
     * @brief Gets the desired sampling rate.
     *  - 208 Hz =   5 ms
     *  - 104 Hz =  10 ms
     *  -  52 Hz =  20 ms
     *  -  26 Hz =  40 ms
     * 
     * @return uint32_t Desired sampling rate.
     */
    uint32_t getSampleRate();
    /**
     * @brief Converts a measurement interval to a sampling rate.
     *  - 208 Hz =   5 ms
     *  - 104 Hz =  10 ms
     *  -  52 Hz =  20 ms
     *  -  26 Hz =  40 ms
     * 
     * @param interval measurement interval.
     * @return uint32_t Sampling rate.
     */
    uint32_t toSampleRate(uint16_t interval);
    /**
     * @brief Set the interval between two Movement measurements in milliseconds.
     * 
     * @param value Movement measurement interval in milliseconds.
     */
    void setMeasurementInterval(uint16_t value);

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

    /** Number of objects per sample message. */
    uint16_t objectSize;
    /**
     * @brief Sets the number of objects per sample message.
     * 
     * @param value Number of objects per sample message.
     */
    void setObjectSize(uint16_t value);
};
