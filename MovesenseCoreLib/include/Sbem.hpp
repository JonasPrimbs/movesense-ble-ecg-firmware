#pragma once
/******************************************************************************

Copyright (c) Suunto Oy 2015.
All rights reserved.

******************************************************************************/
/**
@file Sbem.hpp
*/

#include "common/compiler/genDef.h"
#include "sbem/sbemdescriptor.hpp"
#include "common/text/Text.hpp"
#include "filesystems/file.h"

static const char* SBEM_VERSION_HEADER("SBEM0103");
static const uint16 ATTRIBUTE_UNUSED SBEM_VERSION_HEADER_SIZE(static_cast<uint16>(strlen(SBEM_VERSION_HEADER)));

/**
sbem namespace.
*/
namespace sbem
{
    /**
    *   Write a data chunk to log.
    *
    *   @param chunkId A chunk id.
    *   @param outputBuffer buffer where the chunk is created.
    *   @param data1 Chunk data packet one (must not be NULL).
    *   @param size1 Size of data packet1 (must be >0).
    *   @param data2 Optional data packet two.
    *   @param size2 Optional size of data packet two.
    *   @param data3 Optional data packet three.
    *   @param size3 Optional size of data packet three.
    *
    *   @return length of created chunk in bytes
    */
    uint32 writeChunk(uint16 chunkId, uint8* outputBuffer, const void* data1, uint32 size1,
                      const void* data2 = NULL, uint32 size2 = 0U, const void* data3 = NULL,
                      uint32 size3 = 0U);

    /**
    *   Write a text chunk to log.
    *
    *   @param chunkId A chunk id.
    *   @param text A text.
    *   @param outputBuffer buffer where the chunk is created.
    *
    *   @return length of created chunk in bytes
    */
    uint32 writeChunk(uint16 chunkId, const nea::BText& text, uint8* outputBuffer);

    /**
    *   Write a cstring chunk to log.
    *
    *   @param chunkId A chunk id.
    *   @param pStr A cstring.
    *   @param outputBuffer buffer where the chunk is created.
    *
    *   @return length of created chunk in bytes
    */
    uint32 writeChunk(uint16 chunkId, const char* pStr, uint8* outputBuffer);

    /**
    *   Write a descriptor chunk to log.
    *
    *   @param chunkId A chunk id.
    *   @param pStr A descriptor string (0-terminated)
    *   @param outputBuffer buffer where the chunk is created.
    *
    *   @return length of created chunk in bytes
    */
    int writeDescriptorChunk(uint16 chunkId, const char* pStr, uint8* outputBuffer);

    int writeId(uint16 id, uint8* outputBuffer);
    int writeSize(uint32 sz, uint8* outputBuffer);
    int writeSizeFull(uint32 sz, uint8* outputBuffer);

    /**
    *   Parse id and len from chunk header
    *
    *   @param buffer for parsing. Minimium size is 8
    *   @param id of chuck
    *   @param len of chunk after header
    *
    *   @return length of header
    */
    int readChunkHeader(uint8 *buffer, uint32 &id, uint32 &len);

    /**
    *   Search next chunck by id
    *
    *   @param f is open file handle. Sbem "SBEM0103" is skipped automatically
    *   @param searchId is searched id
    *   @param dst target for chuck. Header is not copied.
    *   @param dstSize max amount of copied data.
    *
    *   @return true if id found
    */
    bool findNextChunk(FileHandle f, uint16 searchId, void * dst, uint32 dstSize);

} // namespace sbem
