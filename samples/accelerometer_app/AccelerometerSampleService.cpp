#include "AccelerometerSampleService.h"
#include "common/core/debug.h"
#include "whiteboard/integration/bsp/shared/debug.h"
#include "app-resources/resources.h"
#include "suunto_device_measurement_linearacceleration/resources.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"
#include "whiteboard/builtinTypes/UnknownStructure.h"

#include <float.h>
#include <math.h>

#define ASSERT WB_DEBUG_ASSERT

const char* const AccelerometerSampleService::LAUNCHABLE_NAME = "SampleA";

static const whiteboard::ExecutionContextId sExecutionContextId =
    WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::EXECUTION_CONTEXT;

    
static const whiteboard::LocalResourceId sProviderResources[] = {
    WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::LID,
};


AccelerometerSampleService::AccelerometerSampleService()
    : ResourceClient(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      ResourceProvider(WBDEBUG_NAME(__FUNCTION__), sExecutionContextId),
      LaunchableModule(LAUNCHABLE_NAME, sExecutionContextId)
{
}

AccelerometerSampleService::~AccelerometerSampleService() 
{
}

bool AccelerometerSampleService::initModule() 
{
    if (registerProviderResources(sProviderResources) != whiteboard::HTTP_CODE_OK)
    {
        return false;
    }

    mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void AccelerometerSampleService::deinitModule() 
{
    unregisterProviderResources(sProviderResources);
    mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

/** @see whiteboard::ILaunchableModule::startModule */
bool AccelerometerSampleService::startModule() 
{
    mModuleState = WB_RES::ModuleStateValues::STARTED;

    return true;
}

void AccelerometerSampleService::onGetRequest(const whiteboard::Request& request,
                              const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("AccelerometerSampleService::onGetRequest() called.");
    
    if (mModuleState != WB_RES::ModuleStateValues::STARTED) {
        return returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
    }

    // Only one request at the time for now...
    if (mOngoingRequests.isFull())
    {
        return returnResult(request, wb::HTTP_CODE_SERVICE_UNAVAILABLE);
    }
    
    switch (request.getResourceConstId())
    {
    case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID:
        {
            whiteboard::RequestId remoteRequestId;
                    
            // Do a GET to LinearAcceleration resource, and wait for result (comes back in onGetResult callback)
            whiteboard::Result result = asyncGet(WB_RES::LOCAL::DEVICE_MEASUREMENT_LINEARACCELERATION::ID, AsyncRequestOptions(&remoteRequestId, 0, true)); 
            // Store incoming requestId so that we can return the result there in callback
            if (!whiteboard::RETURN_OK(result))
            {
                return returnResult(request, result);

            }
            
            bool queueResult = mOngoingRequests.put(remoteRequestId, request);
            WB_ASSERT(queueResult);
        }
    
        break;
        
    default:
        // Return error
        return returnResult(request, whiteboard::HTTP_CODE_NOT_IMPLEMENTED);

    }
}

void AccelerometerSampleService::onGetResult(wb::RequestId requestId, wb::ResourceId resourceId, wb::Result resultCode, const wb::Value& value)
{
    if (resourceId == WB_RES::LOCAL::DEVICE_MEASUREMENT_LINEARACCELERATION::ID) 
    {
        whiteboard::Request relatedIncomingRequest;

        bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);
        
        if (resultCode == wb::HTTP_CODE_OK) 
        {

            // Get result in right type 
            const WB_RES::LinearAccelerationValue& linearAccelerationValue =
                value.convertTo<const WB_RES::LinearAccelerationValue&>();

            
            uint32_t relativeTime = linearAccelerationValue.relativeTime;
            
            float maxAccelerationSq = FLT_MIN;
            if (linearAccelerationValue.measurement.hasValue()) 
            {
                WB_RES::FloatVector3DArray const arrayData = linearAccelerationValue.measurement.getValue();
                
                for (size_t i = 0; i < arrayData.data.size(); i++) 
                {
                    whiteboard::FloatVector3D accValue = arrayData.data[i];
                    float accelerationSq = accValue.mX * accValue.mX + 
                        accValue.mY * accValue.mY + 
                        accValue.mZ * accValue.mZ;
                    
                    if (maxAccelerationSq < accelerationSq)
                        maxAccelerationSq = accelerationSq;
                }
            }
            
            WB_RES::SampleDataValue sampleDataValue;
            sampleDataValue.relativeTime = relativeTime;
            sampleDataValue.value = (maxAccelerationSq == FLT_MIN) ? 0.0f : sqrtf(maxAccelerationSq);
              
            if (relatedRequestFound)
            {
                returnResult(relatedIncomingRequest, resultCode,
                       ResponseOptions::Empty, sampleDataValue);
            }
        }
        else
        {
            DEBUGLOG("DEVICE_MEASUREMENT_LINEARACCELERATION returned error: %u", resultCode);
            if (relatedRequestFound)
            {
                returnResult(relatedIncomingRequest, resultCode);
            }
        }
    }

}

void AccelerometerSampleService::onUnsubscribeResult(whiteboard::RequestId requestId, 
                                     whiteboard::ResourceId resourceId, 
                                     whiteboard::Result resultCode, 
                                     const whiteboard::Value& rResultData)
{
    DEBUGLOG("AccelerometerSampleService::onUnsubscribeResult() called.");
}

void AccelerometerSampleService::onSubscribeResult(whiteboard::RequestId requestId, 
                                     whiteboard::ResourceId resourceId, 
                                     whiteboard::Result resultCode, 
                                     const whiteboard::Value& rResultData)
{
    DEBUGLOG("AccelerometerSampleService::onSubscribeResult() called. resourceId: %u, result: %d", resourceId.localResourceId, (uint32_t)resultCode);

    whiteboard::Request relatedIncomingRequest;
    bool relatedRequestFound = mOngoingRequests.get(requestId, relatedIncomingRequest);
    
    if (relatedRequestFound)
    {
        returnResult(relatedIncomingRequest, wb::HTTP_CODE_OK);
    }
}

whiteboard::Result AccelerometerSampleService::startRunning(whiteboard::RequestId &remoteRequestId)
{
    DEBUGLOG("AccelerometerSampleService::startRunning()");        

    // Reset max acceleration members
    mMaxAccelerationSq = FLT_MIN;
    mSamplesIncluded = 0;
    
    // Subscribe to LinearAcceleration resource (updates at 13Hz), when subscribe is done, we get callback
    wb::Result result = asyncSubscribe(WB_RES::LOCAL::DEVICE_MEASUREMENT_LINEARACCELERATION::ID, AsyncRequestOptions(&remoteRequestId, 0, true));
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncSubscribe threw error: %u", result);        
        return whiteboard::HTTP_CODE_BAD_REQUEST;
    }
    
    return whiteboard::HTTP_CODE_OK;
}

whiteboard::Result AccelerometerSampleService::stopRunning()
{
    DEBUGLOG("AccelerometerSampleService::stopRunning()");        

    // Unsubscribe the LinearAcceleration resource, when unsubscribe is done, we get callback
    wb::Result result = asyncUnsubscribe(WB_RES::LOCAL::DEVICE_MEASUREMENT_LINEARACCELERATION::ID);
    if (!wb::RETURN_OKC(result))
    {
        DEBUGLOG("asyncUnsubscribe threw error: %u", result);        
    }

    
    return whiteboard::HTTP_CODE_OK;
}

// This callback is called when the resource we have subscribed notifies us 
void AccelerometerSampleService::onNotify(whiteboard::ResourceId resourceId, const whiteboard::Value& value,
                            const whiteboard::ParameterList& parameters)
{
    DEBUGLOG("onNotify() called.");

    // Confirm that it is the correct resource
    switch (resourceId.getConstId())
    {
        case WB_RES::LOCAL::DEVICE_MEASUREMENT_LINEARACCELERATION::ID:
        {
            const WB_RES::LinearAccelerationValue& linearAccelerationValue =
                value.convertTo<const WB_RES::LinearAccelerationValue&>();

            if (!linearAccelerationValue.measurement.hasValue()) {
                // No value, do nothing...
                return;
            }

            WB_RES::FloatVector3DArray const arrayData = linearAccelerationValue.measurement.getValue();
            
            uint32_t relativeTime = linearAccelerationValue.relativeTime;
            
            for (size_t i = 0; i < arrayData.data.size(); i++) 
            {
                mSamplesIncluded++;
                
                whiteboard::FloatVector3D accValue = arrayData.data[i];
                float accelerationSq = accValue.mX * accValue.mX + 
                    accValue.mY * accValue.mY + 
                    accValue.mZ * accValue.mZ;
                
                if (mMaxAccelerationSq < accelerationSq)
                    mMaxAccelerationSq = accelerationSq;
            }
            
            // 10Hz 5sec
            if (mSamplesIncluded > 5*10)
            {
                // Reset counter and notify our subscribers
                WB_RES::SampleDataValue sampleDataValue;
                sampleDataValue.relativeTime = relativeTime;
                sampleDataValue.value = sqrtf(mMaxAccelerationSq);
                
                // Reset members
                mSamplesIncluded = 0;
                mMaxAccelerationSq = FLT_MIN;
                
                // and update our WB resource. This causes notification to be fired to our subscribers
                updateResource(WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID,
                       ResponseOptions::Empty, sampleDataValue);
            }
        }
        break;
    }
}



void AccelerometerSampleService::onSubscribe(const whiteboard::Request& request,
                                  const whiteboard::ParameterList& parameters)
{
    switch (request.getResourceConstId())
    {
        case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID:
        {
        // Someone subscribed to our service. Start collecting data and notifying when our service changes state (every 10 seconds)
            whiteboard::RequestId remoteRequestId;
            whiteboard::Result result = startRunning(remoteRequestId);
            
            if (!whiteboard::RETURN_OK(result))
            {
                return returnResult(request, result);

            }
            
            bool queueResult = mOngoingRequests.put(remoteRequestId, request);
            WB_ASSERT(queueResult);
            break;
        }   
        default:
            ASSERT(0); // Should not happen
    }
}

void AccelerometerSampleService::onUnsubscribe(const whiteboard::Request& request,
                                    const whiteboard::ParameterList& parameters)
{
   switch (request.getResourceConstId())
    {
        case WB_RES::LOCAL::SAMPLE_ACCELEROMETER_DATA::ID:
            stopRunning();
            returnResult(request, wb::HTTP_CODE_OK);
            break;
            
        default:
            returnResult(request, wb::HTTP_CODE_BAD_REQUEST);
            ASSERT(0); // Should not happen
    }
}
