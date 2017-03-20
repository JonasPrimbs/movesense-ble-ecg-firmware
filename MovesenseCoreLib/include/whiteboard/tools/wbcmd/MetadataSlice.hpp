#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2016.
    All rights reserved.
******************************************************************************/
#include <whiteboard/integration/port.h>

namespace wbcmd
{

/** A simple wrapper around the data read from metadata bytestream.
 * Can be used to provide correct alingment when using MetadataStreamParser::deserializeFromBuffer. */
class SliceOfMetadata final
{
    static const int ALIGNMENT_MAGIC = 9;
public:
    SliceOfMetadata(const uint8_t *data_, const size_t size_):
        data(static_cast<uint8_t*>(WbMemAllocAligned<8>(size_ + ALIGNMENT_MAGIC))),
        size(size_)
    {
        memcpy(data + ALIGNMENT_MAGIC, data_, size_);
    }
    SliceOfMetadata(const SliceOfMetadata& o) = delete;
    SliceOfMetadata& operator=(const SliceOfMetadata& o) = delete;
    SliceOfMetadata(SliceOfMetadata&& o) noexcept : data(std::move(o.data)), size(o.size)
    {
        o.data = nullptr;
    }

    ~SliceOfMetadata()
    {
        if(data)
            WbMemFreeAligned<8>(data);
    }
    const uint8_t *getData() const { return data + ALIGNMENT_MAGIC; }
    uint8_t *data;
    const size_t size;
};

} // namespace wbcmd
