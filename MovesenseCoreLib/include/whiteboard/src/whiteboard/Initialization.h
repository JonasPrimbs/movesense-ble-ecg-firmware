#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2014.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/ResourceTree.h"
#ifdef WB_HAVE_ELASTIC_INTERFACE
#include "whiteboard/elastic/IFile.h"
#endif

// Metadata of Whiteboard resources. This may contain also embedded application resources,
// which is more optimal than having a separate resource tree for Whiteboard and application.
// This should be generated during application build and is therefore declared in this header.
extern const whiteboard::MetadataMap WbMetadata;

namespace whiteboard
{

/** Whiteboard runtime configuration */
struct Config
{
    /** Type that is used to specify sizes */
    typedef uint16 Size;

    /** Pool size */
    struct PoolSize
    {
        /** Default constructor */
        inline PoolSize()
        {
        }

        /** Constructor */
        inline PoolSize(Size maximumSize_, bool dynamicGrow_ = false)
            : maximumSize(maximumSize_), dynamicGrow(dynamicGrow_)
        {
        }

        /** Maximum number of items */
        Size maximumSize : 15;

        /** A value indicating whether array is grown dynamically */
        Size dynamicGrow : 1;
    };

    /** Size of the client registry. */
    PoolSize clientRegistrySize;

    /** Size of provider registry */
    PoolSize providerRegistrySize;
    
    /** Size of subscription registry */
    PoolSize subscriptionRegistrySize;

    /** Size of path parameter cache */
    PoolSize pathParameterCacheSize;

    /** Size of timer pool */
    Size timerPoolSize;

    /** Size of timed DPC pool */
    Size timedDpcPoolSize;

    /** Size of stream pool */
    PoolSize streamPoolSize;

    /** Size of routing table */
    PoolSize routingTableSize;

    /** Size of IP connection pool */
    PoolSize ipConnectionPoolSize;

    /** Maximum number of comm thread events */
    uint8 maxCommEvents;

    /** Maximum number of send requests */
    uint8 maxCommSendRequests;

    /** Maximum number of receive requests */
    uint8 maxCommReceiveRequests;

    /** Buffer pool group configuration */
    struct BufferPoolGroupConfig
    {
        /** Default constructor */
        inline BufferPoolGroupConfig()
        {
        }

        /** Constructor */
        inline BufferPoolGroupConfig(Size numberOfBuffers_, Size bufferSize_)
            : numberOfBuffers(numberOfBuffers_), bufferSize(bufferSize_)
        {
        }

        /** Number of buffers in the group */
        Size numberOfBuffers;

        /** Size of a single buffer in the group */
        Size bufferSize;
    };

    /** Configuration of smallest buffer pool buffers */
    BufferPoolGroupConfig bufferPoolSmallGroup;

    /** Configuration of medium-sized buffer pool buffers */
    BufferPoolGroupConfig bufferPoolMediumGroup;

    /** Configuration of large buffer pool buffers */
    BufferPoolGroupConfig bufferPoolLargeGroup;

    /** Configuration of maximum length buffer pool buffers */
    Size bufferPoolMaxLengthGroupNumberOfBuffers;

    /** Configuration of communication receive buffers */
    Size bufferPoolCommReceiveGroupNumberOfBuffers;

    /** Default timeout for client requests in milliseconds */
    Size defaultRequestTimeoutMs;

    /** Whiteboard comm-threads stack size, use as 400, 512 needed in unittests */
    Size wbCommProtoStackDepth;

    /** IP receiver stack depth */
    Size ipReceiverStackDepth;

    /** Serial receiver stack depth */
    Size serialReceiverStackDepth;
};

/** Initializes whiteboard library
*
* @param serialNumber Serial number to use
* @param rConfiguration Runtime configuration to use
* @param pApplicationMetadata Optional. Metadata for the application.
* @param hookInstaller Optional. Hook installer function.
* @param fileInterfaceImplementation Optional. File interface for metadata cache.
*/
WB_API void initialize(
    const char* serialNumber, 
    const Config& rConfiguration,
    const whiteboard::MetadataMap * const pApplicationMetadata = &WbMetadata,
    ResourceTree::HookInstallerFunc* hookInstaller = NULL
#ifdef WB_HAVE_ELASTIC_INTERFACE
    , elastic::IFile* fileInterfaceImplementation = NULL
#endif
);

/** Shuts down whiteboard library
*/
WB_API void shutdown();

#ifdef WB_HAVE_CRASH_DUMP
/** Log whiteboard state in system events when fatal assert or errorneous state is detected.
 *
 * @note Will disable interrupt handling and no scheduling occur while the operation is in progress.
 * @note The operation is irreversible and will empty all event queues.
 * @note Will NOT exit the application (needs to be done by the caller after crashDump returns).
*/
WB_API void crashDump();
#endif

/** Gets Whiteboard singleton instance. This is used by unit test helper classes
 * to access Whiteboard singleton without exposing its internal implementation.
 * 
 * @note: This function can be called only when Whiteboard has been initialized
 * @return Whiteboard singleton instance
 */
WB_API class Whiteboard& getWhiteboardInstance();

} // namespace whiteboard
