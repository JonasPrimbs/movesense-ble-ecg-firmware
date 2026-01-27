#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

#include "AutoClientAbsoluteSeriesBuffer.h"
#include "AutoClientMeasurementConfig.h"

/**
 * Movesense-Provider for ECG and IMU Measurement-Data.
 * Uses the native Resources /Meas/ECG and /Meas/Imu, shrinks
 * the data down to 16-bit and emits the data in Chunks of size
 * 16 (ECG) or 8 (IMU) together with a 64-bit unix timestamp.
 */
class DataProvider FINAL : private wb::ResourceProvider,
                                  private wb::ResourceClient,
                                  public wb::LaunchableModule
{
  public:
    static const char* const LAUNCHABLE_NAME;
    DataProvider();
    ~DataProvider();

  private:
    // From wb::LaunchableModule:
    bool initModule() OVERRIDE;
    void deinitModule() OVERRIDE;
    bool startModule() OVERRIDE;
    void stopModule() OVERRIDE;

  private:
    // From wb::ResourceProvider:
    void onPutRequest(const wb::Request& request,
                      const wb::ParameterList& parameters) OVERRIDE;

    void onSubscribe(const whiteboard::Request &rRequest, const whiteboard::ParameterList &rParameters) OVERRIDE;
    void onUnsubscribe(const whiteboard::Request &rRequest, const whiteboard::ParameterList &rParameters) OVERRIDE;

    // From wb::ResourceClient:
    void onNotify(whiteboard::ResourceId resourceId,
                  const whiteboard::Value& rValue,
                  const whiteboard::ParameterList& rParameters) OVERRIDE;

  private:
    // For ECG data
    /** Interval in ms between ECG-measurements **/
    uint8_t mEcgMeasurementInterval;

    /** For counting how many ECG samples where received since the last one was added **/
    uint8_t mEcgSampleCounter;

    /** Sets how many ECG samples are skipped before one is added to the buffer **/
    uint8_t mEcgSampleSkipCount;

    /**
     * @brief Sets the new measurement Interval (ms) for ECG.
     * @param newEcgInterval in ms
     */
    void setEcgMeasurementInterval(uint8_t newEcgInterval);

    /**
     * Converts a 32-bit ecg-sample to a 16-bit sample.
     * @param rawSample ecg-sample as received by the native resource
     * @return downsized sample to 16-bit
     */
    ecg_t prepareEcgSample(int32 rawSample);

    // For IMU data
    /** Interval in ms between IMU-measurements **/
    uint8_t mImuMeasurementInterval;

    /** For counting how many IMU samples where received since the last one was added **/
    uint8_t mImuSampleCounter;

    /** Sets how many IMU samples are skipped before one is added to the buffer **/
    uint8_t mImuSampleSkipCount;

    /**
     * @brief Sets the new measurement interval (ms) for IMU.
     * @param newImuInterval in ms
     */
    void setImuMeasurementInterval(uint8_t newImuInterval);

    /**
     * @brief Converts 3 FloatVectors for Acc, Gyro and Mag to a mov_t struct.
     * @return 9-D imu-sample
     */
    mov_t prepareImuSample(wb::FloatVector3D,
                           wb::FloatVector3D,
                           wb::FloatVector3D);

    /**
     * @brief Converts single acc Float32Vector to an Int16Vector
     * @return prepared acc vector
     */
    acc_vec_t prepareAccSample(wb::FloatVector3D);

    /**
     * @brief Converts single gyr Float32Vector to an Int16Vector
     * @return prepared mag vector
     */
    gyr_vec_t prepareGyrSample(wb::FloatVector3D);

    /**
     * @brief Converts single mag Float32Vector to an Int16Vector
     * @return prepared mag vector
     */
    mag_vec_t prepareMagSample(wb::FloatVector3D);

    // For time synchronization.
    /** Timestamp for when the sensor time has started (microseconds). **/
    int64_t mUnixBaseTimestampUs;

    /** Series Buffer for ECG data. **/
    AutoClientAbsoluteSeriesBuffer<ecg_t>* ecgBuffer;

    /** Series Buffer for IMU/Movement data. **/
    AutoClientAbsoluteSeriesBuffer<mov_t>* movBuffer;

    void pdateMeasState();
    bool mainIsListeningECG;
    bool mainIsListeningIMU9;

};
