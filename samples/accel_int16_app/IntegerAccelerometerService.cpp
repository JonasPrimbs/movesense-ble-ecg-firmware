// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "IntegerAccelerometerService.h"
#include "app-resources/resources.h"
#include "common/core/debug.h"
#include "meas_acc/resources.h"
#include "common/text/Text.hpp"

const char* const IntegerAccelerometerService::LAUNCHABLE_NAME = "IntAcc";
static const wb::ExecutionContextId sExecutionContextId = WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::EXECUTION_CONTEXT;
static const wb::LocalResourceId sProviderResources[] =
{
    WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::LID,
};

// This resource provider will "forward" /sample/intacc/{samplerate} subscriptions to /meas/acc/{samplerate}.
// The sample values are reduced from 32 bit floats specified for /meas/acc to 16 bit integers specified for /sample/intacc.

// Basis of operation:
// 1. Client's subscription request to /sample/intacc/{samplerate} is received in onSubscribe.
// 2. A forwarding subscription to /meas/acc/{samplerate} is made with the same sample rate.
// 3. The response of the forward subscription is passed over as the response to the client's original subscription.
// 4. Each notify event for /meas/acc/{samplerate} is forwarded to /sample/intacc/{samplerate} subscribers.
//      Consequently notifications to e.g. /meas/acc/52 and /meas/acc/416 will be forwarded to subscribers
//      of /sample/intacc/52 and /sample/intacc/416 correspondingly.

IntegerAccelerometerService::IntegerAccelerometerService():
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
    LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId),
    mFwdTracker(*this, *this),
    mOngoingRequests(),
    mScaler(1000.0f) // initialise with a dummy scaler, will be assigned according to config upon use
{
}

IntegerAccelerometerService::~IntegerAccelerometerService()
{
}

bool IntegerAccelerometerService::initModule()
{
    if (registerProviderResources(sProviderResources) != wb::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void IntegerAccelerometerService::deinitModule()
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool IntegerAccelerometerService::startModule()
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;
    return true;
}

void IntegerAccelerometerService::stopModule()
{
    mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

// This callback is called when the acceleration resource notifies us of new data
void IntegerAccelerometerService::onNotify(wb::ResourceId resourceId,
                                           const wb::Value& value,
                                           const wb::ParameterList& parameters)
{
    const WB_RES::AccData& linearAccelerationData = value.convertTo<const WB_RES::AccData&>();
    const wb::FloatVector3D* itemArray = linearAccelerationData.arrayAcc.getItems();

    WB_RES::Int16Vector3D vectors[8]; // for now /meas/acc provides max 8 samples at a time (may vary according to subscribed sample rates)
    size_t numItems = linearAccelerationData.arrayAcc.getNumberOfItems();

    // just for future safety, if more than 8 samples was received the rest will be discarded
    numItems = (numItems > ELEMENTS(vectors)) ? ELEMENTS(vectors) : numItems;

    for (size_t i = 0; i < numItems; i++)
    {
        // scale with the config based range scaler
        wb::FloatVector3D tmp = itemArray[i] * mScaler;

        // clamp to int16 range before casting
        static const float int16Max = static_cast<float>(INT16_MAX);
        static const float int16Min = static_cast<float>(INT16_MIN);
        tmp.mX = (tmp.mX > int16Max) ? int16Max : ((tmp.mX < int16Min) ? int16Min : tmp.mX);
        tmp.mY = (tmp.mY > int16Max) ? int16Max : ((tmp.mY < int16Min) ? int16Min : tmp.mY);
        tmp.mZ = (tmp.mZ > int16Max) ? int16Max : ((tmp.mZ < int16Min) ? int16Min : tmp.mZ);

        vectors[i] = { static_cast<int16>(tmp.mX), static_cast<int16>(tmp.mY), static_cast<int16>(tmp.mZ) };
    }

    // instantiate the specified resource structure
    WB_RES::IntAccData data16bit =
    {
        linearAccelerationData.timestamp,
        wb::MakeArray<WB_RES::Int16Vector3D>(vectors, numItems)
    };

    // send the intacc notifications
    const WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::EVENT::ParameterListRef pRef(parameters);
    if (pRef.hasSampleRate())
    {
        // this will send a notification to subscribers of the specific sample rate
        wb::Result r = updateResource(WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE(), ResponseOptions::Empty, data16bit, pRef.getSampleRate());
        if (!wb::RETURN_OK(r))
        {
            // if updates exceed e.g. comm output bandwidth we get errors here
            DEBUGLOG("update ERR %u", r);
        }
    }
    else
    {
        // this will send a notification to subscribers of any/all sample rates
        // at the moment this kind of notifications are not emitted for /meas/acc/{samplerate}
        updateResource(WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE(), ResponseOptions::Empty, data16bit);
    }
}

void IntegerAccelerometerService::onGetResult(wb::RequestId requestId,
                                              wb::ResourceId resourceId,
                                              wb::Result resultCode,
                                              const wb::Value& rResultData)
{
    const WB_RES::AccConfig& config = rResultData.convertTo<const WB_RES::AccConfig&>();
    // g is 9.832 m/s^2 at poles, 9.780 m/a^2 at equator, average that
    static const float g = (9.832f + 9.780f) * 0.5f;
    // get scaler to utilise the whole int16 range for the selected g-range
    mScaler = static_cast<float>(INT16_MAX) / (g * static_cast<float>(config.gRange));

    DEBUGLOG("%s range=%u scaler*1000=%u", __FUNCTION__, config.gRange, static_cast<uint32>(1000.f * mScaler));
}

void IntegerAccelerometerService::onSubscribeResult(wb::RequestId requestId,
                                                    wb::ResourceId resourceId,
                                                    wb::Result resultCode,
                                                    const wb::Value& rResultData)
{
    DEBUGLOG("%s called. rscID: %u, resCode: %u", __FUNCTION__, resourceId, resultCode);

    wb::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);
    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, resultCode);
    }
}

