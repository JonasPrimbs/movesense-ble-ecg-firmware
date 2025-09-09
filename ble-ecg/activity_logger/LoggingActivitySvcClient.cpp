#include "LoggingActivitySvcClient.h"

#include <meas_ecg/resources.h>

#include "common/core/debug.h"
#include "oswrapper/thread.h"

#include "WakeClient.h"
#include "comm_ble_gattsvc/resources.h"
#include "comm_ble_hrs/resources.h"

// Logging Activity GATT Service implementations:

const char* const LoggingActivitySvcClient::LAUNCHABLE_NAME = "LoggingActivitySvc";
LoggingActivitySvcClient::LoggingActivitySvcClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mActivitySvcHandle(0),
    mEcgVoltageCharHandle(0),
    mEcgVoltageCharResource(wb::ID_INVALID_RESOURCE),
    mEcgMeasurementIntervalCharHandle(0),
    mEcgMeasurementIntervalCharResource(wb::ID_INVALID_RESOURCE),
    ecgSampleCounter(0),
    ecgSampleSkipCount(2),
    ecgMeasurementInterval(DEFAULT_ECG_MEASUREMENT_INTERVAL)
{
    this->ecgBuffer = new SeriesBuffer<ecg_t>(DEFAULT_ECG_OBJECT_SIZE, NUMBER_OF_ECG_BUFFERS);
}

LoggingActivitySvcClient::~LoggingActivitySvcClient()
{
    delete this->ecgBuffer;
}

bool LoggingActivitySvcClient::initModule()
{
    DEBUGLOG("LoggingActivitySvcClient::initModule");

    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void LoggingActivitySvcClient::deinitModule()
{
    DEBUGLOG("LoggingActivitySvcClient::deinitModule");

    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool LoggingActivitySvcClient::startModule()
{
    DEBUGLOG("LoggingActivitySvcClient::startModule");

    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Set ECG measurement interval to compute ECG sampling frequency.
    this->setEcgMeasurementInterval(DEFAULT_ECG_MEASUREMENT_INTERVAL);
    // Subscribe to ECG samples with computed ECG sampling frequency.
    this->subscribeToEcgSamples();

    // Configure Activity GATT Service.
    this->configGattSvc();

    return true;
}

void LoggingActivitySvcClient::stopModule()
{
    DEBUGLOG("LoggingActivitySvcClient::stopModule");

    // un-Subscribe from HRS state notifications
    asyncUnsubscribe(WB_RES::LOCAL::COMM_BLE_HRS());

    // Unsubscribe from ECG samples.
    this->unsubscribeFromEcgSamples();
    this->mEcgVoltageCharResource = wb::ID_INVALID_RESOURCE;
    // Unsubscribe from Movement samples.

    // Unsubscribe and clear ECG Measurement Interval GATT characteristics.
    this->mEcgMeasurementIntervalCharResource = wb::ID_INVALID_RESOURCE;

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void LoggingActivitySvcClient::onGetResult(wb::RequestId requestId,
                                           wb::ResourceId resourceId,
                                           wb::Result resultCode,
                                           const wb::Value& rResultData)
{
    DEBUGLOG("LoggingActivitySvcClient::onGetResult");

    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID: {
        DEBUGLOG("LoggingActivitySvcClient::onGetResult - COMM_BLE_GATTSVC_SVCHANDLE");

        const WB_RES::GattSvc& svc = rResultData.convertTo<const WB_RES::GattSvc&>();
        for (size_t i = 0; i < svc.chars.size(); i++)
        {
            const WB_RES::GattChar& c = svc.chars[i];
            uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[0]));

            switch (uuid16)
            {
            case ECG_VOLTAGE_CHARACTERISTIC_UUID16:
                this->mEcgVoltageCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                break;
            case ECG_INTERVAL_CHARACTERISTIC_UUID16:
                this->mEcgMeasurementIntervalCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                break;
            }
        }

        if (!mEcgVoltageCharHandle || !mEcgMeasurementIntervalCharHandle)
        {
            DEBUGLOG("ERROR: Not all characteristics were configured!");
            return;
        }

        char pathBuffer[32] = {'\0'};
        snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", mActivitySvcHandle, mEcgVoltageCharHandle);
        getResource(pathBuffer, mEcgVoltageCharResource);
        snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", mActivitySvcHandle,
                 mEcgMeasurementIntervalCharHandle);
        getResource(pathBuffer, mEcgMeasurementIntervalCharResource);

        // Force subscriptions asynchronously to save stack (will have stack
        // overflow if not)

        // Subscribe to listen to ECG Voltage Characteristics notifications
        // (someone enables/disables the NOTIFY characteristic)
        this->asyncSubscribe(this->mEcgVoltageCharResource, AsyncRequestOptions(NULL, 0, true));
        // Subscribe to listen to ECG Measurement Interval Characteristics
        // notifications (someone writes new value to ecgMeasurementIntervalChar)
        this->asyncSubscribe(this->mEcgMeasurementIntervalCharResource, AsyncRequestOptions(NULL, 0, true));
        break;
    }
    }
}

