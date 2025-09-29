#pragma once

#include "movesense.h"

class OfflineStorageGattClient FINAL : private wb::ResourceClient,
                                       private wb::LaunchableModule
{
  public:
    static const char* const LAUNCHABLE_NAME;

    OfflineStorageGattClient();
    ~OfflineStorageGattClient() = default;

  private:

    // From wb::LaunchableModule:
    bool initModule() OVERRIDE;
    void deinitModule() OVERRIDE;
    bool startModule() OVERRIDE;
    void stopModule() OVERRIDE;

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
    void configGattSvc();
    void deconfigGattSvc();
    void initGattCharSubscriptions();
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
    bool mClientIsListeningToEcg;
    bool mClientIsListeingToMov;
    bool mClientIsListeningToRecorded;

    // Measurement Interval settings.
    uint8_t mEcgMeasurementInterval;
    uint8_t mImuMeasurementInterval;

    void parseConfigurationField(const uint8_t*);

    // For Data Recording.

    // Record only ECG data.
    void configureDataLoggerECG();

    // Record only IMU data.
    void configureDataLoggerIMU();

    // Record ECG + IMU.
    void configureDataLoggerAll();

    // Record Nothing.
    void configureDataLoggerNone();

    void startDataLogger();
    void stopDataLogger();

    size_t mCurrentLogEntryId;

    // For reading recorded data.
    void startLogStreaming();
    void deleteRecordedData();

    void finishCurrentReadOperation();

    // RecordingStates
    uint8_t mEcgRecordingMode;
    uint8_t mImuRecordingMode;

    // Operation states
    uint8_t mRecordingOperation;
    uint8_t mGetDataOperation;
    uint8_t mDeleteDataOperation;
    void refreshConfigurationFields();

    // TODO: remove debug functions
    wb::TimerId mBlinkTimer;
    uint32_t mBlinkCounter;
    void startBlinker(uint32_t);
    void debug(const char* msg);
    void debugf(const char*, int64_t);
    // End remove
};
