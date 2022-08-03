#include "movesense.h"

#include <meas_ecg/resources.h>

#include "EcgGATTSvcClient.h"
#include "common/core/debug.h"
#include "oswrapper/thread.h"

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"
#include "meas_temp/resources.h"


// At compile-time adjustable constants:

/**
 * @brief Default ECG Measurement Interval in milliseconds.
 * Must be one of the following: 
 *  - 1 ms = 1000 Hz
 *  - 2 ms =  500 Hz
 *  - 4 ms =  250 Hz
 *  - 8 ms =  125 Hz
 */
const int DEFAULT_MEASUREMENT_INTERVAL = 4;

/** Default number of ECG samples per message. */
const uint16_t DEFAULT_OBJECT_SIZE = 16;


// ECG GATT Service implementations:

const char* const EcgGATTSvcClient::LAUNCHABLE_NAME = "EcgGattSvc";

EcgGATTSvcClient::EcgGATTSvcClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mEcgSvcHandle(0),
    mEcgVoltageCharHandle(0),
    mEcgVoltageCharResource(wb::ID_INVALID_RESOURCE),
    mMeasurementIntervalCharHandle(0),
    mMeasurementIntervalCharResource(wb::ID_INVALID_RESOURCE),
    mObjectSizeCharHandle(0),
    mObjectSizeCharResource(wb::ID_INVALID_RESOURCE),
    measurementInterval(DEFAULT_MEASUREMENT_INTERVAL),
    objectSize(DEFAULT_OBJECT_SIZE)
{
}

EcgGATTSvcClient::~EcgGATTSvcClient()
{
}

