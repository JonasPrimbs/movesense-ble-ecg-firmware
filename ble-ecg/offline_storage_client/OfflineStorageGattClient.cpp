#include "OfflineStorageGattClient.h"

#include "app-resources/resources.h"
#include "comm_ble_gattsvc/resources.h"
#include "mem_datalogger/resources.h"
#include "mem_logbook/resources.h"
#include "ui_ind/resources.h"

#include "../WakeClient.h"
#include "GattConfig.h"

const char* const OfflineStorageGattClient::LAUNCHABLE_NAME =
    "OfflineStorageGattClient";

OfflineStorageGattClient::OfflineStorageGattClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    // Debug Blinker:
    mBlinkTimer(wb::ID_INVALID_RESOURCE),
    mBlinkCounter(0),

    // Gatt handles and resources:
    mActivityServiceHandle(0),
    mCharAHandle(0),
    mCharAResource(wb::ID_INVALID_RESOURCE),
    mCharBHandle(0),
    mCharBResource(wb::ID_INVALID_RESOURCE),
    mCharCHandle(0),
    mCharCResource(wb::ID_INVALID_RESOURCE),
    mCharDHandle(0),
    mCharDResource(wb::ID_INVALID_RESOURCE),

    // Configuration Fields
    // Measurement Intervals:
    mEcgMeasurementInterval(IMU_DEFAULT_MEASUREMENT_INTERVAL),
    mImuMeasurementInterval(IMU_DEFAULT_MEASUREMENT_INTERVAL),
    // State fields:
    mEcgRecordingMode(0),
    mImuRecordingMode(0),
    // Operation fields:
    mRecordingOperation(0),
    mGetDataOperation(0),
    mDeleteDataOperation(0),

    // DataLogger related:
    mCurrentLogEntryId(0)
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

    mBlinkTimer = wb::ID_INVALID_TIMER;
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
    case WB_RES::LOCAL::MEM_LOGBOOK_ENTRIES::LID: {
        const WB_RES::LogEntries& entries =
            rResultData.convertTo<const WB_RES::LogEntries&>();

        for (const WB_RES::LogEntry& entry : entries.elements)
        {
            mCurrentLogEntryId = entry.id;
        }
        if (mCurrentLogEntryId == 0)
        {
            this->startBlinker(10);
            finishCurrentReadOperation();
            break;
        }

        asyncGet(WB_RES::LOCAL::MEM_LOGBOOK_BYID_LOGID_DATA(),
                 AsyncRequestOptions::ForceAsync, mCurrentLogEntryId);
        break;
    }

    case WB_RES::LOCAL::MEM_LOGBOOK_BYID_LOGID_DATA::LID: {
        const wb::ByteStream& byteStream =
            rResultData.convertTo<const wb::ByteStream&>();

        // Send off the raw logs.
        const size_t length = byteStream.length();
        WB_RES::Characteristic recordedDataChar;

        // Send in two halves if it is large enough, otherwise just one message.
        if (length > LOG_TRANSMISSION_MTU)
        {
            recordedDataChar.bytes =
                wb::MakeArray<uint8_t>(byteStream.data, LOG_TRANSMISSION_MTU);
            asyncPut(mCharDResource, AsyncRequestOptions::Empty,
                     recordedDataChar);
            recordedDataChar.bytes =
                wb::MakeArray<uint8_t>(byteStream.data + LOG_TRANSMISSION_MTU,
                                       length - LOG_TRANSMISSION_MTU);
            asyncPut(mCharDResource, AsyncRequestOptions::Empty,
                     recordedDataChar);
        }
        else
        {
            recordedDataChar.bytes =
                wb::MakeArray<uint8_t>(byteStream.data, length);
            asyncPut(mCharDResource, AsyncRequestOptions::Empty,
                     recordedDataChar);
        }

        if (resultCode == wb::HTTP_CODE_CONTINUE)
        {
            // Do another get request to get the next block of data.
            asyncGet(WB_RES::LOCAL::MEM_LOGBOOK_BYID_LOGID_DATA(),
                     AsyncRequestOptions::ForceAsync, mCurrentLogEntryId);
        }
        else if (resultCode == wb::HTTP_CODE_OK)
        {
            finishCurrentReadOperation();
        }
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

        // Service created successfully, save its handle.
        mActivityServiceHandle = rResultData.convertTo<int32_t>();

        // Now, get the full details of the service to retrieve characteristic
        // handles.
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
            parseConfigurationField(charValue.bytes.begin());
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
        uint8_t sendBuffer[36];
        const WB_RES::EcgBundle& packet =
            rValue.convertTo<const WB_RES::EcgBundle&>();

        memcpy(sendBuffer, &packet.timestamp, 4);
        memcpy(sendBuffer + 4, packet.samples.begin(), 32);

        WB_RES::Characteristic ecgVoltageCharacteristic;
        ecgVoltageCharacteristic.bytes = wb::MakeArray(sendBuffer);

        // Put value onto Ecg-Characteristic to notify listening client.
        asyncPut(mCharAResource, AsyncRequestOptions::Empty,
                 ecgVoltageCharacteristic);
        break;
    }
    // IMU Data:
    case WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM::LID: {
        // Get raw bytes from the packet.
        const WB_RES::ImuBundle& packet =
            rValue.convertTo<const WB_RES::ImuBundle&>();

        uint8_t sendBuffer[76];
        WB_RES::Characteristic movVectorCharacteristic;
        memcpy(sendBuffer, &packet.timestamp, 4);
        memcpy(sendBuffer + 4, packet.samples.begin(), 72);
        movVectorCharacteristic.bytes = wb::MakeArray(sendBuffer);

        // Put value onto Mov-Characteristic to notify listening client.
        asyncPut(mCharBResource, AsyncRequestOptions::Empty,
                 movVectorCharacteristic);
        break;
    }
    }
}

