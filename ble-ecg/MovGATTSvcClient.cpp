#include "movesense.h"

#include <meas_acc/resources.h>
#include <meas_gyro/resources.h>
#include <meas_magn/resources.h>

#include "MovGATTSvcClient.h"
#include "common/core/debug.h"
#include "oswrapper/thread.h"
#include "component_led/resources.h"
#include "ui_ind/resources.h"

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"


// Movement GATT Service implementations:

const char* const MovGATTSvcClient::LAUNCHABLE_NAME = "MovGattSvc";

MovGATTSvcClient::MovGATTSvcClient() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mMovSvcHandle(0),
    mMovAccCharHandle(0),
    mMovAccCharResource(wb::ID_INVALID_RESOURCE),
    mMovGyrCharHandle(0),
    mMovGyrCharResource(wb::ID_INVALID_RESOURCE),
    mMovMagCharHandle(0),
    mMovMagCharResource(wb::ID_INVALID_RESOURCE),
    mMeasurementIntervalCharHandle(0),
    mMeasurementIntervalCharResource(wb::ID_INVALID_RESOURCE),
    mObjectSizeCharHandle(0),
    mObjectSizeCharResource(wb::ID_INVALID_RESOURCE),
    measurementInterval(DEFAULT_MOV_MEASUREMENT_INTERVAL),
    objectSize(DEFAULT_MOV_OBJECT_SIZE)
{
    this->accBuffer = new SeriesBuffer<acc_vec_t>(this->objectSize, numberOfMovAccBuffers);
    this->gyrBuffer = new SeriesBuffer<gyr_vec_t>(this->objectSize, numberOfMovGyrBuffers);
    this->magBuffer = new SeriesBuffer<mag_vec_t>(this->objectSize, numberOfMovMagBuffers);
}

MovGATTSvcClient::~MovGATTSvcClient()
{
    delete this->accBuffer;
    delete this->gyrBuffer;
    delete this->magBuffer;
}

