#include "movesense.h"

#include "CustomGATTSvcClient.h"
#include "common/core/debug.h"
#include "oswrapper/thread.h"

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"
#include "meas_temp/resources.h"

const char* const CustomGATTSvcClient::LAUNCHABLE_NAME = "CstGattS";

const uint16_t measCharUUID16 = 0x2A1C;
const uint16_t intervalCharUUID16 = 0x2A21;

const int DEFAULT_MEASUREMENT_INTERVAL_SECS = 3;

CustomGATTSvcClient::CustomGATTSvcClient():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mIntervalCharResource(wb::ID_INVALID_RESOURCE),
    mMeasCharResource(wb::ID_INVALID_RESOURCE),
    mMeasurementTimer(wb::ID_INVALID_TIMER),
    mMeasIntervalSecs(DEFAULT_MEASUREMENT_INTERVAL_SECS),
    mTemperatureSvcHandle(0),
    mMeasCharHandle(0),
    mIntervalCharHandle(0)
{
}

CustomGATTSvcClient::~CustomGATTSvcClient()
{
}

bool CustomGATTSvcClient::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void CustomGATTSvcClient::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool CustomGATTSvcClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Configure custom gatt service
    configGattSvc();
    return true;
}

void CustomGATTSvcClient::stopModule()
{
    // Stop timer
    stopTimer(mMeasurementTimer);
    mMeasurementTimer = wb::ID_INVALID_TIMER;

    asyncUnsubscribe(mIntervalCharResource);
    asyncUnsubscribe(mMeasCharResource);
    mIntervalCharResource = wb::ID_INVALID_RESOURCE;
    mMeasCharResource = wb::ID_INVALID_RESOURCE;

    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void CustomGATTSvcClient::configGattSvc()
{
    WB_RES::GattSvc customGattSvc;
    WB_RES::GattChar characteristics[2];
    WB_RES::GattChar &measChar = characteristics[0];
    WB_RES::GattChar &intervalChar = characteristics[1];

    const uint16_t healthThermometerSvcUUID16 = 0x1809;
    
    // Define the CMD characteristics
    WB_RES::GattProperty measCharProp = WB_RES::GattProperty::INDICATE;
    WB_RES::GattProperty intervalCharProps[2] = {WB_RES::GattProperty::READ, WB_RES::GattProperty::WRITE};

    measChar.props = wb::MakeArray<WB_RES::GattProperty>( &measCharProp, 1);
    measChar.uuid = wb::MakeArray<uint8_t>( reinterpret_cast<const uint8_t*>(&measCharUUID16), 2);

    intervalChar.props = wb::MakeArray<WB_RES::GattProperty>( intervalCharProps, 2);
    intervalChar.uuid = wb::MakeArray<uint8_t>( reinterpret_cast<const uint8_t*>(&intervalCharUUID16), 2);
    intervalChar.initial_value = wb::MakeArray<uint8_t>( reinterpret_cast<const uint8_t*>(&mMeasIntervalSecs), 2);

    // Combine chars to service
    customGattSvc.uuid = wb::MakeArray<uint8_t>( reinterpret_cast<const uint8_t*>(&healthThermometerSvcUUID16), 2);
    customGattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 2);

    // Create custom service
    asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, customGattSvc);
}

void CustomGATTSvcClient::onTimer(wb::TimerId timerId)
{
    DEBUGLOG("CustomGATTSvcClient::onTimer");

    // Take temperature reading
    asyncGet(WB_RES::LOCAL::MEAS_TEMP(), NULL);
}

#include <math.h>
static void floatToFLOAT(float value, uint8_t* bufferOut)
{
    bool bNegative = (value < 0.0f);
    if (bNegative) value = fabs(value);

    // Calc exponent
    int exponent = ceil(log10(value));
    DEBUGLOG("exponent: %d", exponent);

    float mantissa = value * pow(10.0, -exponent + 6); // Use up to 10^6 in mantissa (+-1000,000)
    int32_t mantInt24 = (int32_t)round(mantissa);
    if (bNegative)
        mantInt24 = -mantInt24;
    DEBUGLOG("mantInt24: %d", mantInt24);

    bufferOut[0] = (uint8_t)(mantInt24 & 0xff);
    bufferOut[1] = (uint8_t)((mantInt24>>8) & 0xff);
    bufferOut[2] = (uint8_t)((mantInt24>>16) & 0xff);
    bufferOut[3] = (int8_t)exponent-6;
}

