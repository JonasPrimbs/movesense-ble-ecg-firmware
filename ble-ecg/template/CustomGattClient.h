#pragma once

#include <comm_ble/resources.h>
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class CustomGattClient FINAL :
    private wb::ResourceClient,
    public wb::LaunchableModule
{
public:
    static const char* const LAUNCHABLE_NAME;

    CustomGattClient();

private:

    // Module:

    /** @see whiteboard::ILaunchableModule::initModule */
    virtual bool initModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::deinitModule */
    virtual void deinitModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::startModule */
    virtual bool startModule() OVERRIDE;
    /** @see whiteboard::ILaunchableModule::stopModule */
    virtual void stopModule() OVERRIDE;


    // Subscriptions:

    /** @see whiteboard::ResourceClient::onGetResult */
    virtual void onGetResult(wb::RequestId requestId,
                             wb::ResourceId resourceId,
                             wb::Result resultCode,
                             const wb::Value& rResultData) override;
    /** @see whiteboard::ResourceClient::onPostResult */
    virtual void onPostResult(wb::RequestId requestId,
                              wb::ResourceId resourceId,
                              wb::Result resultCode,
                              const wb::Value& rResultData) override;
    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId,
                          const wb::Value& rValue,
                          const wb::ParameterList& rParameters) override;

    virtual void onTimer(whiteboard::TimerId timerId) override;

private:

    // GATT Service:

    /** The handle of the ECG GATT Service. */
    int32_t svcHandle = 0;

    /**
     * @brief Configures the GATT Service.
     */
    void configGattSvc();

    // GATT Characteristics:
    int32_t charAHandle = 0;
    wb::ResourceId charAResource = wb::ID_INVALID_RESOURCE;

    int32_t charBHandle = 0;
    wb::ResourceId charBResource = wb::ID_INVALID_RESOURCE;

    int32_t charCHandle = 0;
    wb::ResourceId charCResource = wb::ID_INVALID_RESOURCE;

    // Debug blinking
    wb::TimerId blinkTimer = wb::ID_INVALID_TIMER;
    uint32_t blinkCounter;
    void startBlinker(uint32_t n);
};