bool MovGATTSvcClient::initModule()
{
    DEBUGLOG("MovGATTSvcClient::initModule");

    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void MovGATTSvcClient::deinitModule()
{
    DEBUGLOG("MovGATTSvcClient::deinitModule");

    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool MovGATTSvcClient::startModule()
{
    DEBUGLOG("MovGATTSvcClient::startModule");

    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Set object size and allocate Movement buffers.
    this->setObjectSize(DEFAULT_MOV_OBJECT_SIZE);
    // Set measurement interval to compute Movement sampling frequency.
    this->setMeasurementInterval(DEFAULT_MOV_MEASUREMENT_INTERVAL);
    // Subscribe to Movement samples with computed Movement sampling frequency.
    this->subscribeToAccSamples();
    this->subscribeToGyrSamples();
    this->subscribeToMagSamples();

    // Configure Movement GATT Service.
    this->configGattSvc();

    return true;
}

void MovGATTSvcClient::stopModule()
{
    DEBUGLOG("MovGATTSvcClient::stopModule");

    // Unsubscribe from Movement samples.
    this->unsubscribeFromAccSamples();
    this->mMovAccCharResource = wb::ID_INVALID_RESOURCE;
    this->unsubscribeFromGyrSamples();
    this->mMovGyrCharResource = wb::ID_INVALID_RESOURCE;
    this->unsubscribeFromMagSamples();
    this->mMovMagCharResource = wb::ID_INVALID_RESOURCE;

    // Unsubscribe and clear Measurement Interval GATT characteristics.
    this->asyncUnsubscribe(this->mMeasurementIntervalCharResource);
    this->mMeasurementIntervalCharResource = wb::ID_INVALID_RESOURCE;

    // Unsubscribe and clear Object Size GATT characteristics.
    this->asyncUnsubscribe(this->mObjectSizeCharResource);
    this->mObjectSizeCharResource = wb::ID_INVALID_RESOURCE;

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void MovGATTSvcClient::onGetResult(wb::RequestId requestId,
                                   wb::ResourceId resourceId,
                                   wb::Result resultCode,
                                   const wb::Value& rResultData)
{
    DEBUGLOG("MovGATTSvcClient::onGetResult");

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID:
        {
            DEBUGLOG("MovGATTSvcClient::onGetResult - COMM_BLE_GATTSVC_SVCHANDLE");

            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc&>();
            for (size_t i = 0; i < svc.chars.size(); i++)
            {
                const WB_RES::GattChar &c = svc.chars[i];
                uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[0]));
                
                switch (uuid16)
                {
                    case movAccCharUUID16:
                        this->mMovAccCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case movGyrCharUUID16:
                        this->mMovGyrCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case movMagCharUUID16:
                        this->mMovMagCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case measurementIntervalCharUUID16:
                        this->mMeasurementIntervalCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case objectSizeCharUUID16:
                        this->mObjectSizeCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                }
            }

            // Force subscriptions asynchronously to save stack (will have stack overflow if not) 

            // Subscribe to listen to Movement Acceleration Characteristics notifications (someone enables/disables the NOTIFY characteristic)
            this->asyncSubscribe(this->mMovAccCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Movement Gyroscope Characteristics notifications (someone enables/disables the NOTIFY characteristic)
            this->asyncSubscribe(this->mMovGyrCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Movement Magnetic Field Characteristics notifications (someone enables/disables the NOTIFY characteristic)
            this->asyncSubscribe(this->mMovMagCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Measurement Interval Characteristics notifications (someone writes new value to measurementIntervalChar) 
            this->asyncSubscribe(this->mMeasurementIntervalCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to Object Size Characteristics notifications (someone writes new value to objectSizeChar)
            this->asyncSubscribe(this->mObjectSizeCharResource,  AsyncRequestOptions(NULL, 0, true));
            break;
        }
    }
}

void MovGATTSvcClient::onPostResult(wb::RequestId requestId, 
                                    wb::ResourceId resourceId, 
                                    wb::Result resultCode, 
                                    const wb::Value& rResultData)
{
    DEBUGLOG("MovGATTSvcClient::onPostResult: %d", resultCode);

    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // Movement GATT service was created.
        this->mMovSvcHandle = (int32_t)rResultData.convertTo<uint16_t>();
        DEBUGLOG("MOV GATT service was created. Handle: %d", this->mMovSvcHandle);

        // Request more info about created GATT service so we get the characteristics handles.
        this->asyncGet(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(),
            AsyncRequestOptions::Empty,
            this->mMovSvcHandle
        );
    }
}

void MovGATTSvcClient::onNotify(wb::ResourceId resourceId,
                                const wb::Value& value,
                                const wb::ParameterList& rParameters)
{
    DEBUGLOG("MovGATTSvcClient::onNotify");

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::MEAS_ACC_SAMPLERATE::LID:
        {
            // Get Acceleration Data.
            auto accData = value.convertTo<const WB_RES::AccData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)accData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = accData.arrayAcc.size();//.getNumberOfItems();
            if (numberOfSamples <= 0)
            {
                break;
            }
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert Acceleration sample.
                auto accSample = this->convertAccSample(accData.arrayAcc[i]);
                // Add converted sample to Acceleration buffer.
                this->accBuffer->addSample(accSample);

                // If buffer is full, add timestamp and send samples.
                if (!this->accBuffer->canAddSample())
                {
                    // Compute timestamp.
                    timestamp_t t = timestamp - ((numberOfSamples - i - 1) * this->measurementInterval);
                    // Set timestamp to timestamp of last sample in buffer.
                    this->accBuffer->setTimestamp(t);
                    // Send samples.
                    bool result = this->sendAccBuffer();
                }
            }
            break;
        }
        case WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE::LID:
        {
            // Get Gyroscope Data.
            auto gyrData = value.convertTo<const WB_RES::GyroData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)gyrData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = gyrData.arrayGyro.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert Gyroscope sample.
                auto gyrSample = this->convertAccSample(gyrData.arrayGyro[i]);
                // Add converted sample to Gyroscope buffer.
                this->gyrBuffer->addSample(gyrSample);

                // If buffer is full, add timestamp and send samples.
                if (!this->gyrBuffer->canAddSample())
                {
                    // Compute timestamp.
                    timestamp_t t = timestamp - ((numberOfSamples - i - 1) * this->measurementInterval);
                    // Set timestamp to timestamp of last sample in buffer.
                    this->gyrBuffer->setTimestamp(t);
                    // Send samples.
                    this->sendGyrBuffer();
                }
            }
            break;
        }
        case WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE::LID:
        {
            // Get Magnetic Field Data.
            auto magData = value.convertTo<const WB_RES::MagnData&>();

            // Parse timestamp.
            timestamp_t timestamp = (timestamp_t)magData.timestamp;

            // Parse samples and put them into sample buffer.
            size_t numberOfSamples = magData.arrayMagn.getNumberOfItems();
            for (size_t i = 0; i < numberOfSamples; i++)
            {
                // Convert Magnetic Field sample.
                auto magSample = this->convertMagSample(magData.arrayMagn[i]);
                // Add converted sample to Magnetic Field buffer.
                this->magBuffer->addSample(magSample);

                // If buffer is full, add timestamp and send samples.
                if (!this->magBuffer->canAddSample())
                {
                    // Compute timestamp.
                    timestamp_t t = timestamp - ((numberOfSamples - i - 1) * this->measurementInterval);
                    // Set timestamp to timestamp of last sample in buffer.
                    this->magBuffer->setTimestamp(t);
                    // Send samples.
                    this->sendMagBuffer();
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

void MovGATTSvcClient::configGattSvc()
{
    // Define Movement GATT Service and its Characteristics.
    WB_RES::GattSvc movGattSvc;
    WB_RES::GattChar characteristics[4];
    WB_RES::GattChar &movAccChar = characteristics[0];
    WB_RES::GattChar &movGyrChar = characteristics[1];
    WB_RES::GattChar &movMagChar = characteristics[2];
    WB_RES::GattChar &measurementIntervalChar = characteristics[3];
    // WB_RES::GattChar &objectSizeChar = characteristics[4];

    // Specify Characteristics's properties.
    WB_RES::GattProperty movAccCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty movGyrCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty movMagCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty measurementIntervalCharProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };
    // WB_RES::GattProperty objectSizeCharProps[2] = {
    //     WB_RES::GattProperty::READ,
    //     WB_RES::GattProperty::WRITE
    // };

    // Specify Movement Acceleration Characteristic.
    movAccChar.props = wb::MakeArray<WB_RES::GattProperty>(&movAccCharProp, 1);
    movAccChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&movAccCharUUID16), sizeof(uint16_t));

    // Specify Movement Gyroscope Characteristic.
    movGyrChar.props = wb::MakeArray<WB_RES::GattProperty>(&movGyrCharProp, 1);
    movGyrChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&movGyrCharUUID16), sizeof(uint16_t));

    // Specify Movement Magnetic Field Characteristic.
    movMagChar.props = wb::MakeArray<WB_RES::GattProperty>(&movMagCharProp, 1);
    movMagChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&movMagCharUUID16), sizeof(uint16_t));

    // Specify Interval Characteristic.
    measurementIntervalChar.props = wb::MakeArray<WB_RES::GattProperty>(measurementIntervalCharProps, 2);
    measurementIntervalChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&measurementIntervalCharUUID16), sizeof(uint16_t));
    measurementIntervalChar.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->measurementInterval), sizeof(uint16_t));

    // Specify Size Characteristic.
    // objectSizeChar.props = wb::MakeArray<WB_RES::GattProperty>(objectSizeCharProps, 2);
    // objectSizeChar.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&objectSizeCharUUID16), sizeof(uint16_t));
    // objectSizeChar.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&this->objectSize), sizeof(uint16_t));

    // Combine Characteristics to Service.
    movGattSvc.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&movSvcUUID16), sizeof(uint16_t));
    movGattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 4);

    // Create custom GATT Service.
    this->asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, movGattSvc);
}