void LoggingActivitySvcClient::onPostResult(wb::RequestId requestId,
                                            wb::ResourceId resourceId,
                                            wb::Result resultCode,
                                            const wb::Value& rResultData)
{
    DEBUGLOG("LoggingActivitySvcClient::onPostResult: %d", resultCode);

    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // Activity GATT service was created.
        this->mActivitySvcHandle = (int32_t)rResultData.convertTo<uint16_t>();
        DEBUGLOG("Activity GATT service was created. Handle: %d", this->mActivitySvcHandle);

        // Request more info about created GATT service so we get the
        // characteristics handles.
        this->asyncGet(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(), AsyncRequestOptions::Empty,
                       this->mActivitySvcHandle);
    }
}

void LoggingActivitySvcClient::onNotify(wb::ResourceId resourceId,
                                        const wb::Value& value,
                                        const wb::ParameterList& rParameters)
{
    DEBUGLOG("LoggingActivitySvcClient::onNotify");

    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE::LID: {
        // Get ECG Data.
        auto ecgData = value.convertTo<const WB_RES::ECGData&>();

        // Parse timestamp.
        timestamp_t timestamp = (timestamp_t)ecgData.timestamp;

        // Parse samples and put them into sample buffer.
        size_t numberOfSamples = ecgData.samples.getNumberOfItems();
        size_t j = 0;
        for (size_t i = 0; i < numberOfSamples; i++)
        {
            // Update ECG sample counter.
            this->ecgSampleCounter = (this->ecgSampleCounter + 1) % this->ecgSampleSkipCount;
            if (this->ecgSampleCounter == 0)
            {
                // Convert ECG sample.
                auto ecgSample = this->convertEcgSample(ecgData.samples[j]);
                // Add converted sample to ECG buffer.
                this->ecgBuffer->addSample(ecgSample);

                // If buffer is full, add timestamp and send samples.
                if (!this->ecgBuffer->canAddSample())
                {
                    // Compute timestamp.
                    timestamp_t t = timestamp - ((numberOfSamples - i - 1) * this->ecgMeasurementInterval);
                    // Set timestamp to timestamp of last sample in buffer.
                    this->ecgBuffer->setTimestamp(t);
                    // Send samples.
                    this->sendEcgBuffer();
                }

                j++;
            }
        }
        break;
    }
    case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID: {
        // Get the GATT characteristic which is handled now.
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
        auto charHandle = parameterRef.getCharHandle();

        // Set the current GATT characteristic.
        if (charHandle == this->mEcgMeasurementIntervalCharHandle)
        {
            // Set ECG Measurement Interval GATT Characteristic:

            // Parse received Measurement Interval.
            const WB_RES::Characteristic& charValue = value.convertTo<const WB_RES::Characteristic&>();
            uint16_t interval = *reinterpret_cast<const uint16_t*>(&charValue.bytes[0]);

            DEBUGLOG("onNotify: EcgMeasurementInterval: len: %d, new interval: %d", charValue.bytes.size(), interval);

            // Update the ECG Measurement Interval.
            this->setEcgMeasurementInterval(interval);
        }
    }
    }
}

