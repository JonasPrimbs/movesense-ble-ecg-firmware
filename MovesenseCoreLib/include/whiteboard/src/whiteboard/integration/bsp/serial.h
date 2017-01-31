#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

/** Type of a serial port */
struct WbSerialPort;
typedef WbSerialPort* WbSerialPortHandle;

#define WB_SERIAL_PORT_UART_BAUDRATE (115200u) // TODO OSX: #define WB_SERIAL_PORT_UART_BAUDRATE (B115200)

/** Data bits settings */
enum WbSerialDataBits
{
    WB_SERIAL_DATABITS_FOUR = 4,
    WB_SERIAL_DATABITS_FIVE,
    WB_SERIAL_DATABITS_SIX,
    WB_SERIAL_DATABITS_SEVEN,
    WB_SERIAL_DATABITS_EIGHT,
    WB_SERIAL_DATABITS_NINE,
    WB_SERIAL_DATABITS_TEN,
    WB_SERIAL_DATABITS_SIXTEEN = 16
};

/** Parity settings */
enum WbSerialParity
{
    WB_SERIAL_PARITY_NONE,
    WB_SERIAL_PARITY_ODD,
    WB_SERIAL_PARITY_EVEN,
    WB_SERIAL_PARITY_MARK,
    WB_SERIAL_PARITY_SPACE
};

/** Stop bits settings */
enum WbSerialStopBits
{
    WB_SERIAL_STOPBITS_ONE = 0,
    WB_SERIAL_STOPBITS_ONE_AND_HALF,
    WB_SERIAL_STOPBITS_TWO
};

/** Flow control settings */
enum WbSerialFlowControl
{
    WB_SERIAL_FLOWCONTROL_NONE,
    WB_SERIAL_FLOWCONTROL_XON_XOFF,
    WB_SERIAL_FLOWCONTROL_HARDWARE,
};

/** Opens given serial port
*
* @param portName Name of the port
* @param baudRate Communication baud rate
* @param dataBits Number of data bits
* @param parity Communication parity mode
* @param stopBits Number of stop bits
* @param flowControl Flow control settings
* @return Serial port handle of the opened serial port
*/
WB_API WbSerialPortHandle WbSerialOpen(
    const char* portName,
    uint32 baudRate,
    WbSerialDataBits dataBits,
    WbSerialParity parity,
    WbSerialStopBits stopBits,
    WbSerialFlowControl flowControl);

/** Closes given serial port
*
* @param socket Socket to close
*/
WB_API void WbSerialClose(WbSerialPortHandle port);

/** Sends a message
*
* @param port Port from which the message should be sent
* @param pBuffer Buffer that contains the message
* @param lengthInBytes Length of the buffer in bytes
* @return A value indicating whether the operation succeeded
*/
WB_API bool WbSerialSend(WbSerialPortHandle port, const void* pBuffer, uint32 lengthInBytes);

/** Receives a message
*
* @param port Port from which the message should be sent
* @param pBuffer Buffer that contains the message
* @param rLengthInBytes On input contains length of the buffer in bytes. On output contains length of the data in bytes
* @return A value indicating whether the operation succeeded
*/
WB_API bool WbSerialReceive(WbSerialPortHandle port, void* pBuffer, uint32& rLengthInBytes);

