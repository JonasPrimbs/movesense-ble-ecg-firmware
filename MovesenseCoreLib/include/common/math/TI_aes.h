/******************************************************************************

        Copyright (c) Suunto Oy 2011.
        All rights reserved.

        $Id$

******************************************************************************/

/** @file ti_aes.h
        Texas Instruments software AES implementation
*/
#ifndef TI_AES
#define TI_AES

void ti_aes_encrypt(unsigned char* state, unsigned char* key);
void ti_aes_decrypt(unsigned char* state, unsigned char* key);

#endif
