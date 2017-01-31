#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"

namespace whiteboard
{
class BleAdapter;
}

//#define TRACE_DATA_CONTENT 1
//void printDataBuffer(const char *msg, const void *buf, uint32_t size);

namespace wbble
{

/** Type of buffer that stores communication addresses */
typedef uint8 Address[WB_COMM_MAX_ADDRESS_LENGTH];

/** Initialize BLE interface
*/
WB_API void Initialize();

/** Deinitialize BLE interface
*/
WB_API void Deinitialize();

/** Convert human readable address to device specific Whiteboard Address representation
*
* @param pDevAddr Human readable address string
* @param wbAddr Device specific Whiteboard Address representation
* @return true if success, false otherwise
*/
WB_API bool ConvertAddress(const char* pDevAddr, Address& wbAddr);

/** Formats the device specific Whiteboard Address to human readable form
*
* @param rAddress Address that should be formatted
* @param bufferSize Maximum number of charactes in the buffer including zero terminator
* @param addressBuffer Buffer where address should be written
* @return true if success, false otherwise
*/
WB_API bool FormatAddress(const Address& rAddress, size_t bufferSize, char* addressBuffer);

/** Connect to the given address
*
* @param address Address of the destination
* @return true if connection establishment was started, false otherwise
*/
WB_API bool Connect(const Address& address);

/** Cancel an ongoing connect to the given address
*
* @param address Address of the destination
* @return true if connection establishment was started, false otherwise
*/
WB_API bool CancelConnect(const Address& address);

/** Disconnect from the given address
*
* @param address Address of the destination
* @return true if disconnection was started, false otherwise
*/
WB_API bool Disconnect(const Address& address);

/// Ble send result
enum SendResult_e
{
    SEND_RESULT_ACCEPTED, ///< Send was accepted, but the send is pending (async send)
    SEND_RESULT_OK,       ///< Send was ok
    SEND_RESULT_FAILED    ///< Send failed
};

/** Send a message to the given address
*
* @param address Address of the destination
* @param pData Pointer to the data to send
* @param size Size of the data to send
* @return @see wbble::SendResult_e
*/
WB_API SendResult_e Send(const Address& address, const void* pData, uint32 size);

/** Cancel sending a message
*
* @param pData Pointer to the data to be canceled
* @return true if sending was canceled, false otherwise
*/
WB_API bool CancelSend(const void* pData);

/** Callback that should be called when send completes. */
WB_API void onSendCompleted(const void* pData, bool success);

/** Callback that should be called when new data is receivd. */
WB_API void onDataReceived(const Address& address, const void* pData, uint32 size);

/** Callback that should be called when asynchronous connection attempt completes.
 * Do not call this unless Connect has been called first.
 */
WB_API void onConnected(const Address& address, bool success);

/** Callback that should be called when asynchronous disconnect completes.
 *
 * Do not call this unless Disconnect has been called first.
 */
WB_API void onDisconnected(const Address& address, bool success);

/** Callback that should be called when connection state changes. */
WB_API void onConnectionState(const Address& address, bool connected);

} // namespace wbble
