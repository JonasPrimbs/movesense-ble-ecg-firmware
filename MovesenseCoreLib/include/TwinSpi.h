/**
 * @brief Initiates SPI bus controller for AFE and Accelerometer
 *
 * @return    true if success 
 */

bool TwinSpiInit();


/**
 * @brief Function for exchanging data over SPI bus without using EasyDMA
 *
 * @param[in] cs_pin_number     Chip Select pin number
 * @param[in] tx_buffer         Pointer to the transmit buffer. Can't be NULL
 * @param[in] tx_size           Number of bytes to send, minimum 1
 * @param[in] disscard_size     Number of bytes to ignore before start receiving
 * @param     rx_buffer         Pointer to the receive buffer. Can be NULL
 * @param[in] rx_size           Number of bytes to receive, can be 0
 *
 * @return    true if success 
 */

bool SPIE_runPolling(Pin csPin, void * txBuffer, unsigned sendCount, 
    unsigned rxDiscardCount, void * rxBuffer, unsigned receiveCount);
