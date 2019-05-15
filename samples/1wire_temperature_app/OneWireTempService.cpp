#include "movesense.h"

#include "OneWireTempService.h"
#include "common/core/debug.h"

#include "DebugLogger.hpp"
#include "comm_1wire/resources.h"
#include "system_states/resources.h"
#include "ui_ind/resources.h"

const char* const OneWireTempService::LAUNCHABLE_NAME = "1WTemp";

static const wb::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_1WIRETEMP::LID,
};

/***********************************************************************
 * OneWire thermometer sample app: This sample demonstrates the use of 1-Wire bus via Movesense API
 * What it does:
 *  - for each GET to /Sample/1WireTemp:
 *    - activate 1Wire bus
 *    - Scan the 1Wire bus for devices
 *    - For each device with THERMOMETER_FAMILY_CODE:
 *      - start temperature measurement and read the value
 ***********************************************************************/

OneWireTempService::OneWireTempService():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
    mPowerOnTimer(wb::ID_INVALID_TIMER),
    mTimer(wb::ID_INVALID_TIMER),
    mGetRequestQueue(),
    mCurrentCmd(NO_COMMAND),
    mDeviceCount(0),
    mNextIdxToMeasure(0),
    mMeasurements{0},
    mDeviceHandles{0}
{
}

OneWireTempService::~OneWireTempService()
{
}

bool OneWireTempService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
    {
        return false;
    }
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void OneWireTempService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool OneWireTempService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void OneWireTempService::stopModule()
{
    wb::ResourceProvider::stopTimer(mPowerOnTimer);
    wb::ResourceProvider::stopTimer(mTimer);
    mPowerOnTimer = wb::ID_INVALID_TIMER;
    mTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void OneWireTempService::readTemperatureFromSensor(size_t idx)
{
    DebugLogger::info("readTemperatureFromSensor() idx: %u", idx);
    const uint8_t readTempCmd[] = {0x33, 0x08, 3}; // Read, DATA_FIFO, 3 bytes (+ 2 crc)
    WB_RES::OWCommand readTemperatureResultCmd;
    readTemperatureResultCmd.dataOut = wb::MakeArray<uint8_t>(readTempCmd, sizeof(readTempCmd));
    readTemperatureResultCmd.readCount = 5;
    mCurrentCmd = READ_TEMP;
    asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
             mDeviceHandles[idx], readTemperatureResultCmd);
}

void OneWireTempService::readFifoCountFromSensor(size_t idx)
{
    DebugLogger::info("readFifoCountFromSensor() idx: %u", idx);
    const uint8_t readTempCmd[] = {0x33, 0x07, 1}; // Read, FIFO_COUNT, 3 bytes (+ 2 crc)
    WB_RES::OWCommand readTemperatureResultCmd;
    readTemperatureResultCmd.dataOut = wb::MakeArray<uint8_t>(readTempCmd, sizeof(readTempCmd));
    readTemperatureResultCmd.readCount = 3;
    mCurrentCmd = READ_FIFO_COUNT;
    asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
             mDeviceHandles[idx], readTemperatureResultCmd);
}

void OneWireTempService::startTemperatureConversion(size_t idx)
{
    DebugLogger::info("startTemperatureConversion() idx: %u", idx);
    const uint8_t readTempCmd[1] = {0x44};
    WB_RES::OWCommand tempConversionCmd;
    tempConversionCmd.dataOut = wb::MakeArray<uint8_t>(readTempCmd, sizeof(readTempCmd));
    tempConversionCmd.readCount = 2; // crc-16
    mCurrentCmd = CONVERT_T;
    asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
             mDeviceHandles[idx], tempConversionCmd);
}

