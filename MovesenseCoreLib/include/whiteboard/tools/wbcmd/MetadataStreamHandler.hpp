#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <whiteboard/services/MetadataStreamReceiver.h>
#include "MetadataContainer.hpp"

namespace wbcmd {

class MetadataStreamHandler final : public wb::services::IMetadataReceiver
{
    MetadataContainer &mrMetadataContainer;
public:
    MetadataStreamHandler(MetadataContainer &rMetadataContainer)
        : mrMetadataContainer(rMetadataContainer) {}
    virtual ~MetadataStreamHandler() {}

    virtual void onMetadataValue(
        const WB_RES::ExecutionContextMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT ExecutionContextMetadata");
        mrMetadataContainer.setExecutionContext(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::ResourceMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT ResourceMetadata");
        mrMetadataContainer.setResource(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::OperationMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT OperationMetadata");
        mrMetadataContainer.setOperation(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::ResponseMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT ResponseMetadata");
        mrMetadataContainer.setResponse(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::ParameterMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT ParameterMetadata");
        mrMetadataContainer.setParameter(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::DataTypeMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT DataTypeMetadata");
        mrMetadataContainer.setDataType(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::SecurityTagMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT SecurityTagMetadata");
        mrMetadataContainer.setSecurityTag(metadata.id, metadata);
    }
    virtual void onMetadataValue(const WB_RES::StringMetadata& metadata) override final
    {
        // WB_DEBUGLOG("GOT StringMetadata");
        mrMetadataContainer.setString(metadata.id, metadata);
    }
};
}
