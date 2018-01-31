#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <whiteboard/integration/port.h>
#include <whiteboard/WhiteboardConfig.h>
#include <whiteboard/Result.h>

namespace whiteboard {
namespace comm_ble {
class BleAdapter;
} // namespace comm_ble
} // namespace whiteboard

//#define TRACE_DATA_CONTENT 1
//void printDataBuffer(const char *msg, const void *buf, uint32_t size);

namespace wbble
{
/** Type of buffer that stores communication addresses */
typedef uint8 Address[WB_COMM_MAX_ADDRESS_LENGTH];

/** Initialize BLE interface
*/
void Initialize();

/** Deinitialize BLE interface
*/
void Deinitialize();

/** Convert human readable address to device specific Whiteboard Address representation
*
* @param pDevAddr Human readable address string
* @param wbAddr Device specific Whiteboard Address representation
* @return true if success, false otherwise
*/
bool ConvertAddress(const char* pDevAddr, Address& wbAddr);

/** Formats the device specific Whiteboard Address to human readable form
*
* @param rAddress Address that should be formatted
* @param bufferSize Maximum number of charactes in the buffer including zero terminator
* @param addressBuffer Buffer where address should be written
* @return true if success, false otherwise
*/
bool FormatAddress(const Address& rAddress, size_t bufferSize, char* addressBuffer);

/** Connect to the given address
*
* @param address Address of the destination
* @return status of operation after the call
*/
whiteboard::Result Connect(const Address& address);

/** Cancel an ongoing connect to the given address
*
* @param address Address of the destination
* @return status of operation after the call
*/
whiteboard::Result CancelConnect(const Address& address);

/** Disconnect from the given address
*
* @param address Address of the destination
* @return status of operation after the call
*/
whiteboard::Result Disconnect(const Address& address);

/** Send a message to the given address
*
* @param address Address of the destination
* @param pData Pointer to the data to send
* @param size Size of the data to send
* @return status of operation after the call
*/
whiteboard::Result Send(const Address& address, const void* pData, uint32 size);

/** Cancel sending a message
*
* @param pData Pointer to the data to be canceled
* @return status of operation after the call
*/
whiteboard::Result CancelSend(const void* pData);

/** Callback that should be called when send completes. */
void onSendCompleted(const void* pData, bool success);

/** Callback that should be called when new data is receivd. */
void onDataReceived(const Address& address, const void* pData, uint32 size);

/** Callback that should be called when asynchronous connection attempt completes.
 * Do not call this unless Connect has been called first.
 */
void onConnected(const Address& address, bool success);

/** Callback that should be called when asynchronous disconnect completes.
 *
 * Do not call this unless Disconnect has been called first.
 */
void onDisconnected(const Address& address, bool success);

/** Callback that should be called when connection state changes. */
void onConnectionState(const Address& address, bool connected);

} // namespace wbble
