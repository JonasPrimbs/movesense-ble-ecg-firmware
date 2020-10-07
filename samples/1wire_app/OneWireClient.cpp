#include "movesense.h"

#include "OneWireClient.h"
#include "common/core/debug.h"

#include "comm_1wire/resources.h"
#include "ui_ind/resources.h"
#include "system_states/resources.h"
#include "DebugLogger.hpp"

constexpr size_t ONEWIRE_PERIOD_MS = 10000;

// Add +1 for 10ms in specs
constexpr size_t DS24EL15_RELEASE_READ_WAIT_MS = 10+1;

const char* const OneWireClient::LAUNCHABLE_NAME = "1Wire";

/***********************************************************************
 * OneWire sample app: This sample demonstrates the use of 1-Wire bus via Movesense API
 * What it does:
 *  - starts timer that performs operation every 10 seconds
 *  - for each operation:
 *    - Scan the 1Wire bus for devices
 *    - If no Smart connectors found, skip the rest
 *    - For Smart connector, read the ScrathPad memory content
 *  - while "Movesense SmartConnector" is connected the app enables double tap detection
 *    - for double tap the app writes bytes [1,2,3,4] to the smart connectors memory
 ***********************************************************************/

OneWireClient::OneWireClient()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
      LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION),
      m1WireScanAndGetMemOngoing(false),
      mDoubleTapSubscribed(false),
      mSmartConnectorHandle(0),
      mWriteMemState(UNDEFINED),
      mTimer(wb::ID_INVALID_TIMER),
      mAAStatusReadTimer(wb::ID_INVALID_TIMER)
{
}

OneWireClient::~OneWireClient()
{
}

bool OneWireClient::initModule()
{
    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void OneWireClient::deinitModule()
{
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool OneWireClient::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Start 1wire timer. true = trigger repeatedly
    mTimer = startTimer(ONEWIRE_PERIOD_MS, true);

    return true;
}

void OneWireClient::stopModule()
{
    // Stop timers
    stopTimer(mTimer);
    stopTimer(mAAStatusReadTimer);
    mTimer = wb::ID_INVALID_TIMER;
    mAAStatusReadTimer = wb::ID_INVALID_TIMER;
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void OneWireClient::onTimer(wb::TimerId timerId)
{
    if (timerId == mTimer)
    {
        // Skip if we are writing 
        if (mWriteMemState != UNDEFINED)
            return;

        // Blink led 
        asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty,
                 WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION);

        m1WireScanAndGetMemOngoing = true;
        mSmartConnectorHandle = 0;
        // and start 1wire operation with scan. logic continues in onGetResult
        asyncGet(WB_RES::LOCAL::COMM_1WIRE_PEERS(), AsyncRequestOptions(NULL, 0, true));
    }
    else if (timerId == mAAStatusReadTimer)
    {
        mAAStatusReadTimer = wb::ID_INVALID_TIMER;
        WB_RES::OWCommand readReleaseCmd;
        readReleaseCmd.dataOut = wb::MakeArray<uint8_t>(NULL, 0);
        readReleaseCmd.readCount = 1; // 0xAA or error
        readReleaseCmd.contPrev = true;
        asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
                 mSmartConnectorHandle, readReleaseCmd);
    }
}