void OfflineStorageGattClient::onTimer(wb::TimerId timerId)
{
    // Only react on the blink-timer. (no other timer used in this module)
    if (timerId == this->mBlinkTimer)
    {
        // Stop the blinking if the blink-counter has reached 0.
        if (this->mBlinkCounter == 0)
        {
            this->stopTimer(this->mBlinkTimer);
            this->mBlinkTimer = wb::ID_INVALID_TIMER;
            return;
        }
        // Blink once.
        const WB_RES::VisualIndType type =
            WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
        this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(),
                       AsyncRequestOptions::Empty, type);

        // Decrement the blink-counter.
        this->mBlinkCounter -= 1;
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
        reinterpret_cast<const uint8_t*>(&INITIAL_C), CONFIGURATION_FIELD_SIZE);
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

void OfflineStorageGattClient::parseConfigurationField(
    const uint8_t* configFields)
{
    // Measurement intervals.
    uint8_t ecgMeasurementInterval = configFields[0];
    uint8_t imuMeasurementInterval = configFields[1];

    // Potentially update ECG measurement interval.
    if (ecgMeasurementInterval != mEcgMeasurementInterval)
    {
        asyncPut(WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_INTERVAL(),
                 AsyncRequestOptions::Empty, ecgMeasurementInterval);

        this->mEcgMeasurementInterval = ecgMeasurementInterval;
    }

    // Potentially update IMU measurement interval.
    if (imuMeasurementInterval != mImuMeasurementInterval)
    {
        asyncPut(WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_INTERVAL(),
                 AsyncRequestOptions::Empty, imuMeasurementInterval);
        this->mImuMeasurementInterval = imuMeasurementInterval;
    }

    // Recording Modes.
    uint8_t ecgRecordingMode = configFields[2];
    uint8_t imuRecordingMode = configFields[3];

    // Potentially update Recording Configuration.
    if (ecgRecordingMode != mEcgRecordingMode ||
        imuRecordingMode != mImuRecordingMode)
    {
        if (ecgRecordingMode && imuRecordingMode)
        {
            configureDataLoggerAll();
            this->mEcgRecordingMode = true;
            this->mImuRecordingMode = true;
        }
        else if (ecgRecordingMode && !imuRecordingMode)
        {
            configureDataLoggerECG();
            this->mEcgRecordingMode = true;
            this->mImuRecordingMode = false;
        }
        else if (!ecgRecordingMode && imuRecordingMode)
        {
            configureDataLoggerIMU();
            this->mEcgRecordingMode = false;
            this->mImuRecordingMode = true;
        }
        else
        {
            configureDataLoggerNone();
            this->mEcgRecordingMode = false;
            this->mImuRecordingMode = false;
        }
    }

    // Recording Operation
    uint8_t recordingOperation = configFields[4];

    // Enable recording in case 0 -> 1
    if (!mRecordingOperation && recordingOperation)
    {
        startDataLogger();
        this->mRecordingOperation = 1;
        WakeClient::deactivate();
        asyncUnsubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM());
        asyncUnsubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM());
    }

    // Disable recording in case 1 -> 0
    else if (mRecordingOperation && !recordingOperation)
    {
        stopDataLogger();
        this->mRecordingOperation = 0;

        WakeClient::activate();
        asyncSubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_ECG_DATASTREAM());
        asyncSubscribe(WB_RES::LOCAL::MEASUREMENTPROVIDER_IMU9_DATASTREAM());
    }

    uint8_t getDataOperation = configFields[5];

    // Send recorded data operation (only if not recording currently and not
    // already in get-data operation).
    if (!mRecordingOperation && !mGetDataOperation && !mDeleteDataOperation &&
        getDataOperation)
    {
        // TODO: no streaming if charD not enabled.
        startLogStreaming();
        mGetDataOperation = true;
    }

    uint8_t deleteDataOperation = configFields[6];

    // Delete recorded data operation (only if not recording now, no ongoing
    // data transfer or already started deletion operation).
    if (!mRecordingOperation && !mGetDataOperation && !mDeleteDataOperation &&
        deleteDataOperation)
    {
        deleteRecordedData();
        // TODO: return to value 0 only on delete result.
    }

    // Refresh the configuration field for potential updates.
    refreshConfigurationFields();
}
void OfflineStorageGattClient::refreshConfigurationFields()
{
    // Set the value of the characteristic to give client response information.
    uint8_t values[sizeof(uint64_t)] = {
        mEcgMeasurementInterval, mImuMeasurementInterval,
        mEcgRecordingMode,       mImuRecordingMode,
        mRecordingOperation,     mGetDataOperation,
        mDeleteDataOperation,    0};

    WB_RES::Characteristic configFieldsChar;
    configFieldsChar.bytes = wb::MakeArray(values, sizeof(uint64_t));

    // Actually PUT onto characteristic's value.
    asyncPut(mCharCResource, AsyncRequestOptions::Empty, configFieldsChar);
}