void LoggingActivitySvcClient::configGattSvc()
{
    // Define Activity GATT Service and its Characteristics.
    WB_RES::GattSvc activityGattSvc;
    WB_RES::GattChar characteristics[ACTIVITY_SERVICE_NUM_OF_CHARACTERISTICS];
    WB_RES::GattChar& ecgVoltageChar = characteristics[0];
    WB_RES::GattChar& ecgMeasurementIntervalChar = characteristics[1];

    // Specify Characteristics's properties.
    WB_RES::GattProperty ecgVoltageCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty ecgMeasurementIntervalCharProps[2] = {WB_RES::GattProperty::READ, WB_RES::GattProperty::WRITE};

    // Specify ECG Characteristic.
    ecgVoltageChar.props = wb::MakeArray<WB_RES::GattProperty>(&ecgVoltageCharProp, 1);
    ecgVoltageChar.uuid =
        wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ECG_VOLTAGE_CHARACTERISTIC_UUID16), sizeof(uint16_t));

    // Specify ECG Measurement Interval Characteristic.
    ecgMeasurementIntervalChar.props = wb::MakeArray<WB_RES::GattProperty>(ecgMeasurementIntervalCharProps, 2);
    ecgMeasurementIntervalChar.uuid =
        wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ECG_INTERVAL_CHARACTERISTIC_UUID16), sizeof(uint16_t));
    ecgMeasurementIntervalChar.initial_value =
        wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->ecgMeasurementInterval), sizeof(uint16_t));

    // Combine Characteristics to Service.
    activityGattSvc.uuid =
        wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ACTIVITY_SERVICE_UUID16), sizeof(uint16_t));
    activityGattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, ACTIVITY_SERVICE_NUM_OF_CHARACTERISTICS);

    // Create custom GATT Service.
    this->asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, activityGattSvc);
}

ecg_t LoggingActivitySvcClient::convertEcgSample(int32 ecgValue)
{
    if (ecgValue > ECG_MAX_VALUE)
    {
        return ECG_INVALID_VALUE;
    }
    else if (ecgValue < ECG_MIN_VALUE)
    {
        return ECG_INVALID_VALUE;
    }
    else
    {
        return (ecg_t)ecgValue;
    }
}

bool LoggingActivitySvcClient::sendEcgBuffer()
{
    // Get the current buffer and its size.
    size_t size = this->ecgBuffer->getSingleBufferSize();

    uint8_t* currentBuffer = this->ecgBuffer->getCurrentBuffer();

    // Move to next message buffer.
    this->ecgBuffer->switchBuffer();

    // Generate ECG Voltage Characteristics value to send.
    WB_RES::Characteristic ecgVoltageCharacteristic;
    ecgVoltageCharacteristic.bytes = wb::MakeArray<uint8_t>(currentBuffer, size);

    // Send ECG Voltage characteristics value.
    this->asyncPut(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(), AsyncRequestOptions::Empty,
                   this->mActivitySvcHandle, this->mEcgVoltageCharHandle, ecgVoltageCharacteristic);
    return true;
}

uint32_t LoggingActivitySvcClient::getEcgSampleRate()
{
    return (uint32_t)(1000 / ECG_BASE_MEASUREMENT_INTERVAL);
}

void LoggingActivitySvcClient::setEcgMeasurementInterval(uint16_t value)
{
    // Ensure that value is valid or fall back to
    // `DEFAULT_ECG_MEASUREMENT_INTERVAL`.
    switch (value)
    {
    case 2:                       // 500 Hz
        WakeClient::deactivate(); // deactivate the auto-shutdown
        break;
    case 4:                     // 250 Hz
        WakeClient::activate(); // activate the auto-shutdown
        break;
    case 8:  // 125 Hz
    case 10: // 100 Hz
        break;
    default:
        value = DEFAULT_ECG_MEASUREMENT_INTERVAL;
        break;
    }

    // // Unsubscribe from current ECG subscription.
    // this->unsubscribeFromEcgSamples();
    // Update measurement interval.
    this->ecgMeasurementInterval = value;
    // Update ECG sample skip count.
    this->ecgSampleSkipCount = value / ECG_BASE_MEASUREMENT_INTERVAL;
    // Reset ecg sample counter.
    this->ecgSampleCounter = 0;
    // Reset current ECG buffer and start over.
    this->ecgBuffer->resetCurrentBuffer();
    // Set measurement interval to GATT Characteristics value.
    WB_RES::Characteristic measurementIntervalChar;
    measurementIntervalChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->ecgMeasurementInterval, sizeof(uint16_t));
    // // Subscribe to new ECG subscription.
    // this->subscribeToEcgSamples();
    this->asyncPut(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(), AsyncRequestOptions::Empty,
                   this->mActivitySvcHandle, this->mEcgMeasurementIntervalCharHandle, measurementIntervalChar);
}

void LoggingActivitySvcClient::subscribeToEcgSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getEcgSampleRate();
    // Subscribe to ECG samples with the desired ECG sample rate.
    this->asyncSubscribe(WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(), AsyncRequestOptions::Empty, sampleRate);
}

void LoggingActivitySvcClient::unsubscribeFromEcgSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getEcgSampleRate();
    // Unsubscribe from ECG samples with desired ECG sample rate.
    this->asyncUnsubscribe(WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(), AsyncRequestOptions::Empty, sampleRate);
}