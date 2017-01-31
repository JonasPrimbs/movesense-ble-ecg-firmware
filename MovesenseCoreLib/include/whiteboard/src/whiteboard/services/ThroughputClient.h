// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceClient.h"
#include "whiteboard/DpcFunctor.h"

namespace whiteboard
{
namespace services
{

enum ThroughputTestType
{
    TEST_ZERO,
    TEST_NULL,
    TEST_ECHO
};

/** Callback for throughput command
*
* @param currRequestNum Request number of currently processed request.
* @param totalNumberOfRequests A number of total request in current test.
* @param result Result code of the operation
* @param elapsedTimeUs Execution time of the request.
* @param bytesTransferred Number of bytes transferred in the request.
* @param totalElapsedTimeUs Total execution time of the test.
* @param totalBytesTransferred Total number of bytes transferred during test.
* @param dataValid True if the data matches in echo test.
* @param testType Type of the test
* @return A value indicating whether the test should continue
*/
typedef bool (*ThroughputCallback)(int32 currRequestNum,
                                   int32 totalNumberOfRequests,
                                   Result result,
                                   uint32 elapsedTimeUs,
                                   uint32 bytesTransferred,
                                   uint32 totalElapsedTimeUs,
                                   uint32 totalBytesTransferred,
                                   bool dataValid,
                                   ThroughputTestType testType);

/** Throughput service client */
class WB_API ThroughputClient : private ResourceClient
{
public:
    /**	Initializes a new instance of ping client
     *
     * @param callback Callback function for result reporting
     * @param executionContextId Execution context ID for the client
     */
    ThroughputClient(ThroughputCallback callback, ExecutionContextId executionContextId);

    /** Throughput test client destructor */
    ~ThroughputClient();

    /**	Reads number of zeros from given destination provider.
    *
    * @param uri Throughput provider resource URI for /svc/zero.
    * @param numberOfRequests Number of requests to be processed.
    * @param numberOfSimultaneousRequests Number of simultaneously executed requests
    * @return A value indicating whether the operation was started
    */
    bool readFromZero(const char* uri, int32 numberOfRequests, int32 numberOfSimultaneousRequests);

    /**	Writes number of bytes to given destination provider.
    *
    * @param uri Throughput provider resource URI for /svc/null
    * @param numberOfRequests Number of requests to be processed.
    * @param numberOfSimultaneousRequests Number of simultaneously executed requests
    * @return A value indicating whether the operation was started
    */
    bool writeToNull(const char* uri, int32 numberOfRequests, int32 numberOfSimultaneousRequests);

    /**	Writes number of bytes to given destination provider which relays them back.
    *	Test data is randomized in client for each request and thus affects performance.
    *
    * @param uri Throughput provider resource URI for /svc/echo
    * @param numberOfRequests Number of requests to be processed.
    * @param numberOfSimultaneousRequests Number of simultaneously executed requests
    * @return A value indicating whether the operation was started
    */
    bool writeToEcho(const char* uri, int32 numberOfRequests, int32 numberOfSimultaneousRequests);

private:
    /**	Starts test with given parameters.
    *
    * @param uri Throughput provider resource URI for /svc/echo
    * @param numberOfRequests Number of requests to be processed.
    * @param numberOfSimultaneousRequests Number of simultaneously executed requests
    * @param testType Type of the test
    * @return A value indicating whether the operation was started
    */
    bool startTest(const char* uri, int32 numberOfRequests, int32 numberOfSimultaneousRequests, ThroughputTestType testType);

    /** Performs tests
     *
     * @param requestNumber Id of request to execute
     */
    void doTest(int32 requestNumber);

    /** Helper for executing zero get request */
    void doZero();

    /** Helper for executing null put request */
    void doNull();

    /** Helper for executing echo get request */
    void doEcho();

    /// @see whiteboard::ResourceClient::onGetResourceResult
    void onGetResourceResult(RequestId requestId, ResourceId resourceId, Result resultCode) OVERRIDE;

    /// @see whiteboard::ResourceClient::onGetResult
    void onGetResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData) OVERRIDE;

    /// @see whiteboard::ResourceClient::onPutResult
    void onPutResult(RequestId requestId, ResourceId resourceId, Result resultCode, const Value& rResultData) OVERRIDE;

    /**
    *	Function to validate echoed data.
    *
    *	@param rResultData Successful result contains the request result
    */
    bool isValidEchoData(const Value& rResultData);

    /**
    *	Function to create random data for payload.
    */
    void randomizeListOfBytesData();

    void requestTrampoline();
private:
    /** Type of current throughput test */
    ThroughputTestType mTestType;

    /** Request timeout in milliseconds */
    static const size_t REQUEST_TIMEOUT_MS = 1000;

    /** Throughput callback */
    ThroughputCallback mCallback;

    /** ID of Throughput resource */
    ResourceId mThroughputResource;

    /** Total number of transfer requests in test. */
    int32 mNumberOfRequests;

    /** Number of simultaneously executed transfer requests. */
    int32 mNumberOfSimultaneousRequests;

    /** Current request number*/
    int32 mCurrentRequestNumber;

    // TODO Issuing several requests at start is not working.
    static const int32 MAX_START_REQUEST_COUNT = 1;

    /** Start time of throughtput test */
    WbTime mTestStart;

    /** Start time or request */
    WbTime mRequestStart[MAX_START_REQUEST_COUNT];

    /** Buffer allocated for put requests */
    uint8* mData;

    /** DPC to use for getting the right resource for further requests */
    DpcFunctor mResourceDpc;

    /** URI for getting the right resource for further requests */
    char* mResourceUri;
};

} // namespace services
} // namespace whiteboard
