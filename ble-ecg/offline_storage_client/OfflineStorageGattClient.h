#pragma once

#include "movesense.h"

class OfflineStorageGattClient final : private wb::ResourceClient,
                                       private wb::LaunchableModule
{
  public:
    static const char* const LAUNCHABLE_NAME;

    OfflineStorageGattClient();
    ~OfflineStorageGattClient() = default;

  private:

    // From wb::LaunchableModule:
    bool initModule() OVERRIDE;
    void deinitModule() OVERRIDE;
    bool startModule() OVERRIDE;
    void stopModule() OVERRIDE;

    // From wb::ResourceClient:
    void onGetResult(wb::RequestId requestId,
                     wb::ResourceId resourceId,
                     wb::Result resultCode,
                     const wb::Value& rResultData) OVERRIDE;

    void onPostResult(wb::RequestId requestId,
                      wb::ResourceId resourceId,
                      wb::Result resultCode,
                      const wb::Value& rResultData) OVERRIDE;

    void onNotify(wb::ResourceId resourceId,
                  const wb::Value& rValue,
                  const wb::ParameterList& rParameters) OVERRIDE;


    // GATT Service configuration methods:
    void configGattSvc();
    void deconfigGattSvc();
    void initGattCharSubscriptions();
    void deinitGattCharSubscriptions();

    // Member variables for GATT handles and resources:
    int32_t mActivityServiceHandle;

    int32_t mCharAHandle;
    wb::ResourceId mCharAResource;

    int32_t mCharBHandle;
    wb::ResourceId mCharBResource;

    int32_t mCharCHandle;
    wb::ResourceId mCharCResource;

    int32_t mCharDHandle;
    wb::ResourceId mCharDResource;
};
