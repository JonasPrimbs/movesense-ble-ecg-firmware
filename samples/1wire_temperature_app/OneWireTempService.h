#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>

#include <whiteboard/containers/RequestQueue.h>

#include "app-resources/resources.h"

constexpr int MAX_THERMOMETERS = 8;
class OneWireTempService FINAL : private wb::ResourceClient,
                                 private wb::ResourceProvider,
                                 public wb::LaunchableModule
{

public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    OneWireTempService();
    ~OneWireTempService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

protected:
    /** @see whiteboard::ResourceProvider::onGetRequest */
    void onGetRequest(const wb::Request& request, const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceClient::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;
    /** @see whiteboard::ResourceClient::onGetResult */
    virtual void onGetResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode, const
                             wb::Value& result) OVERRIDE;
    /** @see whiteboard::ResourceClient::onPutResult */
    virtual void onPutResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode,
                             const wb::Value& result) OVERRIDE;
    /** @see whiteboard::ResourceClient::onSubscribeResult */
    virtual void onSubscribeResult(wb::RequestId requestId,
                                   wb::ResourceId resourceId,
                                   wb::Result resultCode,
                                   const wb::Value& rResultData) OVERRIDE;

    void readFifoCountFromSensor(size_t idx);
    void readTemperatureFromSensor(size_t idx);
    void startTemperatureConversion(size_t idx);

private:
    wb::TimerId mPowerOnTimer;
    wb::TimerId mTimer;

    wb::RequestQueue<4, void> mGetRequestQueue;
    enum
    {
        NO_COMMAND = 0,
        READ_FIFO_COUNT = 1,
        READ_TEMP,
        CONVERT_T
    } mCurrentCmd; // ongoing command: 1=read fifo count, 2 = read temperature, 3=convert T

    size_t mDeviceCount;
    size_t mNextIdxToMeasure;

    WB_RES::TempMeasurement mMeasurements[MAX_THERMOMETERS];
    int32 mDeviceHandles[MAX_THERMOMETERS];
};
