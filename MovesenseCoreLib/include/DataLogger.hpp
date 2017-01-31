// Copyright (c) Suunto Oy 2016. All rights reserved.
#pragma once

#include "wbresources/resources.h"
#include "buildconfig/hal/features.h"
#include <whiteboard/LaunchableModule.h>

// Logging
#include "logger/DescriptorProvider.hpp"

#include "hwconfig.h"
#include "MeasStorage.h"
#include "ExtflashChunkStorage.h"

#define MAX_RESOURCE_COUNT 8

class DataLogger FINAL : 
    private whiteboard::ResourceClient,
    private whiteboard::ResourceProvider,
    public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    DataLogger(ExtflashChunkStorage &chunkStorage, MeasStorage &measStorage);
    virtual ~DataLogger();

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
    *	GET request handler.
    *
    *	@param requestId ID of the request
    *	@param clientId ID of the client that should receive the result
    *	@param resourceId ID of the associated resource
    *	@param parameters List of parameters for the request
    *	@return Result of the operation
    */
    virtual void onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters) OVERRIDE;

    /** @see whiteboard::ResourceProvider::onPutRequest */
    virtual void onPutRequest(const wb::Request& request,
                      const wb::ParameterList& parameterList) OVERRIDE;                              
    
    /** @see whiteboard::ResourceProvider::onNotify */
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

private:
    bool applyConfig(WB_RES::DataLoggerConfig config);
    static bool validateStateTransition(WB_RES::DataLoggerState currentState, WB_RES::DataLoggerState newState);
    bool isLogging() const;
    whiteboard::Result startLogging();
    whiteboard::Result stopLogging();    
    
    WB_RES::DataLoggerState mDataLoggerState;

    ExtflashChunkStorage &mChunkStorage;
    MeasStorage &mMeasStorage;
        
    volatile bool mSubscribeCompleted;
    volatile bool mUnsubscribeCompleted;

    whiteboard::ResourceId getResourceId(whiteboard::LocalResourceId localResourceId) const;

    inline size_t getResourceCount(const whiteboard::ResourceId::Value resArray[]) const;

    whiteboard::ResourceId::Value mResourcesToLog[MAX_RESOURCE_COUNT]; // Max+1 resourceIds, last is terminator. use the uint32_t "Value" packed form
    whiteboard::ResourceId::Value mSubscribedResources[MAX_RESOURCE_COUNT]; // Max+1 resourceIds, last is terminator
};
