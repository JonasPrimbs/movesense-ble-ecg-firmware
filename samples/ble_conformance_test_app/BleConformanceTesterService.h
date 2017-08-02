#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/containers/RequestMap.h>

#include "app-resources/resources.h"

class BleConformanceTesterService FINAL : private whiteboard::ResourceClient,
                                          private whiteboard::ResourceProvider,
                                          public whiteboard::LaunchableModule

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
    virtual void stopModule() OVERRIDE { mModuleState = WB_RES::ModuleStateValues::STOPPED; }

    /**
    *	PUT request handler.
    *
    *	@param requestId ID of the request
    *	@param clientId ID of the client that should receive the result
    *	@param resourceId ID of the associated resource
    *	@param parameters List of parameters for the request
    *	@return Result of the operation
    */
    virtual void onPutRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    /**
    *	Callback for asynchronous resource PUT requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    void onPutResult(wb::RequestId requestId, wb::ResourceId resourceId,
                     wb::Result resultCode, const wb::Value& resultData) OVERRIDE;

    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

private:
    WB_RES::BleTestParams mBleTestParams;
    void runTest();

    whiteboard::TimerId mStartTestTimer;
};