void OfflineStorageGattClient::configureDataLoggerNone()
{
    WB_RES::DataLoggerConfig dlConfig;
    asyncPut(WB_RES::LOCAL::MEM_DATALOGGER_CONFIG(), AsyncRequestOptions::Empty,
             dlConfig);
}

void OfflineStorageGattClient::configureDataLoggerAll()
{
    startBlinker(2);
    WB_RES::DataLoggerConfig dlConfig;
    WB_RES::DataEntry dlEntries[2];
    dlEntries[0].path = "/MeasurementProvider/ECG/DataStream";
    dlEntries[1].path = "/MeasurementProvider/IMU9/DataStream";
    dlConfig.dataEntries.dataEntry = wb::MakeArray(dlEntries, 2);
    asyncPut(WB_RES::LOCAL::MEM_DATALOGGER_CONFIG(), AsyncRequestOptions::Empty,
             dlConfig);
}
void OfflineStorageGattClient::configureDataLoggerECG()
{
    startBlinker(2);
    WB_RES::DataLoggerConfig dlConfig;
    WB_RES::DataEntry dlEntry;
    dlEntry.path = "/MeasurementProvider/ECG/DataStream";
    dlConfig.dataEntries.dataEntry = wb::MakeArray(&dlEntry, 1);
    asyncPut(WB_RES::LOCAL::MEM_DATALOGGER_CONFIG(), AsyncRequestOptions::Empty,
             dlConfig);
}
void OfflineStorageGattClient::configureDataLoggerIMU()
{
    startBlinker(2);
    WB_RES::DataLoggerConfig dlConfig;
    WB_RES::DataEntry dlEntry;
    dlEntry.path = "/MeasurementProvider/IMU9/DataStream";
    dlConfig.dataEntries.dataEntry = wb::MakeArray(&dlEntry, 1);
    asyncPut(WB_RES::LOCAL::MEM_DATALOGGER_CONFIG(), AsyncRequestOptions::Empty,
             dlConfig);
}

