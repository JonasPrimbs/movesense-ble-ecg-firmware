#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class OneWireClient FINAL : private whiteboard::ResourceClient,
                           public whiteboard::LaunchableModule

{
public:
    /** Name of this class. Used in StartupProvider list. */
    static const char* const LAUNCHABLE_NAME;
    OneWireClient();
    ~OneWireClient();

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
    /**
    *	Timer callback.
    *
    *	@param timerId Id of timer that triggered
    */
    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;
    virtual void onGetResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& result) OVERRIDE;
    virtual void onPutResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& result) OVERRIDE;
    virtual void onSubscribeResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId, whiteboard::Result resultCode, const whiteboard::Value& result) OVERRIDE;
    virtual void onNotify(whiteboard::ResourceId resourceId,const whiteboard::Value& value,const whiteboard::ParameterList& parameters) OVERRIDE;
private:
    whiteboard::TimerId mTimer;
    bool m1WireScanAndGetMemOngoing;
    int32_t mSmartConnectorHandle;
    bool mDoubleTapSubscribed;
    
    // WriteMemState
    enum WriteMemState {
        UNDEFINED,
        OneWireActive,
        WriteCmdReadCrc,
        WriteDataReadCrc,
        WriteAA,
        ReadAAStatus
    } mWriteMemState;
    whiteboard::TimerId mAAStatusReadTimer;
};
