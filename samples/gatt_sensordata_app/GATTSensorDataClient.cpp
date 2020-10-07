#include "movesense.h"

#include "GATTSensorDataClient.h"
#include "common/core/debug.h"
#include "oswrapper/thread.h"

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"
#include "meas_temp/resources.h"

// Functions for serializing binary data
#include "meas_acc/resources.h"
#include "meas_gyro/resources.h"
#include "meas_magn/resources.h"
#include "meas_imu/resources.h"
#include "meas_ecg/resources.h"
#include "meas_hr/resources.h"
#include "movesense_time/resources.h"
#include "sbem-code/sbem_definitions.h"

const char* const GATTSensorDataClient::LAUNCHABLE_NAME = "GattData";
// UUID: 34802252-7185-4d5d-b431-630e7050e8f0
constexpr uint8_t SENSOR_DATASERVICE_UUID[] = { 0xf0, 0xe8, 0x50, 0x70, 0x0e, 0x63, 0x31, 0xb4, 0x5d, 0x4d, 0x85, 0x71, 0x52, 0x22, 0x80, 0x34 };
constexpr uint8_t COMMAND_CHAR_UUID[] = { 0xf0, 0xe8, 0x50, 0x70, 0x0e, 0x63, 0x31, 0xb4, 0x5d, 0x4d, 0x85, 0x71, 0x01, 0x00, 0x80, 0x34 };
constexpr uint16_t commandCharUUID16 = 0x0001;
constexpr uint8_t DATA_CHAR_UUID[] = { 0xf0, 0xe8, 0x50, 0x70, 0x0e, 0x63, 0x31, 0xb4, 0x5d, 0x4d, 0x85, 0x71, 0x02, 0x00, 0x80, 0x34 };
constexpr uint16_t dataCharUUID16 = 0x0002;

GATTSensorDataClient::GATTSensorDataClient():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mCommandCharResource(wb::ID_INVALID_RESOURCE),
    mDataCharResource(wb::ID_INVALID_RESOURCE),
    mNotificationsEnabled(false),
    mSensorSvcHandle(0),
    mCommandCharHandle(0),
    mDataCharHandle(0)
{
}

GATTSensorDataClient::~GATTSensorDataClient()
{
}

bool GATTSensorDataClient::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void GATTSensorDataClient::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool GATTSensorDataClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Clear subscription table
    for (size_t i=0; i<MAX_DATASUB_COUNT; i++)
    {
        mDataSubs[i].clientReference = 0;
        mDataSubs[i].resourceId = wb::ID_INVALID_RESOURCE;
        mDataSubs[i].subStarted = false;
        mDataSubs[i].subCompleted = false;
    }

    // Follow BLE connection status
    asyncSubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    // Configure custom gatt service
    configGattSvc();

    return true;
}

void GATTSensorDataClient::stopModule()
{
    // Unsubscribe sensor data
    unsubscribeAllStreams();

    // Clean up GATT stuff
    asyncUnsubscribe(mCommandCharResource);
    asyncUnsubscribe(mDataCharResource);
    
    releaseResource(mCommandCharResource);
    releaseResource(mDataCharResource);
    
    mCommandCharResource = wb::ID_INVALID_RESOURCE;
    mDataCharResource = wb::ID_INVALID_RESOURCE;

    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void GATTSensorDataClient::configGattSvc()
{
    WB_RES::GattSvc customGattSvc;
    WB_RES::GattChar characteristics[2];
    WB_RES::GattChar &commandChar = characteristics[0];
    WB_RES::GattChar &dataChar = characteristics[1];

    // Define the CMD characteristics
    WB_RES::GattProperty dataCharProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty commandCharProp = WB_RES::GattProperty::WRITE;

    dataChar.props = wb::MakeArray<WB_RES::GattProperty>( &dataCharProp, 1);
    dataChar.uuid = wb::MakeArray<uint8_t>( reinterpret_cast<const uint8_t*>(&DATA_CHAR_UUID), sizeof(DATA_CHAR_UUID));

    commandChar.props = wb::MakeArray<WB_RES::GattProperty>( &commandCharProp, 1);
    commandChar.uuid = wb::MakeArray<uint8_t>( reinterpret_cast<const uint8_t*>(&COMMAND_CHAR_UUID), sizeof(COMMAND_CHAR_UUID));

    // Combine chars to service
    customGattSvc.uuid = wb::MakeArray<uint8_t>( SENSOR_DATASERVICE_UUID, sizeof(SENSOR_DATASERVICE_UUID));
    customGattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 2);

    // Create custom service
    asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions(NULL,0,true), customGattSvc);
}