void OfflineStorageGattClient::startDataLogger()
{
    startBlinker(3);
    // Start Logging with the configured LoggerConfig by PUTting the Logger
    // in the LOGGING state.
    asyncPut(WB_RES::LOCAL::MEM_DATALOGGER_STATE(), AsyncRequestOptions::Empty,
             WB_RES::DataLoggerStateValues::DATALOGGER_LOGGING);
}

void OfflineStorageGattClient::stopDataLogger()
{
    startBlinker(3);
    // Stop Logging by PUTting the logger in the READY state.
    asyncPut(WB_RES::LOCAL::MEM_DATALOGGER_STATE(), AsyncRequestOptions::Empty,
             WB_RES::DataLoggerStateValues::DATALOGGER_READY);
}

void OfflineStorageGattClient::startLogStreaming()
{
    startBlinker(4);
    asyncGet(WB_RES::LOCAL::MEM_LOGBOOK_ENTRIES());
    // Continue logic in onGetResult() case Mem/Logbook/Entries.
}

void OfflineStorageGattClient::deleteRecordedData()
{
    startBlinker(5);
    // Delete all Logbook entries.
    asyncDelete(WB_RES::LOCAL::MEM_LOGBOOK_ENTRIES());
}

void OfflineStorageGattClient::finishCurrentReadOperation()
{
    // Mark the stream-end by sending a single 0.
    constexpr uint8_t ZERO = 0;
    WB_RES::Characteristic recordedDataChar;
    recordedDataChar.bytes = wb::MakeArray(&ZERO, 1);
    asyncPut(mCharDResource, AsyncRequestOptions::Empty, recordedDataChar);
    // Reset log entry id to 0 for next READ operation.
    this->mCurrentLogEntryId = 0;
    this->mGetDataOperation = 0;
    refreshConfigurationFields();
}

// TODO: remove after debug.
void OfflineStorageGattClient::startBlinker(const uint32_t n)
{
    constexpr size_t SPECIAL_INDICATION_BLINK_PERIOD = 250;
    this->mBlinkCounter = n;
    this->mBlinkTimer = this->startTimer(SPECIAL_INDICATION_BLINK_PERIOD, true);
}

void OfflineStorageGattClient::debug(const char* msg)
{
    WB_RES::Characteristic recordedDataChar;
    recordedDataChar.bytes = wb::MakeArray((const uint8_t*)msg, strlen(msg));
    asyncPut(mCharDResource, AsyncRequestOptions::ForceAsync, recordedDataChar);
}

void OfflineStorageGattClient::debugf(const char* fmtstr, int64_t x)
{
    char buffer[100];
    sprintf(buffer, fmtstr, x);
    debug(buffer);
}
