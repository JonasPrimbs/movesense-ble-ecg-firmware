#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Identifiers.h"

namespace whiteboard
{

// Forward declarations
struct RoutingTableEntry;

/** Handle to routing table entry */
struct WB_API RoutingTableEntryHandle
{
    /** Routing table entry handle that is invalid */
    static const RoutingTableEntryHandle Invalid;

    union
    {
        struct
        {
            /** ID of remote Whiteboard */
            WhiteboardId remoteWhiteboardId;

            /** Version of the routing table entry */
            uint8 version;
        };

        /** Primitive type that stores RoutingTableEntryHandle information */
        uint16 value;
    };

    /** Checks two routing table entry handles for equality
    *
    * @param rOtherP Another routing table entry handle to check
    * @return A value indicating whether the two handles are equal
    */
    inline bool operator==(const RoutingTableEntryHandle& rOtherP) const;

    /** Checks two routing table entry handles for inequality
    *
    * @param rOtherP Another routing table entry handle to check
    * @return A value indicating whether the two handles are inequal
    */
    inline bool operator!=(const RoutingTableEntryHandle& rOtherP) const;
};

inline bool RoutingTableEntryHandle::operator==(const RoutingTableEntryHandle& rOtherP) const
{
    return this->value == rOtherP.value;
}

inline bool RoutingTableEntryHandle::operator!=(const RoutingTableEntryHandle& rOtherP) const
{
    return !(*this == rOtherP);
}

} // namespace whiteboard