// Simple command structure:
// - command [1 byte]
// - client reference [1 byte, not zero!]
// - Command specific data
//
// Result and data notifications are returned via dataCharacteristic in format
// - result type [1 byte]: (1= response to command, )2: data notification from subscription
// - client reference [1 byte]
// - data: (2 byte "HTTP result" for commands, sbem formatted binary for subscriptions)

// Command reference:
// SUBSCRIBE (=1)
//   data == WB Resource path as string
//
// UNSUBSCRIBE (=2)
//   no data
//   reference must match one given in SUBSCRIBE command
enum Commands 
{
    HELLO = 0,
    SUBSCRIBE = 1,
    UNSUBSCRIBE = 2,
};
enum Responses 
{
    COMMAND_RESULT = 1,
    DATA = 2,
};

GATTSensorDataClient::DataSub* GATTSensorDataClient::findDataSub(const wb::ResourceId resourceId)
{
    for (size_t i=0; i<MAX_DATASUB_COUNT; i++)
    {
        const DataSub &ds= mDataSubs[i];
        if (ds.resourceId == resourceId)
            return &(mDataSubs[i]);
    }
    return nullptr;
}
GATTSensorDataClient::DataSub*  GATTSensorDataClient::findDataSubByRef(const uint8_t clientReference)
{
    for (size_t i=0; i<MAX_DATASUB_COUNT; i++)
    {
        const DataSub &ds= mDataSubs[i];
        if (ds.clientReference == clientReference)
            return &(mDataSubs[i]);
    }
    return nullptr;
}

GATTSensorDataClient::DataSub* GATTSensorDataClient::getFreeDataSubSlot()
{
    for (size_t i=0; i<MAX_DATASUB_COUNT; i++)
    {
        const DataSub &ds= mDataSubs[i];
        if (ds.clientReference == 0 && ds.resourceId == wb::ID_INVALID_RESOURCE)
            return &(mDataSubs[i]);
    }
    return nullptr;
}

void GATTSensorDataClient::handleIncomingCommand(const wb::Array<uint8> &commandData)
{
    uint8_t cmd = commandData[0];
    uint8_t reference = commandData[1];
    const uint8_t *pData = commandData.size()>2 ? &(commandData[2]) : nullptr;
    uint16_t dataLen = commandData.size() - 2;

    switch (cmd)
    {
        case Commands::HELLO:
        {
            // Hello response
            uint8_t helloMsg[] = {COMMAND_RESULT, reference, 'H','e','l','l','o'};

            WB_RES::Characteristic dataCharValue;
            dataCharValue.bytes = wb::MakeArray<uint8_t>(helloMsg, sizeof(helloMsg));
            asyncPut(mDataCharResource, AsyncRequestOptions(NULL, 0, true), dataCharValue);
            return;
        }
        case Commands::SUBSCRIBE:
        {
            DataSub *pDataSub = getFreeDataSubSlot();

            if (!pDataSub)
            {
                DEBUGLOG("No free datasub slot");
                // 507: HTTP_CODE_INSUFFICIENT_STORAGE
                uint8_t errorMsg[] = {COMMAND_RESULT, reference, 0x01,0xFB};
                
                WB_RES::Characteristic dataCharValue;
                dataCharValue.bytes = wb::MakeArray<uint8_t>(errorMsg, sizeof(errorMsg));
                asyncPut(mDataCharResource, AsyncRequestOptions(NULL, 0, true), dataCharValue);
                return;
            }

            // Store client reference to array and trigger subsribe
            DataSub &dataSub = *pDataSub;

            char pathBuffer[160]; // Big enough sincer MTU is 161
            memset(pathBuffer, 0, sizeof(pathBuffer));

            // Copy and null-terminate
            memcpy(pathBuffer, pData, dataLen);

            dataSub.subStarted = true;
            dataSub.subCompleted = false;
            dataSub.clientReference = reference;
            getResource(pathBuffer, dataSub.resourceId);

            asyncSubscribe(dataSub.resourceId, AsyncRequestOptions::ForceAsync);
        }
        break;
        case Commands::UNSUBSCRIBE:
        {
            DEBUGLOG("Commands::UNSUBSCRIBE. reference: %d", reference);

            // Store client reference to array and trigger subsribe
            DataSub *pDataSub = findDataSubByRef(reference);
            if (pDataSub != nullptr)
            {
                asyncUnsubscribe(pDataSub->resourceId);
                pDataSub->resourceId = wb::ID_INVALID_RESOURCE;
                pDataSub->clientReference = 0;
            }
        }
        break;
    }
}