bool EcgGATTSvcClient::initModule()
{
    DEBUGLOG("EcgGATTSvcClient::initModule");

    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void EcgGATTSvcClient::deinitModule()
{
    DEBUGLOG("EcgGATTSvcClient::deinitModule");

    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool EcgGATTSvcClient::startModule()
{
    DEBUGLOG("EcgGATTSvcClient::startModule");

    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Set object size and allocate ECG Voltages buffer.
    this->setObjectSize(DEFAULT_OBJECT_SIZE);
    // Set measurement interval to compute ECG sampling frequency.
    this->setMeasurementInterval(DEFAULT_MEASUREMENT_INTERVAL);
    // Subscribe to ECG samples with computed ECG sampling frequency.
    this->subscribeToEcgSamples();

    // Configure ECG GATT Service.
    this->configGattSvc();

    return true;
}

void EcgGATTSvcClient::stopModule()
{
    DEBUGLOG("EcgGATTSvcClient::stopModule");

    // Unsubscribe from ECG samples.
    this->unsubscribeFromEcgSamples();
    this->mEcgVoltageCharResource = wb::ID_INVALID_RESOURCE;

    // Unsubscribe and clear Measurement Interval GATT characteristics.
    this->asyncUnsubscribe(this->mMeasurementIntervalCharResource);
    this->mMeasurementIntervalCharResource = wb::ID_INVALID_RESOURCE;

    // Unsubscribe and clear Object Size GATT characteristics.
    this->asyncUnsubscribe(this->mObjectSizeCharResource);
    this->mObjectSizeCharResource = wb::ID_INVALID_RESOURCE;

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void EcgGATTSvcClient::onGetResult(wb::RequestId requestId,
                                   wb::ResourceId resourceId,
                                   wb::Result resultCode,
                                   const wb::Value& rResultData)
{
    DEBUGLOG("EcgGATTSvcClient::onGetResult");

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID:
        {
            DEBUGLOG("EcgGATTSvcClient::onGetResult - COMM_BLE_GATTSVC_SVCHANDLE");

            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc&>();
            for (size_t i = 0; i < svc.chars.size(); i++)
            {
                const WB_RES::GattChar &c = svc.chars[i];
                uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[0]));
                
                switch (uuid16)
                {
                    case ecgVoltageCharUUID16:
                        this->mEcgVoltageCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case measurementIntervalCharUUID16:
                        this->mMeasurementIntervalCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case objectSizeCharUUID16:
                        this->mObjectSizeCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                }
            }

            /*
            if (!this->mEcgVoltageCharHandle || !this->mMeasurementIntervalCharHandle || !this->mObjectSizeCharHandle)
            {
                DEBUGLOG("ERROR: Not all chars were configured!");
                return;
            }

            char pathBuffer[32]= {'\0'};
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", this->mEcgSvcHandle, this->mEcgVoltageCharHandle);
            this->getResource(pathBuffer, this->mEcgVoltageCharResource);
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", this->mEcgSvcHandle, this->mMeasurementIntervalCharHandle);
            this->getResource(pathBuffer, this->mMeasurementIntervalCharResource);
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", this->mEcgSvcHandle, this->mObjectSizeCharHandle);
            this->getResource(pathBuffer, this->mObjectSizeCharResource);
            */

            // Force subscriptions asynchronously to save stack (will have stack overflow if not) 

            // Subscribe to listen to ECG Voltage Characteristics notifications (someone enables/disables the NOTIFY characteristic)
            this->asyncSubscribe(this->mEcgVoltageCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Measurement Interval Characteristics notifications (someone writes new value to measurementIntervalChar) 
            this->asyncSubscribe(this->mMeasurementIntervalCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Object Size Characteristics notifications (someone writes new value to objectSizeChar)
            this->asyncSubscribe(this->mObjectSizeCharResource,  AsyncRequestOptions(NULL, 0, true));
            break;
        }

        // case WB_RES::LOCAL::MEAS_TEMP::LID:
        // {
        //     // Temperature result or error
        //     if (resultCode == wb::HTTP_CODE_OK)
        //     {
        //         WB_RES::TemperatureValue value = rResultData.convertTo<WB_RES::TemperatureValue>();
        //         float temperature = value.measurement;

        //         // Convert K to C
        //         temperature -= 273.15f;

        //         // Return data
        //         uint8_t buffer[5]; // 1 byte or flags, 4 for FLOAT "in Celsius" value
        //         buffer[0]=0;

        //         // convert normal float to IEEE-11073 "medical" FLOAT type into buffer
        //         floatToFLOAT(temperature, &buffer[1]);

        //         // Write the result to measChar. This results INDICATE to be triggered in GATT service
        //         WB_RES::Characteristic newMeasCharValue;
        //         newMeasCharValue.bytes = wb::MakeArray<uint8_t>(buffer, sizeof(buffer));
        //         asyncPut(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(), AsyncRequestOptions::Empty,
        //                  mTemperatureSvcHandle, mMeasCharHandle, newMeasCharValue);
        //     }
        //     break;
        // }
    }
}

void EcgGATTSvcClient::onPostResult(wb::RequestId requestId, 
                                    wb::ResourceId resourceId, 
                                    wb::Result resultCode, 
                                    const wb::Value& rResultData)
{
    DEBUGLOG("EcgGATTSvcClient::onPostResult: %d", resultCode);

    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // ECG GATT service was created.
        this->mEcgSvcHandle = (int32_t)rResultData.convertTo<uint16_t>();
        DEBUGLOG("ECG GATT service was created. Handle: %d", this->mEcgSvcHandle);

        // Request more info about created GATT service so we get the characteristics handles.
        this->asyncGet(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(),
            AsyncRequestOptions::Empty,
            this->mEcgSvcHandle
        );
    }
}

void EcgGATTSvcClient::onNotify(wb::ResourceId resourceId,
                                const wb::Value& value,
                                const wb::ParameterList& rParameters)
{
    DEBUGLOG("EcgGATTSvcClient::onNotify");

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE::LID:
        {
            // Verify that ECG message buffer is allocated.
            if (this->ecgCharacteristicsBuffers == nullptr)
            {
                break;
            }

            // Get ECG Data.
            auto ecgData = value.convertTo<const WB_RES::ECGData&>();

            // Parse timestamp.
            uint32_t timestamp = (uint32_t)ecgData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = ecgData.samples.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert ECG sample.
                auto ecgSample = this->convertEcgSample(ecgData.samples[i]);
                // Add converted sample to ECG buffer.
                this->addEcgSample(ecgSample);

                // If buffer is full, add timestamp and send samples.
                if (this->bufferedEcgSamples == this->objectSize)
                {
                    // Compute timestamp.
                    uint32_t t = timestamp - ((numberOfSamples - i - 1) * this->measurementInterval);
                    // Set timestamp to timestamp of last sample in buffer.
                    this->setTimestamp(t);
                    // Send samples.
                    this->sendEcgBuffer();
                }
            }
            break;
        }
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID:
        {
            // Get the GATT characteristic which is handled now.
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
            auto charHandle = parameterRef.getCharHandle();

            // Set the current GATT characteristic.
            if (charHandle == this->mMeasurementIntervalCharHandle)
            {
                // Set Measurement Interval GATT Characteristic:

                // Parse received Measurement Interval.
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic&>();
                uint16_t interval = *reinterpret_cast<const uint16_t*>(&charValue.bytes[0]);

                DEBUGLOG("onNotify: MeasurementInterval: len: %d, new interval: %d", charValue.bytes.size(), interval);

                // Update the Measurement Interval.
                this->setMeasurementInterval(interval);
            }
            else if (charHandle == this->mObjectSizeCharHandle)
            {
                // Set Object Size GATT Characteristic:

                // Parse received Object Size.
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic&>();
                uint16_t size = *reinterpret_cast<const uint16_t*>(&charValue.bytes[0]);

                DEBUGLOG("onNotify: objectSize: len: %d, new interval: %d", charValue.bytes.size(), size);

                // Update the Object Size.
                this->setObjectSize(size);
            }
            break;
        }
    }
}

