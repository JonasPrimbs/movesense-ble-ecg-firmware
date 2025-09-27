#include "OfflineStorageGattClient.h"

#include "app-resources/resources.h"
#include "comm_ble_gattsvc/resources.h"
#include "movesense.h"

#include "GattConfig.h"

const char* const OfflineStorageGattClient::LAUNCHABLE_NAME =
    "OfflineStorageGattClient";

OfflineStorageGattClient::OfflineStorageGattClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    // Gatt related:
    mActivityServiceHandle(0),
    mCharAHandle(0),
    mCharAResource(wb::ID_INVALID_RESOURCE),
    mCharBHandle(0),
    mCharBResource(wb::ID_INVALID_RESOURCE),
    mCharCHandle(0),
    mCharCResource(wb::ID_INVALID_RESOURCE),
    mCharDHandle(0),
    mCharDResource(wb::ID_INVALID_RESOURCE)
{
}

bool OfflineStorageGattClient::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void OfflineStorageGattClient::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool OfflineStorageGattClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // First, configure the Offline Gatt Activity Service.
    configGattSvc();

    // Subscribe to measurement data.
    asyncSubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM());
    asyncSubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM());

    return true;
}

void OfflineStorageGattClient::stopModule()
{
    // Unsubscribe from all measurement data.
    asyncUnsubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM());
    asyncUnsubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM());

    // Unsubscribe from all Gatt characteristics.
    deinitGattCharSubscriptions();

    // Set all resources to invalid.
    deconfigGattSvc();
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void OfflineStorageGattClient::onGetResult(wb::RequestId requestId,
                                           wb::ResourceId resourceId,
                                           wb::Result resultCode,
                                           const wb::Value& rResultData)
{
    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID: {
        if (resultCode != wb::HTTP_CODE_OK)
            return;

        const WB_RES::GattSvc& svc =
            rResultData.convertTo<const WB_RES::GattSvc&>();

        // Get each characteristic's handle.
        for (const WB_RES::GattChar& chr : svc.chars)
        {
            uint16_t uuid16 =
                reinterpret_cast<const uint16_t*>(chr.uuid.begin())[0];
            switch (uuid16)
            {
            case UUID_A:
                mCharAHandle =
                    chr.handle.hasValue() ? chr.handle.getValue() : 0;
                break;
            case UUID_B:
                mCharBHandle =
                    chr.handle.hasValue() ? chr.handle.getValue() : 0;
                break;
            case UUID_C:
                mCharCHandle =
                    chr.handle.hasValue() ? chr.handle.getValue() : 0;
                break;
            case UUID_D:
                mCharDHandle =
                    chr.handle.hasValue() ? chr.handle.getValue() : 0;
                break;
            }
        }

        // Only proceed if all characteristics were configured correctly.
        if (!mCharAHandle || !mCharBHandle || !mCharCHandle || !mCharDHandle)
            return;

        char pathBuffer[32];

        // Get each characteristic's corresponding resource.
        snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d",
                 mActivityServiceHandle, mCharAHandle);
        getResource(pathBuffer, mCharAResource);

        snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d",
                 mActivityServiceHandle, mCharBHandle);
        getResource(pathBuffer, mCharBResource);

        snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d",
                 mActivityServiceHandle, mCharCHandle);
        getResource(pathBuffer, mCharCResource);

        snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d",
                 mActivityServiceHandle, mCharDHandle);
        getResource(pathBuffer, mCharDResource);

        // Subscribe to the necessary characteristics.
        initGattCharSubscriptions();
        break;
    }
    }
}

