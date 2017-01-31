#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard
{

// Forward declarations
struct RoutingTableEntry;

/** Handle to routing table entry */
struct WB_API RoutingTableEntryHandle
{
    /** Routing table entry that is invalid */
    static const RoutingTableEntryHandle Invalid;

    /** Actual routing table entry */
    RoutingTableEntry* pEntry;

    /** Version of the routing table entry */
    uint8 version;

    /** Checks whether the routing table entry has been reassigned
    *
    * @return A value indicating whether the entry has been reassigned
    */
    bool hasBeenReassigned() const;

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
    return (this->pEntry == rOtherP.pEntry) && (this->version == rOtherP.version);
}

inline bool RoutingTableEntryHandle::operator!=(const RoutingTableEntryHandle& rOtherP) const
{
    return !(*this == rOtherP);
}

} // namespace whiteboard