void EcgGATTSvcClient::configGattSvc()
{
    // Define ECG GATT Service and its Characteristics.
    WB_RES::GattSvc ecgGattSvc;
    WB_RES::GattChar characteristics[3];
    WB_RES::GattChar &ecgVoltageChar = characteristics[0];
    WB_RES::GattChar &measurementIntervalChar = characteristics[1];
    WB_RES::GattChar &objectSizeChar = characteristics[2];

    // Specify Characteristics's properties.
    WB_RES::GattProperty ecgVoltageCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty measurementIntervalCharProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };
    WB_RES::GattProperty objectSizeCharProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };

    // Specify ECG Characteristic.
    ecgVoltageChar.props = wb::MakeArray<WB_RES::GattProperty>(&ecgVoltageCharProp, 1);
    ecgVoltageChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ecgVoltageCharUUID16), sizeof(uint16_t));

    // Specify Interval Characteristic.
    measurementIntervalChar.props = wb::MakeArray<WB_RES::GattProperty>(measurementIntervalCharProps, 2);
    measurementIntervalChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&measurementIntervalCharUUID16), sizeof(uint16_t));
    measurementIntervalChar.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->measurementInterval), sizeof(uint16_t));

    // Specify Size Characteristic.
    objectSizeChar.props = wb::MakeArray<WB_RES::GattProperty>(objectSizeCharProps, 2);
    objectSizeChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&objectSizeCharUUID16), sizeof(uint16_t));
    objectSizeChar.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->objectSize), sizeof(uint16_t));

    // Combine Characteristics to Service.
    ecgGattSvc.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&ecgSvcUUID16), sizeof(uint16_t));
    ecgGattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 3);

    // Create custom GATT Service.
    this->asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, ecgGattSvc);
}

size_t EcgGATTSvcClient::getSingleEcgBufferSize()
{
    return sizeof(uint32_t) + (sizeof(ecg_val) * this->objectSize);
}

uint8_t* EcgGATTSvcClient::getEcgBuffer(size_t i)
{
    size_t size = this->getSingleEcgBufferSize();
    return &this->ecgCharacteristicsBuffers[i * size];
}

uint8_t* EcgGATTSvcClient::getCurrentEcgBuffer()
{
    return this->getEcgBuffer(this->ecgBufferIndex);
}

void EcgGATTSvcClient::switchMessageBuffer()
{
    size_t size = this->getSingleEcgBufferSize();
    this->ecgBufferIndex = (this->ecgBufferIndex + 1) % size;
    this->bufferedEcgSamples = 0;
}

bool EcgGATTSvcClient::clearEcgBuffer(size_t i)
{
    // Ensure that ECG buffer is initialized.
    if (this->ecgCharacteristicsBuffers == nullptr)
    {
        return false;
    }

    // Override all buffers with zero-bytes.
    size_t size = this->getSingleEcgBufferSize();
    size_t bufferIndex = i * size;
    for (size_t j = 0; j < size; j++)
    {
        this->ecgCharacteristicsBuffers[bufferIndex + j] = 0;
    }

    // Reset buffered ECG samples counter.
    this->bufferedEcgSamples = 0;

    return true;
}

bool EcgGATTSvcClient::clearCurrentEcgBuffer()
{
    return this->clearEcgBuffer(this->ecgBufferIndex);
}

ecg_val EcgGATTSvcClient::convertEcgSample(int32 ecgValue)
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
        return (ecg_val)ecgValue;
    }
}

bool EcgGATTSvcClient::addEcgSample(ecg_val sample)
{
    // Cancel if ECG buffer is not initialized.
    if (this->ecgCharacteristicsBuffers == nullptr)
    {
        return false;
    }

    // Get the current ECG buffer.
    uint8_t* ecgBuffer = this->getCurrentEcgBuffer();

    // Compute target index in ECG buffer.
    size_t index = sizeof(uint32_t) + (this->bufferedEcgSamples * sizeof(ecg_val));
    // Write ECG sample to buffer.
    ecg_val* sampleAddress = &sample;
    uint8_t* sampleData = (uint8_t*)sampleAddress;
    ecgBuffer[index] = sampleData[0];
    ecgBuffer[index + 1] = sampleData[1];
    // Increase number of buffered ECG samples.
    this->bufferedEcgSamples++;
    return true;
}

