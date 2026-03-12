#pragma once

#include <comm_ble/resources.h>
#include "comm_ble_gattsvc/resources.h"
#include <whiteboard/LaunchableModule.h>
#include <whiteboard/ResourceClient.h>

class RateTestClient FINAL :
        private wb::ResourceClient,
        public wb::LaunchableModule {
public:
    static const char *const LAUNCHABLE_NAME;

    RateTestClient();

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
                             const wb::Value &rResultData) override;

    /** @see whiteboard::ResourceClient::onPostResult */
    virtual void onPostResult(wb::RequestId requestId,
                              wb::ResourceId resourceId,
                              wb::Result resultCode,
                              const wb::Value &rResultData) override;

    /** @see whiteboard::ResourceClient::onNotify */
    virtual void onNotify(wb::ResourceId resourceId,
                          const wb::Value &rValue,
                          const wb::ParameterList &rParameters) override;

    virtual void onPutResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
                             whiteboard::Result resultCode, const whiteboard::Value &rResultData) override;

    virtual void onTimer(whiteboard::TimerId timerId) override;

private:
    // GATT Service:

    /** The handle of the ECG GATT Service. */
    int32_t gattSvcHandle = 0;

    /**
     * @brief Configures the GATT Service.
     */
    void configGattSvc();

    // GATT Characteristics:
    int32_t charAHandle = 0;
    wb::ResourceId charAResource = wb::ID_INVALID_RESOURCE;
    bool charAIsSubscribed = false;

    int32_t charBHandle = 0;
    wb::ResourceId charBResource = wb::ID_INVALID_RESOURCE;

    int32_t charCHandle = 0;
    wb::ResourceId charCResource = wb::ID_INVALID_RESOURCE;

    int32_t charDHandle = 0;
    wb::ResourceId charDResource = wb::ID_INVALID_RESOURCE;

    int32_t connectionHandle = 0;

    void setCharValue(int32_t svcHandle, int32_t charHandle, uint32_t value);

    void setCharValue(int32_t svcHandle, int32_t charHandle, uint8_t *array, uint32_t len);

    wb::TimerId blinkTimer = wb::ID_INVALID_TIMER;
    uint32_t blinkCounter;
    WB_RES::ConnParams savedConnParams{0, 0, 0, 0};

    void startBlinker(uint32_t n);

    void processCommand(WB_RES::Characteristic characteristic);

    void startDataStreaming();
    void stopDataStreaming();
    // 0 -> pseudo data, 1 -> ecg
    uint8_t streaming_mode = 0;
    wb::TimerId rateTestTimer = wb::ID_INVALID_TIMER;
    uint32_t streamingRate = 1;
    uint8_t dataPacket[156] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9b
    };
    bool switchData = false;
    void startPseudoData();
    void stopPseudoData();

    uint8_t measMultiplicator = 1;
    void startEcgData(uint8_t);
    void stopEcgData(uint8_t);
};
