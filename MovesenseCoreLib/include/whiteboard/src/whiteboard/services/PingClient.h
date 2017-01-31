#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/ResourceClient.h"
#include "whiteboard/DpcFunctor.h"

namespace whiteboard
{
namespace services
{

/** Callback for ping command
*
* @param currentRequestNumber Number of current request (-1 if ping failed before first request)
* @param totalNumberOfRequests Total number of requests
* @param result Result code of the operation
* @param roundtripDelayUs Packet round trip delay in micro seconds
* @param averageRoundtripDelayUs Average round trip delay in micro seconds
* @param jitter Standard deviation of roundtrip delays
* @param averageClockOffsetUs Average clock offset in micro seconds (assuming receive and trasmit paths equally fast)
* @return A value indicating whether the ping should continue
*/
typedef bool (*PingCallback_t)(int32 currentRequestNumber,
                             int32 totalNumberOfRequests,
                             Result result,
                             uint32 roundtripDelayUs,
                             uint32 averageRoundtripDelayUs,
                             double jitter,
                             int32 averageClockOffsetUs);

/** PING service client */
class WB_API PingClient : private ResourceClient
{
public:
    /** Initializes a new instance of ping client
     *
     * @param rCallback Callback function for result reporting
     * @param rExecutionContext Id of execution context for the client
     */
    PingClient(PingCallback_t callback, ExecutionContextId executionContextId);

    /** Destructor */
    ~PingClient();

    /** Pings given destination provider.
    * Note: Current implementation uses sleeps and blocks the execution context it is running on.
    *
    * @param pingUri Ping provider resource URI
    * @param numberOfPackets Number of times to ping
    * @param intervalMs Ping interval in milliseconds
    * @return A value indicating whether the ping operation was started
    */
    bool ping(const char* pingUri, int32 numberOfPackets, size_t intervalMs = 1000);

private:
    /** Prevent use of default constructor */
    PingClient() DELETED;

    /** Prevent use of copy constructor */
    PingClient(const PingClient&) DELETED;

    /** Performs ping */
    void doPing();

    /// @see whiteboard::ResourceClient::onGetResourceResult
    void onGetResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE;

    /// @see whiteboard::ResourceClient::onGetResult
    void onGetResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData) OVERRIDE;

private:
    void pingTrampoline();

    /** Request timeout in milliseconds */
    static const size_t REQUEST_TIMEOUT_MS = 1000;

    /** Ping callback */
    PingCallback_t mCallback;

    /** ID of PING resource */
    ResourceId mPingResource;

    /** Number of times to ping */
    int32 mNumberOfPackets;

    /** Current packet */
    int32 mCurrentPacket;

    /** Number of successful packets */
    int32 mSuccessfulPackets;

    /** Ping interval in milliseconds */
    size_t mIntervalMs;

    /** Total round trip delay */
    uint64 mTotalRoundtripDelay;

    /** Total squared round trip delay for jitter calculation */
    uint64 mTotalSquaredRoundtripDelay;

    /** Total clock offset */
    int64 mTotalClockOffset;

    /** Time of last ping start */
    WbTime mPingStart;

    /** ID of last request */
    RequestId mLastRequestId;

    /** DPC to use for ping() */
    DpcFunctor mPingDpc;

    /** URI for getting the ping resource */
    char* mPingUri;
};

} // namespace services
} // namespace whiteboard
