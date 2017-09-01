/******************************************************************************

        Copyright (c) Suunto Oy 2011.
        All rights reserved.

        $Id$

******************************************************************************/

/** @file tiaes.h
        interface functions and CBC mode to TI aes encrypt/decrypt
*/
#ifndef __TIAES_H
#define __TIAES_H

#include "common/compiler/genDef.h"
#include "common/compiler/genType.h"

// BLOCK_SIZE must be multiple of 2
#define AES_BLOCK_SIZE 16

typedef struct AESState
{
    uint8 iv[AES_BLOCK_SIZE];
    uint8 key[AES_BLOCK_SIZE];
    uint32 ctr;
    uint8 block[AES_BLOCK_SIZE];
    uint8 blockPos;
} AESState;

extern void aesCBCInit(AESState* pState, const uint8* iv, const uint8* key);

extern void aesCBCDecryptBlock(AESState* pState, uint8* pOutBuf, const uint8* pInBuf);
// size must be divisible by 16
extern void aesCBCDecrypt(AESState* pState, uint8* pOutBuf, const uint8* pInBuf, uint32 size);

extern void aesCTRInit(AESState* pState, const uint8* iv, const uint8* key);
extern void aesCTRReset(AESState* pState);
extern uint8 aesCTRProcessByte(AESState* pState, uint8 inputByte);
extern void aesCTRProcessBuffer(AESState* pState, uint8* pOutBuffer, const uint8* pInBuffer, uint32 size);

extern uint8 aesDummyCTRProcessByte(AESState* pState, uint8 inputByte);
extern void aesDummyCTRProcessBuffer(AESState* pState, uint8* pOutBuffer, const uint8* pInBuffer, uint32 size);

#endif