acc_vec_t MovGATTSvcClient::convertAccSample(whiteboard::FloatVector3D accVector)
{
    acc_vec_t value;
    value.x = (acc_t)accVector.x;
    value.y = (acc_t)accVector.y;
    value.z = (acc_t)accVector.z;
    return value;
}

bool MovGATTSvcClient::sendAccBuffer()
{
    // Get the current buffer and its size.
    size_t size = this->accBuffer->getSingleBufferSize();

    uint8_t* currentBuffer = this->accBuffer->getCurrentBuffer();

    // Move to next message buffer.
    this->accBuffer->switchBuffer();

    // Generate Movement Acceleration Characteristics value to send.
    WB_RES::Characteristic movAccCharacteristic;
    movAccCharacteristic.bytes = wb::MakeArray<uint8_t>(currentBuffer, size);

    // Send Movement Acceleration characteristics value.
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mMovSvcHandle,
        this->mMovAccCharHandle,
        movAccCharacteristic
    );
    return true;
}

gyr_vec_t MovGATTSvcClient::convertGyrSample(whiteboard::FloatVector3D gyrVector)
{
    gyr_vec_t value;
    value.x = (gyr_t)gyrVector.x;
    value.y = (gyr_t)gyrVector.y;
    value.z = (gyr_t)gyrVector.z;
    return value;
}

bool MovGATTSvcClient::sendGyrBuffer()
{
    // Get the current buffer and its size.
    size_t size = this->gyrBuffer->getSingleBufferSize();

    uint8_t* currentBuffer = this->gyrBuffer->getCurrentBuffer();

    // Move to next message buffer.
    this->gyrBuffer->switchBuffer();

    // Generate Movement Gyroscope Characteristics value to send.
    WB_RES::Characteristic movGyrCharacteristic;
    movGyrCharacteristic.bytes = wb::MakeArray<uint8_t>(currentBuffer, size);

    // Send Movement Gyroscope characteristics value.
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mMovSvcHandle,
        this->mMovGyrCharHandle,
        movGyrCharacteristic
    );
    return true;
}

