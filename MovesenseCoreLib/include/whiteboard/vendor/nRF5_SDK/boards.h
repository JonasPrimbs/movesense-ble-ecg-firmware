
#ifndef BOARDS_H
#define BOARDS_H

#include "nrf_gpio.h"
#include "hwconfig_pins.h"

// Needed for app_trace.c
#define RX_PIN_NUMBER  (PIN_ALIAS_DEBUG_RX)    // UART RX pin number.
#define TX_PIN_NUMBER  (PIN_ALIAS_DEBUG_TX)    // UART TX pin number.
#define CTS_PIN_NUMBER (-1)    // UART Clear To Send pin number. Not used if HWFC is set to false. 
#define RTS_PIN_NUMBER (-1)    // UART Request To Send pin number. Not used if HWFC is set to false. 
#define HWFC           false // UART hardware flow control.

#endif