void OneWireClient::onNotify(wb::ResourceId resourceId,const wb::Value& value,const wb::ParameterList& parameters)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::SYSTEM_STATES_STATEID::LID:
        {
            const WB_RES::StateChange &stateChange = value.convertTo<const WB_RES::StateChange&>();
            if (stateChange.stateId == WB_RES::StateIdValues::DOUBLETAP)
            {
                DebugLogger::info("stateChange DOUBLETAP: newState = %d", stateChange.newState);

                // Only do Write mem if not "Read Mem" ongoing or no smart connector found yet
                if (stateChange.newState != 1 || m1WireScanAndGetMemOngoing ||
                    mSmartConnectorHandle == 0 || mWriteMemState != UNDEFINED)
                    return;

                // Subscribe 1wire to activate bus, then run write mem sequence. continues from onSubscribeResult and then onPutResult's
                mWriteMemState = OneWireActive;
                asyncSubscribe(WB_RES::LOCAL::COMM_1WIRE(),AsyncRequestOptions(NULL, 0, true)); 
            }
            break;
        }
    }
}
void OneWireClient::onSubscribeResult(wb::RequestId requestId,
                                      wb::ResourceId resourceId,
                                      wb::Result resultCode,
                                      const wb::Value& result)
{
    // If error result, just log and do nothing
    if (wb::IsErrorResult(resultCode))
    {
        DebugLogger::error("onSubscribeResult error: resource: %u, resultCode: %u", resourceId.localResourceId,resultCode);
        return;
    }

    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::SYSTEM_STATES_STATEID::LID:
        {
            // Double tab subscribed (no other state in use here)
            mDoubleTapSubscribed = true;
            break;
        }
        case WB_RES::LOCAL::COMM_1WIRE::LID:
        {
            DebugLogger::info("1Wire active");
            mWriteMemState = WriteCmdReadCrc;
            // Send "Write Memory" command to Smart connector device
            WB_RES::OWCommand writeMemCmd;
            const uint8_t cmdBytes[2] = {0x55,0x00};  // Write to page 0, segment 0
            writeMemCmd.dataOut = wb::MakeArray<uint8_t>(cmdBytes, sizeof(cmdBytes));
            writeMemCmd.readCount = 2; // 2 byte CRC 

            asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
                     mSmartConnectorHandle, writeMemCmd);
            // Logic continues in onPutResult
            break;
        }
    }
}

void OneWireClient::onGetResult(wb::RequestId requestId,
                                wb::ResourceId resourceId,
                                wb::Result resultCode,
                                const wb::Value& result)
{
    if (wb::IsErrorResult(resultCode))
    {
        DebugLogger::error("onGetResult failed! resource: %u, result: %u", resourceId.localResourceId, resultCode);
        m1WireScanAndGetMemOngoing = false;
        mSmartConnectorHandle = 0;
        mWriteMemState = UNDEFINED;
        return;
    }
   
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_1WIRE_PEERS::LID:
        {
            if (!wb::RETURN_OKC(resultCode))
            {
                DEBUGLOG("GET COMM_1WIRE_PEERS. error: %u", resultCode);
                return;
            }

            // Result of scan available. 
            const WB_RES::OWPeerList& peerList = result.convertTo<WB_RES::OWPeerList&>();
            DEBUGLOG("COMM_1WIRE_PEERS. size: %u", peerList.connectedPeers.size());

            // For 1st device matching Smart connector family, start operation
            for (size_t i=0;i<peerList.connectedPeers.size(); i++)
            {
                uint64_t peer_addr = peerList.connectedPeers[i].address;
                // Check family code
                constexpr uint8_t SMART_CONNECTOR_FAMILY_CODE = 0x17;
                uint8_t familyCode = (peer_addr & 0xFF);

                DEBUGLOG("peer_addr: %08X", (uint32_t)(peer_addr&0xffffffff));
                DEBUGLOG("handle hasValue: %u", peerList.connectedPeers[i].handle.hasValue());
                DEBUGLOG("familyCode: %u", familyCode);

                if (familyCode == SMART_CONNECTOR_FAMILY_CODE && peerList.connectedPeers[i].handle.hasValue())
                {
                    int32_t handle = peerList.connectedPeers[i].handle.getValue();
                    mSmartConnectorHandle = handle;
                    DebugLogger::info("Smart connector found. handle: %d", handle);

                    if (!mDoubleTapSubscribed)
                    {                                            
                        //Enable double tap detection for writing memory
                        asyncSubscribe(WB_RES::LOCAL::SYSTEM_STATES_STATEID(), AsyncRequestOptions::Empty,
                                       WB_RES::StateIdValues::DOUBLETAP);
                    }

                    // Send "Read Memory" command to Smart connector device
                    WB_RES::OWCommand readMemCmd;
                    const uint8_t cmdBytes[2] = {0xF0,0x00};  // Start from page 0, segment 0 => read whole page 32 bytes
                    readMemCmd.dataOut = wb::MakeArray<uint8_t>(cmdBytes, sizeof(cmdBytes));
                    readMemCmd.readCount = 2 + 32 + 2; // 2 byte CRC + 32 bytes + 2 byte CRC 

                    asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
                             handle, readMemCmd);

                    // Logic continues in onPutResult

                    break; // for loop
                }
            }
            if (mSmartConnectorHandle == 0 && mDoubleTapSubscribed)
            {
                // Disable double tap detection to save battery when no Smart connector is available
                mDoubleTapSubscribed = false;
                asyncUnsubscribe(WB_RES::LOCAL::SYSTEM_STATES_STATEID(), AsyncRequestOptions::Empty,
                                 WB_RES::StateIdValues::DOUBLETAP);
            }
            break;
        }
    }
}