mag_vec_t MovGATTSvcClient::convertMagSample(whiteboard::FloatVector3D magVector)
{
    mag_vec_t value;
    value.x = (mag_t)magVector.x;
    value.y = (mag_t)magVector.y;
    value.z = (mag_t)magVector.z;
    return value;
}

bool MovGATTSvcClient::sendMagBuffer()
{
    // Get the current buffer and its size.
    size_t size = this->magBuffer->getSingleBufferSize();

    uint8_t* currentBuffer = this->magBuffer->getCurrentBuffer();

    // Move to next message buffer.
    this->magBuffer->switchBuffer();

    // Generate Movement Magnetic Field Characteristics value to send.
    WB_RES::Characteristic movMagCharacteristic;
    movMagCharacteristic.bytes = wb::MakeArray<uint8_t>(currentBuffer, size);

    // Send Movement Acceleration characteristics value.
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mMovSvcHandle,
        this->mMovMagCharHandle,
        movMagCharacteristic
    );
    return true;
}

uint32_t MovGATTSvcClient::getSampleRate()
{
    return this->toSampleRate(this->measurementInterval);
}

uint32_t MovGATTSvcClient::toSampleRate(uint16_t interval)
{
    switch (interval)
    {
        case 5:
            return 208;
        case 10:
            return 104;
        case 20:
            return 52;
        case 40:
            return 26;
        default:
            return 52;
    }
}

void MovGATTSvcClient::setMeasurementInterval(uint16_t value)
{
    // Unsubscribe from current Movement subscriptions.
    this->unsubscribeFromAccSamples();
    this->unsubscribeFromGyrSamples();
    this->unsubscribeFromMagSamples();
    // Update measurement interval.
    this->measurementInterval = value;
    // Set measurement interval to GATT Characteristics value.
    WB_RES::Characteristic measurementIntervalChar;
    measurementIntervalChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->measurementInterval, sizeof(uint16_t));
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->mMovSvcHandle,
        this->mMeasurementIntervalCharHandle,
        measurementIntervalChar
    );
    // Reset current Movement buffers and start over.
    this->accBuffer->resetCurrentBuffer();
    this->gyrBuffer->resetCurrentBuffer();
    this->magBuffer->resetCurrentBuffer();
    // Subscribe to new Movement subscriptions.
    this->subscribeToAccSamples();
    this->subscribeToGyrSamples();
    this->subscribeToMagSamples();
}

void MovGATTSvcClient::subscribeToAccSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Subscribe to Acceleration samples with the desired Acceleration sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_ACC_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void MovGATTSvcClient::unsubscribeFromAccSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Unsubscribe from Acceleration samples with desired Acceleration sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_ACC_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void MovGATTSvcClient::subscribeToGyrSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Subscribe to Gyroscope samples with the desired Gyroscope sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void MovGATTSvcClient::unsubscribeFromGyrSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Unsubscribe from Gyroscope samples with desired Gyroscope sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_GYRO_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void MovGATTSvcClient::subscribeToMagSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Subscribe to Magnetic Field samples with the desired Magnetic Field sample rate.
    this->asyncSubscribe(
        WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void MovGATTSvcClient::unsubscribeFromMagSamples()
{
    // Compute desired sample rate.
    uint32_t sampleRate = this->getSampleRate();
    // Unsubscribe from Magnetic Field samples with desired Magnetic Field sample rate.
    this->asyncUnsubscribe(
        WB_RES::LOCAL::MEAS_MAGN_SAMPLERATE(),
        AsyncRequestOptions::Empty,
        sampleRate
    );
}

void MovGATTSvcClient::setObjectSize(uint16_t value)
{
    // Set new object size.
    this->objectSize = value;
    // Change object size in buffers.
    this->accBuffer->setLength((size_t)value);
    this->gyrBuffer->setLength((size_t)value);
    this->magBuffer->setLength((size_t)value);
    // Set object size to GATT Characteristics value.
    if (this->mObjectSizeCharHandle != 0) {
        WB_RES::Characteristic objectSizeChar;
        objectSizeChar.bytes = wb::MakeArray<uint8_t>((uint8_t*)&this->objectSize, sizeof(uint16_t));
        asyncPut(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
            AsyncRequestOptions::Empty,
            this->mMovSvcHandle,
            this->mObjectSizeCharHandle,
            objectSizeChar
        );
    }
}