void CustomGATTSvcClient::onGetResult(wb::RequestId requestId,
                                      wb::ResourceId resourceId,
                                      wb::Result resultCode,
                                      const wb::Value& rResultData)
{
    DEBUGLOG("CustomGATTSvcClient::onGetResult");
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID:
        {
            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc &>();
            for (size_t i=0; i<svc.chars.size(); i++)
            {
                const WB_RES::GattChar &c = svc.chars[i];
                uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[0]));
                
                if(uuid16 == measCharUUID16)
                    mMeasCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                else if(uuid16 == intervalCharUUID16)
                    mIntervalCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
            }

            if (!mIntervalCharHandle || !mMeasCharHandle)
            {
                DEBUGLOG("ERROR: Not all chars were configured!");
                return;
            }

            char pathBuffer[32]= {'\0'};
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", mTemperatureSvcHandle, mIntervalCharHandle);
            getResource(pathBuffer, mIntervalCharResource);
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", mTemperatureSvcHandle, mMeasCharHandle);
            getResource(pathBuffer, mMeasCharResource);

            // Forse subscriptions asynchronously to save stack (will have stack overflow if not) 
            // Subscribe to listen to intervalChar notifications (someone writes new value to intervalChar) 
            asyncSubscribe(mIntervalCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to measChar notifications (someone enables/disables the INDICATE characteristic) 
            asyncSubscribe(mMeasCharResource,  AsyncRequestOptions(NULL, 0, true));
            break;
        }

        case WB_RES::LOCAL::MEAS_TEMP::LID:
        {
            // Temperature result or error
            if (resultCode == wb::HTTP_CODE_OK)
            {
                WB_RES::TemperatureValue value = rResultData.convertTo<WB_RES::TemperatureValue>();
                float temperature = value.measurement;

                // Convert K to C
                temperature -= 273.15f;

                // Return data
                uint8_t buffer[5]; // 1 byte or flags, 4 for FLOAT "in Celsius" value
                buffer[0]=0;

                // convert normal float to IEEE-11073 "medical" FLOAT type into buffer
                floatToFLOAT(temperature, &buffer[1]);

                // Write the result to measChar. This results INDICATE to be triggered in GATT service
                WB_RES::Characteristic newMeasCharValue;
                newMeasCharValue.bytes = wb::MakeArray<uint8_t>(buffer, sizeof(buffer));
                asyncPut(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(), AsyncRequestOptions::Empty,
                         mTemperatureSvcHandle, mMeasCharHandle, newMeasCharValue);
            }
            break;
        }
    }
}

void CustomGATTSvcClient::onNotify(wb::ResourceId resourceId,
                                   const wb::Value& value,
                                   const wb::ParameterList& rParameters)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID:
        {
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
            if (parameterRef.getCharHandle() == mIntervalCharHandle) 
            {
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic &>();
                uint16_t interval = *reinterpret_cast<const uint16_t*>(&charValue.bytes[0]);
                DEBUGLOG("onNotify: mIntervalCharHandle: len: %d, new interval: %d", charValue.bytes.size(), interval);
                // Update the interval
                if (interval >= 1)
                    mMeasIntervalSecs = interval;
                // restart timer if exists
                if (mMeasurementTimer != wb::ID_INVALID_TIMER)
                {
                    stopTimer(mMeasurementTimer);
                    mMeasurementTimer = startTimer(mMeasIntervalSecs*1000, true);
                }
            }
            else if (parameterRef.getCharHandle() == mMeasCharHandle) 
            {
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic &>();
                bool bNotificationsEnabled = charValue.notifications.hasValue() ? charValue.notifications.getValue() : false;
                DEBUGLOG("onNotify: mMeasCharHandle. bNotificationsEnabled: %d", bNotificationsEnabled);
                // Start or stop the timer
                if (mMeasurementTimer != wb::ID_INVALID_TIMER)
                {
                    stopTimer(mMeasurementTimer);
                    mMeasurementTimer = wb::ID_INVALID_TIMER;
                }
                if (bNotificationsEnabled)
                    mMeasurementTimer = startTimer(mMeasIntervalSecs*1000, true);
            }
            break;
        }
    }
}

void CustomGATTSvcClient::onPostResult(wb::RequestId requestId, 
                                       wb::ResourceId resourceId, 
                                       wb::Result resultCode, 
                                       const wb::Value& rResultData)
{
    DEBUGLOG("CustomGATTSvcClient::onPostResult: %d", resultCode);

    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // Custom Gatt service was created
        mTemperatureSvcHandle = (int32_t)rResultData.convertTo<uint16_t>();
        DEBUGLOG("Custom Gatt service was created. handle: %d", mTemperatureSvcHandle);
        
        // Request more info about created svc so we get the char handles
        asyncGet(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(), AsyncRequestOptions::Empty, mTemperatureSvcHandle);
    }
}