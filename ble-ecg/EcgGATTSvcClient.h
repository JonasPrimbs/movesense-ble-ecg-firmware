#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>


// ECG Value definitions:

/** Type definition of ECG Value. */
typedef int16_t ecg_val;
/** Maximum ECG Value. */
const ecg_val ECG_MAX_VALUE = 32767; // 0x7FFF = 2^15 - 1;
/** Minimum ECG Value. */
const ecg_val ECG_MIN_VALUE = -32767; // 0x8001 = -(2^15 - 1);
/** Invalid ECG Value. */
const ecg_val ECG_INVALID_VALUE = -32768; // 0x8000 = -2^15;


// ECG GATT Services and Characteristics UUIDs:

/** 16-bit UUID for ECG Service. */
const uint16_t ecgSvcUUID16 = 0x1857;

/** 16-bit UUID for ECG Voltage Characteristic. */
const uint16_t ecgVoltageCharUUID16 = 0x2BDD;
/** 16-bit UUID for sample interval Characteristic. */
const uint16_t measurementIntervalCharUUID16 = 0x2A21;
/** 16-bit UUID for object size Characteristic. */
const uint16_t objectSizeCharUUID16 = 0x2BDE;

/** Number of message buffers. Must be > 1. */
const size_t numberOfMessageBuffers = 2;


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


    // ECG Buffer:

    /** Index of current ECG buffer. */
    size_t ecgBufferIndex = 0;
    /** Array of available ECG buffers to switch between them. */
    uint8_t* ecgCharacteristicsBuffers = nullptr;
    /** Number of samples in ECG buffer. */
    size_t bufferedEcgSamples;
    /**
     * @brief Gets the size of a single ECG buffer.
     * 
     * @return size_t Size of one ECG buffer.
     */
    size_t getSingleEcgBufferSize();
    /**
     * @brief Gets a pointer to the start of the i-th ECG buffer.
     * 
     * @param i Index of requested ECG buffer.
     * @return uint8_t* Pointer to start of requested ECG buffer.
     */
    uint8_t* getEcgBuffer(size_t i);
    /**
     * @brief Gets a pointer to the current ECG buffer.
     * 
     * @return uint8_t* Pointer to the start of the current ECG buffer.
     */
    uint8_t* getCurrentEcgBuffer();
    /**
     * @brief Switches the current active message buffer to the next active message buffer.
     */
    void switchMessageBuffer();
    /**
     * @brief Clears the ECG buffer at index i.
     * 
     * @param i Index of ECG buffer to clear.
     * @return true Clearing ECG buffer was successful.
     * @return false Clearing ECG buffer was not successful due to not initialized buffer.
     */
    bool clearEcgBuffer(size_t i);
    /**
     * @brief Clears the current ECG buffer.
     * 
     * @return true Clearing ECG buffer was successful.
     * @return false Clearing ECG buffer was not successful due to not initialized buffer.
     */
    bool clearCurrentEcgBuffer();


    // ECG Samples:

    /**
     * @brief Converts the sensor's ECG value to a BLE ECG value.
     * 
     * @param ecgValue Sensor's ECG value to convert.
     * @return ecg_val Converted BLE ECG value.
     */
    ecg_val convertEcgSample(int32 ecgValue);

    /**
     * @brief Adds an ECG sample to the current ECG buffer.
     * 
     * @param sample ECG sample to add.
     * @return true Adding was successful.
     * @return false Adding was not successful since ECG buffer is not initialized.
     */
    bool addEcgSample(ecg_val sample);

    /**
     * @brief Sets the relative timestamp of the last sample in ECG buffer.
     * 
     * @param timestamp Timestamp to set.
     * @return true Setting was successful.
     * @return false Setting was not successful since ECG buffer is not initialized.
     */
    bool setTimestamp(uint32_t timestamp);

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
