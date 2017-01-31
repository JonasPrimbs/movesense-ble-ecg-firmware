/******************************************************************************

Copyright (c) Suunto Oy 2016.
All rights reserved.

******************************************************************************/
#pragma once
#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"
#include "filesystems/file.h"

namespace nea
{

/**
 * Utilities for sending and receiving data using XMODEM, XMODEMCRC and XMODEM1K.
 *
 * Based on specifications found in:
 * http://fieldeffect.info/wp/frontpage/xmodem/
 * http://www.ninerpedia.org/index.php?title=Protocols
 * https://pythonhosted.org/xmodem/xmodem.html
 *
 * Originally implemented for Valencell PT4110 firmware update.
 * No timings included and no batch support of any kind. Mixed size packets are supported.
 *
 * XMODEM1K data flow example:
 * SENDER                                    RECEIVER
 *
 *                                       <-- ASCII C
 * STX 1 FE Data[1024] CRC16             -->
 *                                       <-- ACK
 * STX 2 FD Data[1024] CRC16             -->
 *                                       <-- ACK
 * STX 3 FC Data[1000] Padding[24] CRC16 -->
 *                                       <-- ACK
 * EOT                                   -->
 *                                       <-- ACK
 *
 * Interface usage:
 * TX:
 *
 * XModemTx tx(interface);
 * FileHandle file = fileOpen("image.gif", FILE_MODE_READ);
 *
 * tx.init(file);
 *
 * while (!tx.isFinished()) {
 *     if (!tx.transmit(DATASIZE1K)) {
 *          // Report error.
 *     }
 * }
 *
 * fileClose(file);
 *
 * if (tx.isDone()) {
 *    return true; // Success
 * }
 *
 * while (!tx.cancel());
 *
 * return false; // Failure
 *
 * RX:
 *
 * XModemRx rx(interface);
 * FileHandle file = fileOpen("image.gif", FILE_MODE_WRITE);
 *
 * rx.init(file, true);
 *
 * while (!rx.isFinished()) {
 *     if (!rx.receive()) {
 *          // Report error.
 *     }
 * }
 *
 * fileClose(file);
 *
 * if (rx.isDone()) {
 *    return true; // Success
 * }
 *
 * while (!rx.cancel());
 *
 * return false; // Failure
 *
 */

/**
 * XMODEM communication interface.
 */
class XModemComm
{
public:
    XModemComm();
    virtual ~XModemComm();

    /**
     * Expected to move size bytes forward in stream after each successful call.
     */
    virtual bool read(void* buffer, size_t size) = 0;
    /**
     * Expected to move size bytes forward in stream after each successful call.
     */
    virtual bool write(const void* buffer, size_t size) = 0;
    /**
     * Guarantee buffer is emptied. Implement if required in used interface.
     */
    virtual void flush();
};

/**
 * XMODEM base class.
 * Contains general definitions and common methods and attributes.
 */
class XModem
{
public:
    // Internal states.
    enum XModemState_e {
        HANDSHAKE,       // Initial state. Handshake byte expected from receiver.
        TRANSFER,        // Data is transferred one packet at the time.
        FINALIZE,        // Ending transfer. Initiated by transmitter.
        DONE,            // All done.
        TOO_MANY_ERRORS, // Error threshold reached.
        CANCELLED,       // Transfer is cancelled by either side.
    };

    // Packet size and position constants.
    static const uint8  DATAPOS      = 3;        // Offset of data in packet.
    static const uint8  DATASIZE     = 128;
    static const uint8  PACKETSIZE   = 128 + 5;  // Regular packet size. Place for header, data and CRC/Checksum.
    static const uint16 DATASIZE1K   = 1024;
    static const uint16 PACKETSIZE1K = 1024 + 5; // 1k packet size. Place for header, data and CRC/Checksum.

    // Mapping ASCII characters to their names.
    static const uint8 SOH = 0x01; // Start byte of regular packet.
    static const uint8 STX = 0x02; // Start byte of 1k packet.
    static const uint8 EOT = 0x04; // End of transfer.
    static const uint8 ACK = 0x06; // Acknowledge.
    static const uint8 NAK = 0x15; // Not-acknowledge. Handshake of non-CRC transfer.
    static const uint8 CAN = 0x18; // Cancel.
    static const uint8 PAD = 0x1A; // Padding to fill last packet. (Called SUB in ASCII.)
    // Other: ASCII C = Handshake of CRC transfer.