// NOTE: all response codes that may be returned by this method must be specified in yaml under subscription post responses.
void IntegerAccelerometerService::onSubscribe(const wb::Request& request,
                                              const wb::ParameterList& parameters)
{
    wb::ResourceId rscId = request.getResourceId();
    DEBUGLOG("%s %u %u %u", __FUNCTION__, rscId, rscId.isPathParameterRef(), rscId.instanceId);
    switch (rscId.localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::LID:
    {
        const int32_t samplerate = WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::SUBSCRIBE::ParameterListRef(parameters).getSampleRate();
        // Someone subscribed to our service. If no previous subscribers to the specific sample rate we need to make a new forward subscription
        // NOTE: unique resource ID for each sample rate due to path parameter instance ID, mimics separate resource for each sample rate
        if (wb::HTTP_CODE_NO_CONTENT == mFwdTracker.isResourceSubscribed(rscId, samplerate))
        {
            DEBUGLOG(" + samplerate: %u", samplerate);
            // Because /meas/acc and our new /sample/intacc are both in the MEAS execution context and
            // AsyncRequestOptions::ForceAsync is not used, this GET request will be executed synchronously by WB and the
            // above onGetResult callback will be called before returning from the asyncGet(...) method.
            // Otherwise we would need postpone the /meas/acc subscription below and do it e.g. in the onGetResult method.
            asyncGet(WB_RES::LOCAL::MEAS_ACC_CONFIG(), AsyncRequestOptions::Empty);

            // Subscribe to LinearAcceleration resource (notifications received at requested sample rate),
            // when subscribe is done we get async callback to onSubscribeResult

            ResourceForwardingTracker::ResourceIdMapEntry* mapped = mFwdTracker.allocateMapEntry(rscId, samplerate);
            if (NULL == mapped)
            {
                DEBUGLOG("allocateMapEntry error"); // map is full
                returnResult(request, wb::HTTP_CODE_LOCKED); // pass on the error result
                return; // we were unable to process the request, bail out, officially client should retry
            }

            nea::Text<32> path;
            path.format("meas/acc/%u", static_cast<uint32>(samplerate)); // generate our forward resource path
            wb::Result result = getResource(path.c_str(), mapped->outResource); // get the forward reseource ID
            if (false == wb::RETURN_OK(result))
            {
                DEBUGLOG("getResource error: %u", result);
                mFwdTracker.freeMapEntry(mapped);
                returnResult(request, result); // pass on the error result
                return; // we were unable to process the request, bail out, officially client should retry
            }
            DEBUGLOG("GOT: %s = %u", path.c_str(), mapped->outResource);

            wb::RequestId remoteRequestId;
            result = asyncSubscribe(mapped->outResource, AsyncRequestOptions(&remoteRequestId, 0, true)); // forceAsync = true
            if (false == wb::RETURN_OK(result))
            {
                // async operations usually can only fail if the request message queue is full (too many requests or someone is blocking)
                DEBUGLOG("asyncSubscribe error: %u", result);
                releaseResource(mapped->outResource); // release resource to free the path parameter allocation
                mFwdTracker.freeMapEntry(mapped);
                returnResult(request, result); // pass on the error result
                return; // we were unable to process the request, bail out, officially client should retry
            }

            bool queueResult = mOngoingRequests.put(remoteRequestId, request);
            ASSERT(queueResult); // all is lost if we cannot track the request since the request was already sent

            // all went ok, returnResult call will be made in onSubscribeResult callback.
        }
        else
        {
            // the forward subscription was already present by another client so return result immediately
            returnResult(request, wb::HTTP_CODE_OK);
        }
        break;
    }
    default:
        ASSERT(0); // Should not happen
    }
}

