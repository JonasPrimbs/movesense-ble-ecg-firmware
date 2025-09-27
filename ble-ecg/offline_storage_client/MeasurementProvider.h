#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

#include "../SeriesBuffer.h"
#include "MeasurementConfig.h"

class MeasurementProvider FINAL : private wb::ResourceProvider,
                                  private wb::ResourceClient,
                                  public wb::LaunchableModule
{
  public:
    static const char* const LAUNCHABLE_NAME;
    MeasurementProvider();
    ~MeasurementProvider();

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

  private:
    // From wb::ResourceClient:
    void onNotify(whiteboard::ResourceId resourceId,
                  const whiteboard::Value& rValue,
                  const whiteboard::ParameterList& rParameters) OVERRIDE;

  private:
    // For ECG data
    uint8_t mEcgMeasurementInterval;
    uint8_t mEcgSampleCounter;
    uint8_t mEcgSampleSkipCount;
    void setEcgMeasurementInterval(uint8_t);
    ecg_t prepareEcgSample(int32);

    // For IMU data
    uint8_t mImuMeasurementInterval;
    uint8_t mImuSampleCounter;
    uint8_t mImuSampleSkipCount;
    void setImuMeasurementInterval(uint8_t);
    mov_t prepareImuSample(wb::FloatVector3D,
                            wb::FloatVector3D,
                            wb::FloatVector3D);

    acc_vec_t prepareAccSample(wb::FloatVector3D);
    gyr_vec_t prepareGyrSample(wb::FloatVector3D);
    mag_vec_t prepareMagSample(wb::FloatVector3D);

    // Series Buffer for ECG data.
    SeriesBuffer<ecg_t>* ecgBuffer;

    // Series Buffer for IMU/Movement data.
    SeriesBuffer<mov_t>* movBuffer;
};
