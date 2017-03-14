// Copyright (c) Suunto Oy 2015-2016. All rights reserved.
#pragma once

#include <whiteboard/ResourceClient.h>
#include "plugin-api/plugin-api.hpp"
#include "semaphorecxx11.h"

namespace wbcmd
{

class CmdLine;

class RequestClient final : public whiteboard::ResourceClient
{
public:
    RequestClient(
        const wbcmd::CmdLine& rCmdLine,
        const std::string& whiteboardAddress,
        const wb::IDataTypeMetadataContainer* pMetadataContainer,
        semaphore& rOperationCompleted);
    
    virtual ~RequestClient() { }

    void executeGetResourceId();
    void executeOperation();

    inline wb::ResourceId getResourceId() const
    {
        return mResourceId;
    }

    inline const wbjson::Json& getResult() const
    {
        return mResult;
    }

private:
    const wbcmd::CmdLine& mrCmdLine;
    const std::string mWhiteboardAddress;
    const std::string mFullResourcePath;
    const wb::IDataTypeMetadataContainer* mpMetadataContainer;
    semaphore& mrOperationCompleted;

    /// List of parameter related allocations that need cleaning after operation
    /// has completed. ByteStream objects must wait until operation has completed,
    /// others could be deallocated immediately after asyncRequest call.
    std::vector<std::unique_ptr<ICleanable>> mCleanupList;

    /// Contains the id of the resource we are going to request
    wb::ResourceId mResourceId;
    wbjson::Json mResult;

    virtual void onGetResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
        whiteboard::Result resultCode, const whiteboard::Value& resultData) override;

    virtual void onGetResourceResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
        whiteboard::Result resultCode) override;

    virtual void onPutResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
        whiteboard::Result resultCode, const whiteboard::Value& rResultData) override;

    virtual void onPostResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
        whiteboard::Result resultCode, const whiteboard::Value& rResultData) override;

    virtual void onDeleteResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
        whiteboard::Result resultCode, const whiteboard::Value& rResultData) override;

private:
    void setResult(const std::string& operation, whiteboard::Result resultCode, const std::string& message = "", const wbjson::Json& content = wbjson::Json());
};

}