bool EcgGATTSvcClient::setTimestamp(uint32_t timestamp)
{
    // Cancel if ECG buffer is not initialized.
    if (this->ecgCharacteristicsBuffers == nullptr)
    {
        return false;
    }

    // Get the current ECG buffer.
    uint8_t* ecgBuffer = this->getCurrentEcgBuffer();

    // Write timestamp to the start of the ECG buffer.
    uint32_t* timestampAddress = &timestamp;
    uint8_t* timestampData = (uint8_t*)timestampAddress;
    ecgBuffer[0] = timestampData[0];
    ecgBuffer[1] = timestampData[1];
    ecgBuffer[2] = timestampData[2];
    ecgBuffer[3] = timestampData[3];
    return true;
}

bool EcgGATTSvcClient::sendEcgBuffer()
{
    // Cancel if message buffers not initialized.
    if (this->ecgCharacteristicsBuffers == nullptr)
    {
        return false;
    }

    // Get the current buffer and its size.
    uint8_t* ecgBuffer = this->getCurrentEcgBuffer();
    size_t size = this->getSingleEcgBufferSize();

    // Move to next message buffer.
    this->switchMessageBuffer();

    // Generate ECG Voltage Characteristics value to send.
    WB_RES::Characteristic ecgVoltageCharacteristic;
    ecgVoltageCharacteristic.bytes = wb::MakeArray<uint8_t>(ecgBuffer, size);

    // Send ECG Voltage characteristics value.
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        mEcgSvcHandle,
        mEcgVoltageCharHandle,
        ecgVoltageCharacteristic
    );
    return true;
}

uint32_t EcgGATTSvcClient::getSampleRate()
{
    return (uint32_t)(1000 / this->measurementInterval);
}

void EcgGATTSvcClient::setMeasurementInterval(uint16_t value)
{
    // Ensure that value is valid or fall back to `DEFAULT_MEASUREMENT_INTERVAL`.
    switch (value)
    {
        case 1: // 1000 Hz
        case 2: // 500 Hz
        case 4: // 250 Hz
        case 8: // 125 Hz
            break;
        default:
            value = DEFAULT_MEASUREMENT_INTERVAL;
            break;
    }

    // Unsubscribe from current ECG subscription.
    this->unsubscribeFromEcgSamples();
    // Update measurement interval.
    this->measurementInterval = value;
    // Set measurement interval to GATT Characteristics value.
    WB_RES::Characteristic measurementIntervalChar;
    measurementIntervalChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->measurementInterval, sizeof(uint16_t));
    asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mEcgSvcHandle,
        mMeasurementIntervalCharHandle,
        measurementIntervalChar
    );
    // Reset current ECG buffer and start over.
    this->clearCurrentEcgBuffer();
    // Subscribe to new ECG subscription.
    this->subscribeToEcgSamples();
}

void EcgGATTSvcClient::subscribeToEcgSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Subscribe to ECG samples with the desired ECG sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void EcgGATTSvcClient::unsubscribeFromEcgSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Unsubscribe from ECG samples with desired ECG sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void EcgGATTSvcClient::setObjectSize(uint16_t value)
{
    // Delete message buffers if exists.
    if (this->ecgCharacteristicsBuffers != nullptr)
    {
        delete[] this->ecgCharacteristicsBuffers;
    }
    // Set new object size.
    this->objectSize = value;
    // Set object size to GATT Characteristics value.
    if (this->mObjectSizeCharHandle != 0) {
        WB_RES::Characteristic objectSizeChar;
        objectSizeChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->objectSize, sizeof(uint16_t));
        asyncPut(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
            AsyncRequestOptions::Empty,
            this->mEcgSvcHandle,
            this->mObjectSizeCharHandle,
            objectSizeChar
        );
    }
    // Compute size of one single message buffer.
    size_t singleBufferSize = this->getSingleEcgBufferSize();
    // Allocate new message buffers.
    this->ecgCharacteristicsBuffers = new uint8_t[numberOfMessageBuffers * singleBufferSize] { 0 };
    // Reset number of buffered samples.
    this->bufferedEcgSamples = 0;
    this->ecgBufferIndex = 0;
}
