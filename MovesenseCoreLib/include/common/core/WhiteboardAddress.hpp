#pragma once

#include <cstring>
#include "common/core/dbgassert.h"

/// Strongly-typed wrapper for containing the Whiteboard device id.
/// For spec, see TP#20920. Globally unique.
struct WhiteboardAddress_t
{
    /// The Maximum length of a Whiteboard device id (in characters).
    static const int WB_DEVICE_ID_MAX_LENGTH = 16;

    /// The minimum length of a Whiteboard device id (in characters).
    static const int WB_DEVICE_ID_MIN_LENGTH = 1;

    /// Construct a new instance of Whiteboard device id.
    /// Caller is responsible for providing a valid id.
    /// @param _id Parameter length is checked against min and max length
    explicit WhiteboardAddress_t(const char* _id)
    {
        ASSERT(strlen(_id) <= WB_DEVICE_ID_MAX_LENGTH);
        ASSERT(strlen(_id) >= WB_DEVICE_ID_MIN_LENGTH);
#if _MSC_VER
        strcpy_s(id, _id);
#else
        strncpy(id, _id, WB_DEVICE_ID_MAX_LENGTH);
#endif
        id[WB_DEVICE_ID_MAX_LENGTH] = 0;
    }

    /// Construct a non-initialized Whiteboard device id.
    WhiteboardAddress_t() { id[0] = 0; }

    /// Convert whiteboard to null-terminated string
    const char* c_str() const { return id; }

    /// Check if Whiteboard id has been set.
    bool hasId() const { return strlen(id) <= WB_DEVICE_ID_MAX_LENGTH && strlen(id) >= WB_DEVICE_ID_MIN_LENGTH; }

    char id[WB_DEVICE_ID_MAX_LENGTH + 1];
};
