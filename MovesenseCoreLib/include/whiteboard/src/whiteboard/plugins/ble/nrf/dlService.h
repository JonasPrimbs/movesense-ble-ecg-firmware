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


void dlServiceInit( void );
bool dlServiceSend( const void* pData, uint16_t byteCount );
void dlServiceGetUuid( ble_uuid_t *dest );

extern uint16_t m_conn_handle;
extern ble_gatts_char_handles_t	charHandlesFromPeripheral;
extern ble_gatts_char_handles_t	charHandlesToPeripheral;

#endif
