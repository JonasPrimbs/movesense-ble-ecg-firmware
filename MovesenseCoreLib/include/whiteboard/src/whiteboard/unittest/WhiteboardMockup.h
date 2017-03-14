#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/Initialization.h"
#include "whiteboard/ResourceClient.h"

#include "whiteboard/unittest/TestProvider.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

// TODO: Refactor this to work with TestProvider and TestClient implementations
// instead of having second implementation here

class DynamicExecutionContext;
class Whiteboard;
class WhiteboardCommunication;

// Custom whiteboard for having multiple whiteboard instances
// and doing some simple requests
class WhiteboardMockup
{
public:
    /** Constructor 
     *
     * @param serialNumber Serial number of Whiteboard
     * @param rConfiguration Configuration
     */
    WhiteboardMockup(const char* serialNumber, const whiteboard::Config& rConfiguration);
    
    /** Destructor */
    ~WhiteboardMockup();

    /** Implicit cast operator for using this class in place of any Whiteboard */
    operator Whiteboard&();

    /**
    * Gets associated communication class instance
    *
    * @return Whiteboard communication instance
    */
    WhiteboardCommunication& getCommunication();

    /** Gets simulation execution context of the whiteboard */
    whiteboard::ExecutionContextId getExecutionContextId();

    wb::Result mResult;
    const char* mpFullPath;
    size_t mTimeoutMs;

    wb::Result syncGetResource(const char* pFullPath, size_t timeoutMs = 0);

    wb::ResourceId mResourceId;
    const wb::ParameterList* mpParameters;
    const wb::ResourceClient_AsyncRequestOptions* mpOptions;

    wb::Result syncGet(wb::ResourceId resourceId, const wb::ParameterList& rParameters, const wb::ResourceClient_AsyncRequestOptions& rOptions);

    bool hasResultBeenReceived();
    whiteboard::Result getResult() const;
    void resetResult();

    const whiteboard::ResourceId getResourceId() const;

private:
    class ResourceClientMockup : public whiteboard::ResourceClient
    {
    public:
        ResourceClientMockup(whiteboard::Whiteboard& rWhiteboard, whiteboard::ExecutionContextId executionContextId);
        virtual ~ResourceClientMockup();

        bool hasResultBeenReceived();
        whiteboard::Result getResult();
        void resetResult();

        const whiteboard::ResourceId getResourceId() const;

    private:
        void onGetResourceResult(whiteboard::RequestId requestId,
            whiteboard::ResourceId resourceId,
            whiteboard::Result resultCode) OVERRIDE;

        void onGetResult(whiteboard::RequestId requestId,
            whiteboard::ResourceId resourceId,
            whiteboard::Result resultCode,
            const whiteboard::Value& /*rResultData*/) OVERRIDE;

    private:
        whiteboard::Whiteboard& mrWhiteboard;
        volatile whiteboard::Result mResult;
        whiteboard::ResourceId mResourceId;
    };

    wb::RequestId getNextRequestId(wb::RequestId* pRequestIdOption);

    void getResourceDpc();
    void getDpc();

private:
    whiteboard::Whiteboard* mpWhiteboard;
    whiteboard::DynamicExecutionContext* mpExecutionContext;
    whiteboard::TestProvider<int32>* mpProvider;
    ResourceClientMockup* mpClient;
    wb::RequestId mNextRequestId;
};

} // namespace whiteboard

#endif
