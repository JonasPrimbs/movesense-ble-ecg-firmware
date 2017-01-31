#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "wb-resources/resources.h"

namespace whiteboard
{
namespace services
{

/** Interface for metadata receivers */
class IMetadataReceiver
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IMetadataReceiver() {}

public:
    // Callbacks that are called when metadata object has been successfully parsed
    // from the stream
    virtual void onMetadataValue(const WB_RES::ExecutionContextMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::ResourceMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::OperationMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::ResponseMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::ParameterMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::DataTypeMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::SecurityTagMetadata& metadata) = 0;
    virtual void onMetadataValue(const WB_RES::StringMetadata& metadata) = 0;
};

/**
    Helper class for whiteboard metadata stream handling.
*/
class MetadataStreamParser
{
public:
    /** Constructor */
    MetadataStreamParser();

    /** Destructor */
    ~MetadataStreamParser();

private:

    /** Moves stream receiver to next state */
    void moveToNextState();

    /** Stream receive states */
    enum StreamReceiveState
    {
        EXECUTION_CONTEXT_METADATA,
        RESOURCE_METADATA,
        OPERATION_METADATA,
        RESPONSE_METADATA,
        PARAMETER_METADATA,
        DATATYPE_METADATA,
        SECURITYTAG_METADATA,
        STRING_METADATA
    };

    /** Current state */
    StreamReceiveState mState;

    /** Number of items to receive in current state */
    uint16 mCount;

    /** Current item index in current state */
    uint16 mIndex;

public:
    /** Deserializes data from given buffer
     *
     * @param rCallbacks Metadata receiver callbacks
     * @param rpBufferPos Buffer position
     * @param rBufferRemainingLength Number of bytes remaining in the buffer
     * @return A value indicating whether the meta data was successfully parsed from the buffer
     */
    bool deserializeFromBuffer(IMetadataReceiver& rCallbacks, const uint8*& rpBufferPos, size_t& rBufferRemainingLength);
};

} // namespace services
} // namespace whiteboard