void OfflineStorageGattClient::onPostResult(wb::RequestId requestId,
                                            wb::ResourceId resourceId,
                                            wb::Result resultCode,
                                            const wb::Value& rResultData)
{
    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::COMM_BLE_GATTSVC::LID: {
        if (resultCode != wb::HTTP_CODE_CREATED)
            return;

        // Service created successfully, save its handle
        mActivityServiceHandle = rResultData.convertTo<int32_t>();

        // Now, get the full details of the service to retrieve characteristic
        // handles
        asyncGet(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(),
                 AsyncRequestOptions::Empty, mActivityServiceHandle);
        // Continue logic in onGetResult() case Comm/Ble/GattSvc.
        break;
    }
    }
}

void OfflineStorageGattClient::onNotify(wb::ResourceId resourceId,
                                        const wb::Value& rValue,
                                        const wb::ParameterList& rParameters)
{
    switch (resourceId.localResourceId)
    {
    // GATT Characteristic change:
    case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID: {
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::
            ParameterListRef parameterRef(rParameters);
        int32_t svcHandle = parameterRef.getSvcHandle();
        if (svcHandle != mActivityServiceHandle)
            return;
        int32_t charHandle = parameterRef.getCharHandle();

        const WB_RES::Characteristic& charValue =
            rValue.convertTo<const WB_RES::Characteristic&>();

        if (charHandle == mCharAHandle)
        {
            if (!charValue.notifications.hasValue())
                return;
            this->mClientIsListeningToEcg = charValue.notifications.getValue();
        }
        else if (charHandle == mCharBHandle)
        {
            if (!charValue.notifications.hasValue())
                return;
            this->mClientIsListeingToMov = charValue.notifications.getValue();
        }
        else if (charHandle == mCharCHandle)
        {
            // TODO: use extend type length.
            uint16_t value =
                reinterpret_cast<const uint16_t*>(charValue.bytes.begin())[0];
            parseConfigurationField(value);
        }
        else if (charHandle == mCharDHandle)
        {
            if (!charValue.notifications.hasValue())
                return;
            this->mClientIsListeningToRecorded =
                charValue.notifications.getValue();
        }

        break;
    }
    // ECG Data:
    case WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM::LID: {
        // Get raw bytes from the packet.
        const WB_RES::MeasurementBundle& packet =
            rValue.convertTo<const WB_RES::MeasurementBundle&>();

        WB_RES::Characteristic ecgVoltageCharacteristic;
        ecgVoltageCharacteristic.bytes = packet.bytes;

        // Put value onto Ecg-Characteristic to notify listening client.
        asyncPut(mCharAResource, AsyncRequestOptions::Empty,
                 ecgVoltageCharacteristic);
        break;
    }
    // IMU Data:
    case WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM::LID: {
        // Get raw bytes from the packet.
        const WB_RES::MeasurementBundle& packet =
            rValue.convertTo<const WB_RES::MeasurementBundle&>();
        WB_RES::Characteristic movVectorCharacteristic;
        movVectorCharacteristic.bytes = packet.bytes;

        // Put value onto Mov-Characteristic to notify listening client.
        asyncPut(mCharBResource, AsyncRequestOptions::Empty,
                 movVectorCharacteristic);
        break;
    }
    }
}

