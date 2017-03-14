#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/

#include <functional>
#include <iostream>
#include "wbcmd.hpp"
#include "plugin-api/plugin-api.hpp"
#include <whiteboard/ResourceClient.h>
#include "wb-resources/resources.h"

namespace wbcmd
{

typedef std::function<void(const wb::Result)> MetadataResolutionComplete_t;

/** MetadataRequestHandler class can be used to fetch the
 * metadata in binary from the target. Calls the provided operationCompleted 
 * callback when everything is fetched and after that the buffer can be
 * accessed via \ref getMetadataSlices().
 */
class MetadataRequestHandler final : public wb::ResourceClient
{
public:
    const std::vector<std::vector<uint8_t>> & getMetadataSlices() const { return mMetadataSlices; }

    static const int METADATA_REQUEST_TIMEOUT_MS = 3000;

    MetadataRequestHandler(
        const std::string& whiteboardAddress,
        const MetadataResolutionComplete_t operationCompleted):
        wb::ResourceClient(__FUNCTION__, WBCMD_EXECUTION_CONTEXT),
        mMetaDataResourceId(WB_RES::LOCAL::META_METADATASTREAM::ID),
        mMetadataRequestId(wb::ID_INVALID_REQUEST),
        mOperationCompleted(operationCompleted),
        mMetadataStart(NOW())
    {
        // Replace local whiteboard id with remote whiteboard id
        mMetaDataResourceId.whiteboardId = getWhiteboardId(whiteboardAddress);
    }

    void startMetadataRequest()
    {
        const wb::ResourceClient::AsyncRequestOptions options(&mMetadataRequestId, METADATA_REQUEST_TIMEOUT_MS);
        const wb::Result asyncResult = asyncGet(mMetaDataResourceId, options);

        if (!wb::RETURN_OK(asyncResult))
        {
            WB_DEBUGLOG("ERROR: operation failed %s", wb::getResultString(asyncResult));
            mOperationCompleted(asyncResult);
        }
    }

private:
    void onGetResult(wb::RequestId requestId, wb::ResourceId resourceId,
        wb::Result resultCode, const wb::Value& resultData) override
    {
        if (resultData.getType() != wb::WB_TYPE_BYTE_STREAM || !wb::RETURN_OKC(resultCode))
        {
            WB_DEBUGLOG("ERROR: resolving metadata failed %s", wb::getResultString(resultCode));
            mOperationCompleted(resultCode);
            return;
        }

        const wb::ByteStream& stream = resultData.convertTo<const wb::ByteStream&>();
        mMetadataSlices.push_back({stream.data, stream.data+ stream.length() });
#if defined(_DEBUG)
        std::cout << ".";
#endif

        if (resultCode == wb::HTTP_CODE_CONTINUE)
        {
            const wb::ResourceClient::AsyncRequestOptions options(NULL, METADATA_REQUEST_TIMEOUT_MS);
            asyncGet(mMetaDataResourceId, options, mMetadataRequestId);
        }
        else
        {
#if defined(_DEBUG)
            std::cout << std::endl;
#endif

            const auto end = NOW();
            const auto durationms =
                std::chrono::duration_cast<std::chrono::milliseconds>(end - mMetadataStart).count();
            WB_DEBUGLOG("Metadata bytestream: done in %d ms", durationms);
            mOperationCompleted(wb::HTTP_CODE_OK);
        }
    }

private:
    wb::ResourceId mMetaDataResourceId;
    wb::RequestId mMetadataRequestId;
    const MetadataResolutionComplete_t mOperationCompleted;
    std::chrono::high_resolution_clock::time_point mMetadataStart;
    std::vector<std::vector<uint8_t>> mMetadataSlices;
};

} // namespace wbcmd