void OneWireTempService::onTimer(wb::TimerId timerId)
{
    if (timerId == mPowerOnTimer)
    {
        mPowerOnTimer = wb::ID_INVALID_TIMER;
        // Start measurement by running PEERS-scan
        asyncGet(WB_RES::LOCAL::COMM_1WIRE_PEERS(), AsyncRequestOptions::Empty);
        return;
    }

    // Read previous measurement result (if available)
    if (mNextIdxToMeasure > 0)
    {
        readFifoCountFromSensor(mNextIdxToMeasure - 1);
    }
    else
    {
        // If nothing to read, start conversion here (later in onPutResult after previous device is read)
        startTemperatureConversion(mNextIdxToMeasure);
    }
}

void OneWireTempService::onGetResult(wb::RequestId requestId,
                                     wb::ResourceId resourceId,
                                     wb::Result resultCode,
                                     const wb::Value& result)
{
    if (wb::IsErrorResult(resultCode))
    {
        DEBUGLOG("onGetResult returned error! return empty measurement list");
        while (!mGetRequestQueue.isEmpty())
        {
            WB_RES::ExtTempMeasurements tempResult;
            tempResult.measurements = wb::MakeArray<WB_RES::TempMeasurement>(mMeasurements, 0);
            wb::Request request;
            mGetRequestQueue.pop(request);
            returnResult(request, wb::HTTP_CODE_OK, wb::ResourceProvider::ResponseOptions::Empty, tempResult);
        }
        return;
    }

    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::COMM_1WIRE_PEERS::LID:
    {
        // Result of scan available.
        const WB_RES::OWPeerList& peerList = result.convertTo<WB_RES::OWPeerList&>();
        DEBUGLOG("COMM_1WIRE_PEERS. size: %u", peerList.connectedPeers.size());

        // clear device array
        for (size_t i = 0; i < MAX_THERMOMETERS; i++)
        {
            mMeasurements[i] = {0, 0.0f};
            mDeviceHandles[i] = 0;
        }
        mDeviceCount = 0;

        // For all devices matching thermometer, start operation
        size_t idx = 0;
        for (size_t i = 0; i < peerList.connectedPeers.size(); i++)
        {
            uint64_t peer_addr = peerList.connectedPeers[i].address;
            // Check family code
            constexpr uint8_t THERMOMETER_FAMILY_CODE = 0x54;
            uint8_t familyCode = (peer_addr & 0xFF);

            DEBUGLOG("peer_addr: %08X", (uint32_t)(peer_addr & 0xffffffff));
            DEBUGLOG("handle hasValue: %u", peerList.connectedPeers[i].handle.hasValue());
            DEBUGLOG("familyCode: %u", familyCode);

            if (familyCode == THERMOMETER_FAMILY_CODE && peerList.connectedPeers[i].handle.hasValue())
            {
                int32_t handle = peerList.connectedPeers[i].handle.getValue();
                DebugLogger::info("Thermometer found. handle: %d", handle);

                // Add device to list of devices
                mDeviceHandles[idx] = handle;
                mMeasurements[idx++].deviceId = peer_addr;
                mDeviceCount++;
            }
        }
        // Start measurement timer if matches found
        if (idx > 0)
        {
            mNextIdxToMeasure = 0;
            mTimer = wb::ResourceProvider::startTimer(1, false);
        }
        else
        {
            WB_RES::ExtTempMeasurements tempResult;
            tempResult.measurements = wb::MakeArray<WB_RES::TempMeasurement>(mMeasurements, 0);
            while (!mGetRequestQueue.isEmpty())
            {
                wb::Request request;
                mGetRequestQueue.pop(request);
                returnResult(request, wb::HTTP_CODE_OK, wb::ResourceProvider::ResponseOptions::Empty, tempResult);
            }

            // DeActivate 1wire bus
            asyncUnsubscribe(WB_RES::LOCAL::COMM_1WIRE(), AsyncRequestOptions::Empty);
        }
    }
    break;
    }
}

