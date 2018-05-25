#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class CustomGATTSvcClient FINAL : private whiteboard::ResourceClient,
                           public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    CustomGATTSvcClient();
    ~CustomGATTSvcClient();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /***
    * Callback for POST operation result
    *
    * @param requestId ID of the request
    * @param resourceId Successful request contains ID of the resource
    * @param resultCode Result code of the request
    * @param rResultData Successful result contains the request result
    *
    * @see whiteboard::ResourceClient::asyncPost
    */
    virtual void onPostResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& rResultData) OVERRIDE;

    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

    /**
    *	Callback for asynchronous resource GET requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onGetResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& rResultData);

    /**
    *	Callback for resource notifications.
    *
    *	@param resourceId Resource id associated with the update
    *	@param rValue Current value of the resource
    *	@param rParameters Notification parameters
    */
    virtual void onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& rValue, const whiteboard::ParameterList& rParameters);

private:
    void configGattSvc();

    whiteboard::ResourceId mIntervalCharResource;
    whiteboard::ResourceId mMeasCharResource;
    whiteboard::TimerId mMeasurementTimer;

    uint16_t mMeasIntervalSecs;
    int32_t mTemperatureSvcHandle;
    int32_t mMeasCharHandle;
    int32_t mIntervalCharHandle;
};
