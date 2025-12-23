#pragma once

#include "movesense.h"

/**
 *  Movesense-Client for Processing all GAtt-related functions and
 *  controlling/ consuming Data of the MeasurementProvider.
 */
class OfflineStorageGattClient FINAL : private wb::ResourceClient,
                                       private wb::LaunchableModule
{
  public:
    static const char* const LAUNCHABLE_NAME;

    OfflineStorageGattClient();
    ~OfflineStorageGattClient();

  private:
    // From wb::LaunchableModule:
    bool initModule() OVERRIDE;
    void deinitModule() OVERRIDE;
    bool startModule() OVERRIDE;
    void stopModule() OVERRIDE;

    uint8_t* mSbemWaitBuffer;
    size_t mSbemWaitBufferIndex = 0;

    // From wb::ResourceClient:
    void onGetResult(wb::RequestId requestId,
                     wb::ResourceId resourceId,
                     wb::Result resultCode,
                     const wb::Value& rResultData) OVERRIDE;

    void onPostResult(wb::RequestId requestId,
                      wb::ResourceId resourceId,
                      wb::Result resultCode,
                      const wb::Value& rResultData) OVERRIDE;

    void onNotify(wb::ResourceId resourceId,
                  const wb::Value& rValue,
                  const wb::ParameterList& rParameters) OVERRIDE;

    void onTimer(wb::TimerId timerId) OVERRIDE;

    // GATT Service configuration methods:
    /**
     * @brief Configure the GATT Service during Module start.
     */
    void configGattSvc();

    /**
     * @brief Uninitialize Fields used by the GATT service during module stop.
     */
    void deconfigGattSvc();

    /**
     * @brief Initialize Subscriptions to incoming GATT-Events.
     */
    void initGattCharSubscriptions();

    /**
     * @brief Uninitialize Subscriptions from incomn GATT-Events.
     */
    void deinitGattCharSubscriptions();

    // Member variables for GATT handles and resources:
    int32_t mActivityServiceHandle;

    int32_t mCharAHandle;
    wb::ResourceId mCharAResource;

    int32_t mCharBHandle;
    wb::ResourceId mCharBResource;

    int32_t mCharCHandle;
    wb::ResourceId mCharCResource;

    int32_t mCharDHandle;
    wb::ResourceId mCharDResource;

    // Flags to keep track GATT subscribers.
    /*+ Subscription to the ECG-Measurement Characteristic **/
    bool mClientIsListeningToEcg;
    /*+ Subscription to the IMU-Measurement Characteristic **/
    bool mClientIsListeningToMov;
    /*+ Subscription to the Recorded-Data Characteristic **/
    bool mClientIsListeningToRecorded;

    // Measurement Interval settings.
    /** The current ECG-Measurement Interval setting **/
    uint8_t mEcgMeasurementInterval;
    /** The current IMU-Measurement Interval setting **/
    uint8_t mImuMeasurementInterval;

    /**
     * @brief Execute the necessary configurations, when the BLE-Client writes to the
     * configuration field characteristic.
     * @param configFields the newly received configuration field characteristic value
     */
    void parseConfigurationField(const uint8_t* configFields);

    // For Data Recording.

    // Record only ECG data.
    /** Configures the data-logger to log ECG only **/
    void configureDataLoggerECG();

    // Record only IMU data.
    /** Configures the data-logger to log IMU only **/
    void configureDataLoggerIMU();

    // Record ECG + IMU.
    /** Configures the data-logger to log ECG + IMU **/
    void configureDataLoggerAll();

    // Record Nothing.
    /** Configures the data-logger to nothing **/
    void configureDataLoggerNone();

    /**
     * @brief Starts the data logger to log Measurement Data according to its configuration.
     */
    void startDataLogger();

    /**
     * @brief Stops the data logger.
     */
    void stopDataLogger();

    /** The ID of the currently accessed DataLogEntry (normally 1) **/
    size_t mCurrentLogEntryId;

    // For reading recorded data.
    /**
     * @brief Start the streaming of recorded Logs to the BLE-Client.
     */
    void startLogStreaming();

    /**
     * @brief Delete all recorded Logs from the storage.
     */
    void deleteRecordedData();

    /**
     * @brief Cleanup after reading the logbook by resetting all used variables.
     */
    void finishCurrentReadOperation();

    // Recording States:
    // 0 -> No Recording
    // 1 -> Recording
    /** Recording states for ECG **/
    uint8_t mEcgRecordingMode;

    /** Recording states for IMU **/
    uint8_t mImuRecordingMode;

    // Operation States:
    // 0 -> Operation not ongoing
    // 1 -> Operation ongoing

    /** Recording Operation Status **/
    uint8_t mRecordingOperation;
    /** Log-Read and Send Status **/
    uint8_t mGetDataOperation;
    /** Delete all logs Operation Status **/
    uint8_t mDeleteDataOperation;

    /**
     * @brief Update the value of the configuration field visible to the
     * BLE-Client, based on each individual fields value.
     */
    void refreshConfigurationFields();

    // Time Synchronization:
    /**
     * @brief Sets the timestamp of the MeasurementProvider, based on the
     * one set to mSynchronisationTimestamp by the BLE-Client over the
     * ConfigurationField Characteristic.
     */
    void setTimestamp();

    /** SynchronisationTimestamp value set by BLE-Client **/
    int64_t mSynchronizationTimestamp;

    // Debug Functions
    /** Timer Resource for the Blink timer **/
    wb::ResourceId mTimeResource;

    /** Timer for Blink Debugging **/
    wb::TimerId mBlinkTimer;

    /** Counter for Blink Debugging **/
    uint32_t mBlinkCounter;

    /**
     * @brief Make the Sensors Red Blinker blink for n times.
     * @param n
     */
    void startBlinker(uint32_t n);

    /**
     * @brief Send a debug string encoded to the Recording Characteristic.
     * @param msg
     */
    void debug(const char* msg);

    /**
     * @brief Send a debug format string encoded to the Recording
     * Characteristic, containing the value x.
     * @param msg fmt-string
     * @param x value appearing in the fmt-string
     */
    void debugf(const char* msg, int64_t x);
};
