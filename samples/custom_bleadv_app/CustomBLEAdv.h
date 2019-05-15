#pragma once

#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class CustomBLEAdv FINAL : private wb::ResourceClient,
                           public wb::LaunchableModule

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

    /** @see whiteboard::ResourceClient::onPutResult */
    virtual void onPutResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode,
                             const wb::Value& rResultData);

protected:
    /** @see whiteboard::ResourceClient::onTimer */
    virtual void onTimer(wb::TimerId timerId) OVERRIDE;

private:
    wb::TimerId mTimer;
    uint16_t mData;
};