void GATTSensorDataClient::onGetResult(wb::RequestId requestId,
                                      wb::ResourceId resourceId,
                                      wb::Result resultCode,
                                      const wb::Value& rResultData)
{
    DEBUGLOG("GATTSensorDataClient::onGetResult");
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID:
        {
            // This code finalizes the service setup (triggered by code in onPostResult)
            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc &>();
            for (size_t i=0; i<svc.chars.size(); i++)
            {
                // Find out characteristic handles and store them for later use
                const WB_RES::GattChar &c = svc.chars[i];
                // Extract 16 bit sub-uuid from full 128bit uuid
                DEBUGLOG("c.uuid.size(): %u", c.uuid.size());
                uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[12]));
                
                DEBUGLOG("char[%u] uuid16: 0x%04X", i, uuid16);

                if(uuid16 == dataCharUUID16)
                    mDataCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                else if(uuid16 == commandCharUUID16)
                    mCommandCharHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
            }

            if (!mCommandCharHandle || !mDataCharHandle)
            {
                DEBUGLOG("ERROR: Not all chars were configured!");
                return;
            }

            char pathBuffer[32]= {'\0'};
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", mSensorSvcHandle, mCommandCharHandle);
            getResource(pathBuffer, mCommandCharResource);
            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", mSensorSvcHandle, mDataCharHandle);
            getResource(pathBuffer, mDataCharResource);

            // Forse subscriptions asynchronously to save stack (will have stack overflow if not) 
            // Subscribe to listen to intervalChar notifications (someone writes new value to intervalChar) 
            asyncSubscribe(mCommandCharResource, AsyncRequestOptions(NULL, 0, true));
            // Subscribe to listen to measChar notifications (someone enables/disables the INDICATE characteristic) 
            asyncSubscribe(mDataCharResource,  AsyncRequestOptions(NULL, 0, true));
            break;
        }
    }
}

/** @see whiteboard::ResourceClient::onGetResult */
void  GATTSensorDataClient::onSubscribeResult(wb::RequestId requestId,
                                              wb::ResourceId resourceId,
                                              wb::Result resultCode,
                                              const wb::Value& rResultData)
{
    DEBUGLOG("onSubscribeResult() resourceId: %u", resourceId);

    switch (resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_PEERS::LID:
            {
                DEBUGLOG("OnSubscribeResult: WB_RES::LOCAL::COMM_BLE_PEERS: %d", resultCode);
                return;
            }
            break;
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID:
            {
                DEBUGLOG("OnSubscribeResult: COMM_BLE_GATTSVC*: %d", resultCode);
                return;
            }
            break;
        default:
        {
            // All other notifications. These must be the client subscribed data streams
            GATTSensorDataClient::DataSub *ds = findDataSub(resourceId);
            if (ds == nullptr)
            {
                DEBUGLOG("DataSub not found for resource: %u", resourceId);
                return;
            }
            ASSERT(ds->subStarted);
            if (ds->subCompleted)
            {
                DEBUGLOG("subCompleted already: %u", resourceId);
                return;
            }

            if (resultCode >= 400)
            {
                ds->clientReference = 0;
                ds->resourceId = wb::ID_INVALID_RESOURCE;
                ds->subStarted=false;
                ds->subCompleted=false;
            }
            else
            {
                ds->subCompleted=true;
            }
        }
        break;
    }
}