void OneWireClient::onPutResult(wb::RequestId requestId,
                                wb::ResourceId resourceId,
                                wb::Result resultCode,
                                const wb::Value& result)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE::LID:
        {
            if (wb::IsErrorResult(resultCode))
            {
                DebugLogger::error("onPutResult failed! resource: %u, result: %u", resourceId.localResourceId, resultCode);
                m1WireScanAndGetMemOngoing = false;
                mSmartConnectorHandle = 0;
                mWriteMemState = UNDEFINED;
                return;
            }

            // Result of "Read Memory"-command
            if (m1WireScanAndGetMemOngoing)
            {
                m1WireScanAndGetMemOngoing = false;
                // Handle result of the "Read Memory" command
                DebugLogger::info("Put result: COMM_1WIRE_PEERS_CONNHANDLE");

                const WB_RES::OWCommandResult& cmdResult = result.convertTo<WB_RES::OWCommandResult&>();
                char msg[120];
                for (size_t i=0;i<cmdResult.data.size();i++)
                {
                    uint8_t data = cmdResult.data[i];
                    snprintf(msg+i*3, sizeof(msg), "%02X ", data);
                }
                DebugLogger::info("received data: %s", msg);
            }
            else if(mWriteMemState == WriteCmdReadCrc && mSmartConnectorHandle != 0)
            {
                const WB_RES::OWCommandResult &dataRead = result.convertTo<const WB_RES::OWCommandResult&>();
                DebugLogger::info("WriteCmdReadCrc received data (%u bytes): 0x%02X, 0x%02X",
                                    dataRead.data.size(), dataRead.data[0], dataRead.data[1]);

                mWriteMemState = WriteDataReadCrc;
                WB_RES::OWCommand writeDataCmd;
                const uint8_t dataBytes[4] = {1,2,3,4};  // Write to page 0, segment 0
                writeDataCmd.dataOut = wb::MakeArray<uint8_t>(dataBytes, sizeof(dataBytes));
                writeDataCmd.readCount = 2; // 2 byte CRC 
                writeDataCmd.contPrev = true;
                asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
                         mSmartConnectorHandle, writeDataCmd);

            }
            else if(mWriteMemState == WriteDataReadCrc && mSmartConnectorHandle != 0)
            {
                const WB_RES::OWCommandResult &dataRead = result.convertTo<const WB_RES::OWCommandResult&>();
                DebugLogger::info("WriteDataReadCrc received data (%u bytes): 0x%02X, 0x%02X",
                                    dataRead.data.size(), dataRead.data[0], dataRead.data[1]);

                mWriteMemState = WriteAA;
                WB_RES::OWCommand writeReleaseCmd;
                const uint8_t cmdBytes[1] = {0xAA};  // Write AA, wait >10ms and then device responds with 0xAA (or error code)
                writeReleaseCmd.dataOut = wb::MakeArray<uint8_t>(cmdBytes, sizeof(cmdBytes));
                writeReleaseCmd.readCount = 0; // 0xAA
                writeReleaseCmd.contPrev = true;
                asyncPut(WB_RES::LOCAL::COMM_1WIRE_PEERS_CONNHANDLE(), AsyncRequestOptions(NULL, 0, true),
                         mSmartConnectorHandle, writeReleaseCmd);
            }
            else if(mWriteMemState == WriteAA && mSmartConnectorHandle != 0)
            {
                const WB_RES::OWCommandResult &dataRead = result.convertTo<const WB_RES::OWCommandResult&>();
                DebugLogger::info("WriteAA received data (%u bytes)", dataRead.data.size());

                mWriteMemState = ReadAAStatus;
                mAAStatusReadTimer = startTimer(DS24EL15_RELEASE_READ_WAIT_MS, false);
            }
            else if(mWriteMemState == ReadAAStatus && mSmartConnectorHandle != 0)
            {
                const WB_RES::OWCommandResult &dataRead = result.convertTo<const WB_RES::OWCommandResult&>();
                DebugLogger::info("ReadAAStatus received data (%u bytes): 0x%02X", dataRead.data.size(), dataRead.data[0]);

                // Write mem completed
                mWriteMemState = UNDEFINED;

                // Free 1wire bus
                asyncUnsubscribe(WB_RES::LOCAL::COMM_1WIRE());
            }
            break;
        }
    }
}