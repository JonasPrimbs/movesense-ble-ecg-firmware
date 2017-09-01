// Copyright (c) Suunto Oy 2015. All rights reserved.

/**
	Interface for ble WB data link service.
*/
#ifndef DL_SERVICE_H_
#define DL_SERVICE_H_

extern "C"
{
	#include "ble.h" 
}


// if USE_16BIT_VENDOR_UUIDS defined, use 16bit uuids instead of the real 128bit ones
#ifdef USE_16BIT_VENDOR_UUIDS
    #error "USE_16BIT_VENDOR_UUIDS Not implemented"
//	#define UUID_WB_SERVICE				0xFFF0
//	#define UUID_WB_TO_PERIPHERAL_CHAR     0xFFF1
//	#define UUID_WB_FROM_PERIPHERAL_CHAR   0xFFF2
#else
    // 128bit uuids in little endian format
    #define UUID_WB_SERVICE                { 0x41, 0x00, 0x74, 0x70, 0x63, 0x88, 0x7a, 0xb5, 0xcc, 0x49, 0x31, 0x82, 0x90, 0x30, 0x35, 0x61 }
    #define UUID_WB_TO_PERIPHERAL_CHAR     { 0x85, 0xfa, 0xe5, 0x61, 0xee, 0x3a, 0x9f, 0x90, 0x7f, 0x41, 0x52, 0x56, 0x57, 0x65, 0x81, 0x17 }
    #define UUID_WB_FROM_PERIPHERAL_CHAR   { 0xf0, 0xe8, 0x50, 0x70, 0x0e, 0x63, 0x31, 0xb4, 0x5d, 0x4d, 0x85, 0x71, 0x52, 0x22, 0x80, 0x34 }
#endif


void dlServiceInit( void );
bool dlServiceSend( const void* pData, uint16_t byteCount );
void dlServiceGetUuid( ble_uuid_t *dest );

extern uint16_t m_conn_handle;
extern ble_gatts_char_handles_t	charHandlesFromPeripheral;
extern ble_gatts_char_handles_t	charHandlesToPeripheral;

#endif