void IntegerAccelerometerService::onUnsubscribeResult(wb::RequestId requestId,
                                                      wb::ResourceId resourceId,
                                                      wb::Result resultCode,
                                                      const wb::Value& rResultData)
{
    DEBUGLOG("%s called. rscID: %u, resCode: %u", __FUNCTION__, resourceId, resultCode);

    if (wb::HTTP_CODE_OK == resultCode)
    {
        releaseResource(resourceId); // release resource to free the path parameter allocation
    }

    wb::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);
    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, resultCode);
    }
}

// NOTE: all response codes that may be returned by this method must be specified in yaml under subscription delete responses.
void IntegerAccelerometerService::onUnsubscribe(const wb::Request& request,
                                                const wb::ParameterList& parameters)
{
    wb::ResourceId rscId = request.getResourceId();
    DEBUGLOG("%s %u %u %u", __FUNCTION__, rscId, rscId.isPathParameterRef(), rscId.instanceId);

    switch (rscId.localResourceId)
    {
    case WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::LID:
    {
        const int32_t samplerate = WB_RES::LOCAL::SAMPLE_INTACC_SAMPLERATE::SUBSCRIBE::ParameterListRef(parameters).getSampleRate();
        // Someone unsubscribed the service. If no more subscribers to the specific sample rate we need to unsubscribe the forwarded resource
        // NOTE: unique resource ID for each sample rate due to path parameter instance ID, mimics separate resource for each sample rate
        if (wb::HTTP_CODE_NO_CONTENT == mFwdTracker.isResourceSubscribed(rscId, samplerate))
        {
            DEBUGLOG(" - samplerate: %u", samplerate);

            wb::ResourceId outResource = mFwdTracker.freeMapEntry(rscId, samplerate); // drops back out the outResource for unsubscribing
            if (wb::ID_INVALID_RESOURCE == outResource)
            {
                // apparently the client was not subscribed, subscription may be lost e.g. upon comm connection loss
                returnResult(request, wb::HTTP_CODE_NOT_FOUND);
                return; // nothing more to do, bail out
            }

            wb::RequestId remoteRequestId;
            wb::Result result = asyncUnsubscribe(outResource, AsyncRequestOptions(&remoteRequestId, 0, true)); // forceAsync = true
            if (false == wb::RETURN_OK(result))
            {
                // async operations usually can only fail if the request message queue is full (too many requests or someone is blocking)
                DEBUGLOG("asyncUnsubscribe error: %u", result);
                returnResult(request, result); // pass on the error result
                return; // we were unable to process the request, bail out, officially client should retry
            }

            bool queueResult = mOngoingRequests.put(remoteRequestId, request);
            ASSERT(queueResult); // all is lost if we cannot track the request since the request was already sent

            // all went ok, returnResult call will be made in onSubscribeResult callback.
        }
        break;
    }
    default:
        ASSERT(0); // Should not happen
    }
}

void IntegerAccelerometerService::onRemoteWhiteboardDisconnected(wb::WhiteboardId whiteboardId)
{
    DEBUGLOG("%s called.", __FUNCTION__);

    // upon connection loss remote clients are expected to resubscribe to the resources
    // reset all forwarded subscriptions made by remote clients
    mFwdTracker.clearDisconnected();
}

void IntegerAccelerometerService::onClientUnavailable(wb::ClientId clientId)
{
    DEBUGLOG("%s called.", __FUNCTION__);
}
