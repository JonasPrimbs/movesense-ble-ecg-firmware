#include "BleConformanceTesterService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "component_nrf52/resources.h"
#include "device_system_shutdown/resources.h"

#define TEST_START_TIMEOUT_MS 1000

const char* const BleConformanceTesterService::LAUNCHABLE_NAME = "BleConformanceTest";

static const wb::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::BLECONFORMANCE::EXECUTION_CONTEXT;

static const wb::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::BLECONFORMANCE::LID,
};

BleConformanceTesterService::BleConformanceTesterService():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
    mStartTestTimer(wb::ID_INVALID_TIMER),
    mBleTestParams{0u, 0u, 0u, wb::Optional<uint8>(0u), 0u, 0u, 0u}
{
}

BleConformanceTesterService::~BleConformanceTesterService()
{
}

bool BleConformanceTesterService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void BleConformanceTesterService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool BleConformanceTesterService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void BleConformanceTesterService::stopModule()
{
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void BleConformanceTesterService::onPutRequest(const wb::Request& request,
                                               const wb::ParameterList& parameters)
{
    DEBUGLOG("BleConformanceTesterService::onPutRequest() called.");

    if (mModuleState != WB_RES::ModuleStateValues::STARTED)
    {
        returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
        return;
    }

    switch (request.getResourceId().localResourceId)
    {
    case WB_RES::LOCAL::BLECONFORMANCE::LID:
    {
        DEBUGLOG("BLECONFORMANCE");

        auto bleTestParams = WB_RES::LOCAL::BLECONFORMANCE::PUT::ParameterListRef(parameters).getTest();
        DEBUGLOG("bleTestParams. pow: %d, cmd: %d, chn: %d, len: %d, payload: %d, timeout: %d",
                 bleTestParams.powerLvl,
                 bleTestParams.cmd,
                 bleTestParams.chn,
                 bleTestParams.len,
                 bleTestParams.payload,
                 bleTestParams.timeout);

        // Validate test parameters
        if (bleTestParams.powerLvl > 11)
        {
            DEBUGLOG("Invalid powerLvl, returning BAD_REQUEST.");
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            return;
        }
        if (bleTestParams.cmd < 1 || bleTestParams.cmd > 2)
        {
            DEBUGLOG("Invalid cmd, returning BAD_REQUEST.");
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            return;
        }
        if (bleTestParams.chn > 39)
        {
            DEBUGLOG("Invalid chn, returning BAD_REQUEST.");
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            return;
        }
        if (bleTestParams.len > 37)
        {
            DEBUGLOG("Invalid len, returning BAD_REQUEST.");
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            return;
        }
        if (!(bleTestParams.payload == 0 || bleTestParams.payload == 1 || bleTestParams.payload == 2 || bleTestParams.payload == 0xffffffff))
        {
            DEBUGLOG("Invalid payload, returning BAD_REQUEST.");
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            return;
        }
        if (bleTestParams.timeout == 0 && bleTestParams.chn_end.hasValue())
        {
            DEBUGLOG("Infinite timeout cannot have chn_end.");
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            return;
        }

        mBleTestParams = bleTestParams;

        // Everything ok, so trigger start of test timer.
        // This gives time to return result before test start switches off BLE
        DEBUGLOG("starting test timer...");
        mStartTestTimer = ResourceProvider::startTimer(TEST_START_TIMEOUT_MS, false);

        returnResult(request, wb::HTTP_CODE_OK);
        break;
    }

    default:
        ASSERT(0); // it would be a system error if we got here some other resource than the one that is registered.
    }
}

void BleConformanceTesterService::onTimer(wb::TimerId timerId)
{
    DEBUGLOG("BleConformanceTesterService::onTimer");

    if (timerId == mStartTestTimer)
    {
        mStartTestTimer = wb::ID_INVALID_TIMER;
        DEBUGLOG("timerId == mStartTestTimer");

        DEBUGLOG("BleConformanceTesterService::StartTest");
        DEBUGLOG("- pow: %d, cmd: %d, chn: %d, len: %d, payload: 0x%x, timeout: %d",
                 mBleTestParams.powerLvl,
                 mBleTestParams.cmd,
                 mBleTestParams.chn,
                 mBleTestParams.len,
                 mBleTestParams.payload,
                 mBleTestParams.timeout);

        // Set tx power level in BleTestService
        WB_RES::TxPow txPow;
        txPow.level = mBleTestParams.powerLvl;
        asyncPut(WB_RES::LOCAL::COMPONENT_NRF52_TEST_TXPOW(), AsyncRequestOptions(NULL, 0, true), txPow);

        // Set test mode in BleTestService
        WB_RES::TestMode testMode;
        testMode.cmd = mBleTestParams.cmd;
        testMode.chn = mBleTestParams.chn;
        testMode.chn_end = mBleTestParams.chn_end;
        testMode.len = mBleTestParams.len;
        testMode.payload = mBleTestParams.payload;
        testMode.timeout = mBleTestParams.timeout;

        asyncPut(WB_RES::LOCAL::COMPONENT_NRF52_TEST_MODE(), AsyncRequestOptions(NULL, 0, true), testMode);
        DEBUGLOG("Now in test mode. will restart in %u seconds.", mBleTestParams.timeout);
    }
}

void BleConformanceTesterService::onPutResult(wb::RequestId requestId,
                                              wb::ResourceId resourceId,
                                              wb::Result resultCode,
                                              const wb::Value& resultData)
{
    DEBUGLOG("BleConformanceTesterService::onPutResult");
    if (resourceId.localResourceId == WB_RES::LOCAL::COMPONENT_NRF52_TEST_MODE::LID)
    {
        DEBUGLOG("NRF52_TEST_MODE returned. rebooting...");
        // Time to end the test, boot the device
        asyncPut(WB_RES::LOCAL::DEVICE_SYSTEM_SHUTDOWN(), AsyncRequestOptions(NULL, 0, true));
    }
}