    /**
     * Constructor.
     * @param interface Communication layer instance
     */
    XModem(XModemComm& interface);
    /*
     * Destructor.
     */
    virtual ~XModem();
    /**
     * Resets to initial internal state.
     * Reserves PACKETSIZE1K size buffer if not done already.
     * @param file   Handle to open file.
     * @param useCrc
     */
    virtual void init(FileHandle file, const bool useCrc = false);
    /**
     * Method to clear dynamic resources.
     */
    virtual void clear();
    /**
     * Runs one iteration of implemented cancel sequence.
     * @return True when sequence is over.
     */
    virtual bool cancel();
    /**
     * Tells if process has reached one of the final states. Use getState method to find out which one.
     * @return Final state reached or not
     */
    bool isFinished() const;
    /**
     * Tells if file transfer was successfully completed.
     * @return Success
     */
    bool isDone() const;
    /**
     * Returns transfer state.
     */
    XModemState_e getState() const;
    /**
     * Returns total error count.
     */
    uint8 getErrorCount() const;
    /*
     * Utility to get packet size from buffer containing a packet.
     */
    static size_t getDataSize(const uint8* buffer);
    /*
     * Utility to calculate packet size from data size and checksum type.
     */
    static size_t getPacketSize(const size_t dataSize, const bool isCrc);
    /*
     * Utility to wrap data into packet.
     * @param buffer      Contains data at buffer + DATAPOS.
     * @param bufferSize  Size of buffer.
     * @param dataSize    Length of data in buffer.
     * @param blockNumber Sequence number of current block.
     * @param isCrc       To use CRC or regular checksum.
     * @return            Total size of created packet. Buffer must be at least this large.
     */
    static size_t createPacket(uint8* buffer, const size_t bufferSize, const size_t dataSize, const uint8 blockNumber, const bool isCrc);
    /**
     * Utility to check if packet is valid.
     * @param buffer      Buffer containing packet.
     * @param bufferSize  Size of buffer.
     * @param blockNumber Running block number.
     * @param isCrc
     * @return 0 - Packet is invalid. CRC/Checksum or invalid block number.
     *         1 - Packet is valid.
     *        -1 - Block number is one smaller than expected.
     */
    static int checkPacket(const uint8* buffer, const size_t bufferSize, const uint8 blockNumber, const bool isCrc);
    /**
     * Calculates checkcum over byte array.
     * Checksum is the least significant byte of sum of byte values.
     * @param buffer Pointer to byte array.
     * @param size   Size of byte array.
     * @return       Checksum.
     */
    static uint8 checksum(const uint8* buffer, const size_t size);

protected:
    /**
     * Can be used to mangle the input value into something else for desired effect.
     * Requires knowledge of internal workings.
     * @param input Pointer to input byte that was read.
     */
    virtual void inputTransform(uint8* input);
    /**
     * Do something after state change.
     * @param newState
     */
    virtual void onStateChange(const XModemState_e newState);
    /**
     * Updates error count according to given result parameter.
     * @param result Result to handle. Defaults to false. True is only echoed as return value.
     * @return       result parameter's value.
     */
    bool handleError(bool result = false);
    /**
     * Sets internal state. Resets errors when actual change is detected.
     * @param state one of XModemState_e.
     */
    void setState(const XModemState_e state);

protected:
    /**
     * Communication interface
     */
    XModemComm& mrComm;
    /**
     * Handle to used file.
     */
    FileHandle mFile;
    /*
     * Buffer used to store packet.
     */
    uint8*     mpBuffer;
    /**
     * Running block number. On first round starts from 1 to 255. After that runs from 0 to 255.
     */
    uint8      mBlockNumber;
    /**
     * Register of on/off values.
     */
    struct Flags {
        bool crc : 1;  // Is CRC used.
        bool sent : 1; // Was data sent successfully.
    } mFlags;
    /**
     * Cancel counters.
     */
    struct CancelCount {
        uint8 delivered : 4;
        uint8 total : 4;
    } mCancel;

private:
    /*
     * State of transfer. Determines what kind of input/output data is expected.
     */
    XModemState_e mState;
    /**
     * Total count of errors. Includes all kinds (protocol, I/O).
     */
    uint8         mErrorCount;
};

/**
 * XMODEM transmitter interface.
 */
class XModemTx : public XModem
{
public:
    XModemTx(XModemComm& interface);
    virtual ~XModemTx();
    /**
     * Resets TX parameters.
     * Calls base class implementation.
     * @param file   Handle to file opened in read mode.
     * @param useCrc Can be used to force CRC. Otherwise detected from first input.
     */
    virtual void init(FileHandle file, const bool useCrc = false) OVERRIDE;
    /**
     * Runs one iteration of file transmit.
     * @param  Requested packet size. Can be smaller if not enough data left.
     * @return True if read/write operations were successful and error limit not reached.
     */
    bool transmit(const size_t dataSize = DATASIZE);

private:
    /**
     * Helper method to prepare next packet or
     * go to finalizing state when no more data is available.
     */
    void nextPacket(const size_t dataSize);
    /**
     * Cache for size of latest packet.
     */
    uint16 mPacketSize;
};

/**
 * XMODEM receiver interface.
 */
class XModemRx : public XModem
{
public:
    XModemRx(XModemComm& interface);
    virtual ~XModemRx();
    /**
     * Resets RX parameters.
     * Calls base class implementation.
     * @param file   Handle to file opened in write mode.
     * @param useCrc Enables support for CRC.
     */
    virtual void init(FileHandle file, const bool useCrc = false) OVERRIDE;
    /**
     * Runs one iteration of file receive.
     * @return True if read/write operations were successful and error limit not reached.
     */
    bool receive();
};

}