void OfflineStorageGattClient::configGattSvc()
{
    WB_RES::GattSvc activityGattSvc;
    WB_RES::GattChar characteristics[OFFLINE_STORAGE_GATT_CLIENT_CHARS];

    // Define each characteristic by reference for clarity
    WB_RES::GattChar& charA = characteristics[0];
    WB_RES::GattChar& charB = characteristics[1];
    WB_RES::GattChar& charC = characteristics[2];
    WB_RES::GattChar& charD = characteristics[3];

    // TODO: compare with INDICATE.
    // Define properties for each characteristic
    static WB_RES::GattProperty propsA[] = {WB_RES::GattProperty::NOTIFY};
    static WB_RES::GattProperty propsB[] = {WB_RES::GattProperty::NOTIFY};
    static WB_RES::GattProperty propsC[] = {WB_RES::GattProperty::READ,
                                            WB_RES::GattProperty::WRITE};
    static WB_RES::GattProperty propsD[] = {WB_RES::GattProperty::NOTIFY};

    // Assign properties
    charA.props = wb::MakeArray(propsA);
    charB.props = wb::MakeArray(propsB);
    charC.props = wb::MakeArray(propsC);
    charD.props = wb::MakeArray(propsD);

    // Assign UUIDs
    charA.uuid = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&UUID_A), sizeof(UUID_A));
    charB.uuid = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&UUID_B), sizeof(UUID_B));
    charC.uuid = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&UUID_C), sizeof(UUID_C));
    charD.uuid = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&UUID_D), sizeof(UUID_D));

    // Assign initial values
    charA.initial_value = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&INITIAL_A), sizeof(INITIAL_A));
    charB.initial_value = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&INITIAL_B), sizeof(INITIAL_B));
    charC.initial_value = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&INITIAL_C), sizeof(INITIAL_C));
    charD.initial_value = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&INITIAL_D), sizeof(INITIAL_D));

    // Combine characteristics into the service definition
    activityGattSvc.uuid = wb::MakeArray<uint8_t>(
        reinterpret_cast<const uint8_t*>(&OFFLINE_ACTIVITY_SVC_UUID16),
        sizeof(uint16_t));
    activityGattSvc.chars = wb::MakeArray(characteristics);

    // Create Gatt service by POSTing.
    asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty,
              activityGattSvc);
    // Logic continues in onPostResult() case Comm/Ble/Gattsvc.
}

void OfflineStorageGattClient::deconfigGattSvc()
{
    // Invalidate all resource IDs on shutdown
    mCharAResource = wb::ID_INVALID_RESOURCE;
    mCharBResource = wb::ID_INVALID_RESOURCE;
    mCharCResource = wb::ID_INVALID_RESOURCE;
    mCharDResource = wb::ID_INVALID_RESOURCE;
    mActivityServiceHandle = 0;
}

void OfflineStorageGattClient::initGattCharSubscriptions()
{
    // Safe-subscribe to all Gatt characteristics to get notified on
    // value-changes and notification status changes. [!] must use
    // ForceAsync here.
    if (mCharAResource != wb::ID_INVALID_RESOURCE)
        asyncSubscribe(mCharAResource, AsyncRequestOptions::ForceAsync);
    if (mCharBResource != wb::ID_INVALID_RESOURCE)
        asyncSubscribe(mCharBResource, AsyncRequestOptions::ForceAsync);
    if (mCharCResource != wb::ID_INVALID_RESOURCE)
        asyncSubscribe(mCharCResource, AsyncRequestOptions::ForceAsync);
    if (mCharDResource != wb::ID_INVALID_RESOURCE)
        asyncSubscribe(mCharDResource, AsyncRequestOptions::ForceAsync);
}

void OfflineStorageGattClient::deinitGattCharSubscriptions()
{
    // Safe-unsubscribe from all Gatt characteristics.
    if (mCharAResource != wb::ID_INVALID_RESOURCE)
        asyncUnsubscribe(mCharAResource);
    if (mCharBResource != wb::ID_INVALID_RESOURCE)
        asyncUnsubscribe(mCharBResource);
    if (mCharCResource != wb::ID_INVALID_RESOURCE)
        asyncUnsubscribe(mCharCResource);
    if (mCharDResource != wb::ID_INVALID_RESOURCE)
        asyncUnsubscribe(mCharDResource);
}

void OfflineStorageGattClient::parseConfigurationField(uint16_t configField)
{
    // Extract individual values as bytes from configField.
    uint8_t ecgInterval = reinterpret_cast<uint8_t*>(&configField)[0];
    uint8_t imuInterval = reinterpret_cast<uint8_t*>(&configField)[1];

    // Update measurement intervals (operations are idempotent).
    asyncPut(WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_INTERVAL(),
             AsyncRequestOptions::Empty, ecgInterval);

    asyncPut(WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_INTERVAL(),
             AsyncRequestOptions::Empty, imuInterval);
}