void OneWireTempService::onPutResult(wb::RequestId requestId,
                                     wb::ResourceId resourceId,
                                     wb::Result resultCode,
                                     const wb::Value& result)
{
    if (wb::IsErrorResult(resultCode))
    {
        DebugLogger::error("onPutResult failed! resource: %u, result: %u", resourceId.localResourceId, resultCode);
        return;
    }

    switch (resourceId.localResourceId)
    {
    case WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE::LID:
    {
        DebugLogger::info("Put result: COMM_1WIRE_PEERS_CONNHANDLE");
        const WB_RES::OWCommandResult& cmdResult = result.convertTo<WB_RES::OWCommandResult&>();

        for (size_t i = 0; i < cmdResult.data.size(); i++)
        {
            DEBUGLOG(" #%u: %02x", i, cmdResult.data[i]);
        }

        // If read temp command, store result and start conversion
        if (mCurrentCmd == READ_FIFO_COUNT)
        {
            DEBUGLOG("READ_FIFO_COUNT: %u", cmdResult.data[0]);
            readTemperatureFromSensor(mNextIdxToMeasure - 1);
        }
        else if (mCurrentCmd == READ_TEMP)
        {
            // T = DIGITAL OUTPUT (in decimal) * 0.005
            union
            {
                int16_t i16;
                uint8_t b[2];
            } temp;
            temp.b[0] = cmdResult.data[1];
            temp.b[1] = cmdResult.data[0];
            int32_t tempC = temp.i16;
            tempC *= 5;
            DEBUGLOG("temp: %d => %d.%03u C", temp.i16, tempC / 1000, tempC % 1000);
            float fTempC = 0.005f * temp.i16;
            mMeasurements[mNextIdxToMeasure - 1].temperature = fTempC;
            DEBUGLOG("Next device handle: %d", mDeviceHandles[mNextIdxToMeasure]);
            if (mDeviceHandles[mNextIdxToMeasure] != 0)
            {
                // Start conversion for next device
                startTemperatureConversion(mNextIdxToMeasure);
            }
            else
            {
                DEBUGLOG("No more devices, return measurements to GET caller");
                WB_RES::ExtTempMeasurements tempResult;
                tempResult.measurements = wb::MakeArray<WB_RES::TempMeasurement>(mMeasurements, mDeviceCount);
                wb::Request request;
                while (!mGetRequestQueue.isEmpty())
                {
                    mGetRequestQueue.pop(request);
                    returnResult(request, wb::HTTP_CODE_OK, wb::ResourceProvider::ResponseOptions::Empty, tempResult);
                }

                // DeActivate 1wire bus
                asyncUnsubscribe(WB_RES::LOCAL::COMM_1WIRE(), AsyncRequestOptions::Empty);
            }
        }
        else
        {
            // was Convert T command
            // Wait for conversion to finish before starting next measurement (timer)
            mNextIdxToMeasure++;
            mTimer = wb::ResourceProvider::startTimer(16, false);
        }
        break;
    }
    }
}

void OneWireTempService::onSubscribeResult(wb::RequestId requestId,
                                           wb::ResourceId resourceId,
                                           wb::Result resultCode,
                                           const wb::Value& rResultData)
{
    DEBUGLOG("OneWireTempService::onSubscribeResult() called.");
}

void OneWireTempService::onGetRequest(const wb::Request& request,
                                      const wb::ParameterList& parameters)
{
    DEBUGLOG("OneWireTempService::onGetRequest() called.");

    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
    {
        returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
        return;
    }

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_1WIRETEMP::LID:
    {
        if (!mGetRequestQueue.push(request))
        {
            // the request was ok but we coulnd't store it for tracking
            returnResult(request, wb::HTTP_CODE_TOO_MANY_REQUESTS);
            return;
        }

        // Activate 1wire bus
        asyncSubscribe(WB_RES::LOCAL::COMM_1WIRE(), AsyncRequestOptions::Empty);

        // And wait until we can start measurement
        mPowerOnTimer = ResourceProvider::startTimer(50, false);
        break;
    }

    default:
        ASSERT(0); // would be a system error if we reached this, trust the system and safe rom
    }
}