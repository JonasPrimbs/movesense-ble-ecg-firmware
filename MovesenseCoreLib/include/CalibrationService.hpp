// Copyright (c) Suunto Oy 2017. All rights reserved.
#pragma once

#include "nea/common/math/Matrix.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

#include "misc_calibration/resources.h"
#include "common/compiler/genType.h"

enum class CalibrationState : uint8_t {
    IDLE = 0,
    SEQUENCE_ON = 1,
    STEP_ON = 2,
    STEP_DONE = 3,
    SEQUENCE_DONE = 4,
    LAST = SEQUENCE_DONE
};

enum class CalibrationMode : uint8_t {
    NO_MODE = 0,
    PREDEFINED_TILT_SEQUENCE = 1,
    PREDEFINED_MAGNETIC_FIELD_SEQUENCE = 2,
    RANDOM_TILT_SEQUENCE = 3,
    LAST = RANDOM_TILT_SEQUENCE
};

class CalibrationService FINAL : private whiteboard::ResourceClient,
                                 private whiteboard::ResourceProvider,
                                 public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char_t* const LAUNCHABLE_NAME;
    CalibrationService();
    ~CalibrationService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onPutRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                          const whiteboard::ParameterList& parameters);

    /**
    *	Callback for asynchronous UNSUBSCRIBE requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onUnsubscribeResult(whiteboard::RequestId requestId,
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
    virtual void onSubscribeResult(whiteboard::RequestId requestId,
        whiteboard::ResourceId resourceId,
        whiteboard::Result resultCode,
        const whiteboard::Value& rResultData);

    /* Start external resource */
    bool start();
    /* Stop external resource */
    bool stop();
    /* Reset everything */
    void reset();

    /* Changes state of the machine */
    void changeState(const whiteboard::Request& request, CalibrationState state);

    bool beginSequenceOn();
    bool beginStepOn();
    bool beginStepDone();
    bool beginSequenceDone();

    // State variables
    CalibrationState mState;
    CalibrationMode mMode;

    // Calibration sequence
    WB_RES::CalibrationSequence mSequence;
    //References
    float mGravity;
    float mMagneticField;
    uint8_t mAveSamplesAcceleration;
    uint8_t mAveSamplesMagneticField;

    // For gyroscope calibration:
    // Which steps are used x+/x-/y+/y-/z+/z-
    uint8 mStepsForGyro[6];
    // How many degrees we rotate around each axis
    float mGyroRefRot[3];

    // reference in device coordinates
    whiteboard::FloatVector3D mDeviceRef;

    // time differece between highspeed sensor samples in milliseconds
    uint32 mDelta;
    // Step number index
    int mStep;
    // Put request id
    whiteboard::Request mStepRequest;
    // Logical variables to disable measurements between steps
    bool mAccMeasurementActive;
    bool mMagnMeasurementActive;
    bool mGyroMeasurementActive;
    // Keep track of the first angular velocity measurement
    // to initialize the integration
    bool first;
    // Number of measurements
    int mMeasProbes;
    // Measurement vectors
    whiteboard::FloatVector3D mMeasAvgs;
    whiteboard::FloatVector3D mGyroIntegrals;
    whiteboard::FloatVector3D mGyroBias;
    // Measurement matrices
    nea::Matrix u;
    nea::Matrix gm;

    // number of measurements for angular velocity bias
    int mBiasCount;
    // Previous values to calculate the integral using midpoint rule
    uint32 prev_timestamp;
    whiteboard::FloatVector3D prev_gyrovalue;

    // is start called
    bool mStarted;
};
