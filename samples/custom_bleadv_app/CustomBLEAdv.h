#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class CustomBLEAdv FINAL : private whiteboard::ResourceClient,
                           public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    CustomBLEAdv();
    ~CustomBLEAdv();

private:
    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;

    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;

    /**
    *	Callback for asynchronous resource PUT requests
    *
    *	@param requestId ID of the request
    *	@param resourceId Successful request contains ID of the resource
    *	@param resultCode Result code of the request
    *	@param rResultData Successful result contains the request result
    */
    virtual void onPutResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& rResultData);

protected:
    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

private:
    whiteboard::TimerId mTimer;
    uint16_t mData;
};