void GATTSensorDataClient::unsubscribeAllStreams()
{
    for (size_t i=0;i<MAX_DATASUB_COUNT; i++)
    {
        if (mDataSubs[i].resourceId != wb::ID_INVALID_RESOURCE)
        {
            asyncUnsubscribe(mDataSubs[i].resourceId);
            mDataSubs[i].clientReference = 0;
            mDataSubs[i].resourceId = wb::ID_INVALID_RESOURCE;
            mDataSubs[i].subStarted = false;
            mDataSubs[i].subCompleted = false;
        }
    }
}

void GATTSensorDataClient::onNotify(wb::ResourceId resourceId,
                                   const wb::Value& value,
                                   const wb::ParameterList& rParameters)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_PEERS::LID:
            {
                WB_RES::PeerChange peerChange = value.convertTo<WB_RES::PeerChange>();
                if (peerChange.state = peerChange.state.DISCONNECTED)
                {
                    // if connection is dropped, unsubscribe all data streams so that sensor does not stay on for no reason
                    unsubscribeAllStreams();
                }
            }

            break;
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID:
        {
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
            if (parameterRef.getCharHandle() == mCommandCharHandle) 
            {
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic &>();

                DEBUGLOG("onNotify: mCommandCharHandle: len: %d", charValue.bytes.size());

                handleIncomingCommand(charValue.bytes);
                return;
            }
            else if (parameterRef.getCharHandle() == mDataCharHandle) 
            {
                const WB_RES::Characteristic &charValue = value.convertTo<const WB_RES::Characteristic &>();
                // Update the notification state so we know if to forward data to datapipe
                mNotificationsEnabled = charValue.notifications.hasValue() ? charValue.notifications.getValue() : false;
                DEBUGLOG("onNotify: mDataCharHandle. mNotificationsEnabled: %d", mNotificationsEnabled);
            }
            break;
        }

        default:
        {
            // All other notifications. These must be the client subscribed data streams
            GATTSensorDataClient::DataSub *ds = findDataSub(resourceId);
            if (ds == nullptr)
            {
                DEBUGLOG("DataSub not found for resource: %u", resourceId);
                return;
            }
        
            DEBUGLOG("DS clientReference: %u", ds->clientReference);
            DEBUGLOG("DS subStarted: %u", ds->subStarted);
            DEBUGLOG("DS subCompleted: %u", ds->subCompleted);

            // Make sure we can serialize the data
            size_t length = getSbemLength(resourceId.localResourceId, value);
            if (length == 0)
            {
                DEBUGLOG("No length for localResourceId: %u", resourceId.localResourceId);
                return;
            }

            // Forward data to client
            memset(mDataMsgBuffer, 0, sizeof(mDataMsgBuffer));
            mDataMsgBuffer[0] = DATA;
            mDataMsgBuffer[1] = ds->clientReference;

            length = writeToSbemBuffer(&mDataMsgBuffer[2], sizeof(mDataMsgBuffer)-2, 0, resourceId.localResourceId, value);

            WB_RES::Characteristic dataCharValue;
            dataCharValue.bytes = wb::MakeArray<uint8_t>(mDataMsgBuffer, length+2);
            asyncPut(mDataCharResource, AsyncRequestOptions(NULL, 0, true), dataCharValue);
            return;

            break;
        }

    }
}

void GATTSensorDataClient::onPostResult(wb::RequestId requestId, 
                                       wb::ResourceId resourceId, 
                                       wb::Result resultCode, 
                                       const wb::Value& rResultData)
{
    DEBUGLOG("GATTSensorDataClient::onPostResult: %d", resultCode);

    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // Custom Gatt service was created
        mSensorSvcHandle = (int32_t)rResultData.convertTo<uint16_t>();
        DEBUGLOG("Custom Gatt service was created. handle: %d", mSensorSvcHandle);
        
        // Request more info about created svc so we get the char handles
        asyncGet(WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(), AsyncRequestOptions(NULL,0,true), mSensorSvcHandle);
        // Note: The rest of the init is performed in onGetResult()
    }
}