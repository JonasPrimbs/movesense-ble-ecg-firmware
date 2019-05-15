#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "app-resources/resources.h"

class BleConformanceTesterService FINAL : private wb::ResourceClient,
                                          private wb::ResourceProvider,
                                          public wb::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    BleConformanceTesterService();
    ~BleConformanceTesterService();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /** @see whiteboard::ResourceProvider::onPutRequest */
    virtual void onPutRequest(const wb::Request& request,
                              const wb::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceClient::onPutResult */
    void onPutResult(wb::RequestId requestId,
                     wb::ResourceId resourceId,
                     wb::Result resultCode,
                     const wb::Value& resultData) OVERRIDE;

    /** @see whiteboard::ResourceClient::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

    void runTest();

private:
    wb::TimerId mStartTestTimer;
    WB_RES::BleTestParams mBleTestParams;
};
