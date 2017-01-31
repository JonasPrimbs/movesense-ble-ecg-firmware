// Copyright (c) Suunto Oy 2015. All rights reserved.
#pragma once

#include "whiteboard/ResourceClient.h"
#include "whiteboard/ResourceProvider.h"
#include "whiteboard/DpcFunctor.h"

namespace whiteboard
{
namespace services
{

// Buffer size allocated for ListOfBytes. Must be less than WB_COMM_PROTO_MAX_MESSAGE_LENGTH including headers.
static const int TEST_PACKET_SIZE = 400;

/** Throughput service provider */
class ThroughputProvider FINAL : private ResourceProvider
{
public:
    /** Initializes a new instance of ThroughputProvider
    */
    ThroughputProvider();

    /** Destructor */
    virtual ~ThroughputProvider();

    /// Called by the CommServices launchable module
    void initSubmodule();
    void deinitSubmodule();

private:
    /**
    *	GET request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@return Result of the operation
    */
    void onGetRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;

    /**
    *	PUT request handler.
    *
    *	@param rRequest Request information
    *	@param rParameters List of parameters for the request
    *	@return Result of the operation
    */
    void onPutRequest(const Request& rRequest, const ParameterList& rParameters) OVERRIDE;
};

} // namespace services
} // namespace whiteboard
