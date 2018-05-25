#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include <gtest/gtest.h>
#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"

#include "whiteboard/comm/internal/RoutingTable.h"

struct RoutingTableInfo
{
    const char* serialNumber;
    whiteboard::HopCount numberOfHops;
    const char* nextHopSerialNumber;
    whiteboard::CommAdapter* pInterface;
    size_t address;
    const whiteboard::WhiteboardVersion& whiteboardVersion;
};

inline bool equalAddresses(size_t index, const whiteboard::Address& rAddress)
{
    size_t otherIndex = *reinterpret_cast<const size_t*>(&rAddress);
    return (index == otherIndex) && (memcmp(&whiteboard::NULL_ADDRESS[sizeof(size_t)],
                                            &rAddress[sizeof(size_t)],
                                            sizeof(whiteboard::Address) - sizeof(size_t)) == 0);
}

class RoutingTableChecker
{
public:
    template <size_t COUNT>
    static bool check(whiteboard::RoutingTable& rRoutingTable, const RoutingTableInfo(&rExpectedRoutes)[COUNT])
    {
        bool result = true;

        for (size_t i = 0; i < COUNT; ++i)
        {
            const RoutingTableInfo& rExpectedRoute = rExpectedRoutes[i];

            bool found = false;
            for (whiteboard::RoutingTable::EntryPool::Iterator j = rRoutingTable.begin(); j.hasMore(); ++j)
            {
                whiteboard::RoutingTableEntry& rEntry = *j;

                if ((rEntry.serialNumber == rExpectedRoute.serialNumber) &&
                    (rExpectedRoute.numberOfHops == rEntry.numberOfHops) &&
                    (rExpectedRoute.whiteboardVersion == rEntry.whiteboardVersion) &&
                    (((rExpectedRoute.numberOfHops == 0) &&
                        (rExpectedRoute.pInterface == rEntry.destination.pInterface) &&
                        equalAddresses(rExpectedRoute.address, rEntry.destination.address))
#ifdef WB_HAVE_COMM_ROUTING
                        ||
                        ((rExpectedRoute.numberOfHops != 0) && 
                        (rEntry.nextHop.routingTableEntryHandle.pEntry->serialNumber == rExpectedRoute.nextHopSerialNumber))
#endif
                    )
                    )
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                result = false;
                break;
            }
        }

        for (whiteboard::RoutingTable::EntryPool::Iterator j = rRoutingTable.begin(); j.hasMore(); ++j)
        {
            whiteboard::RoutingTableEntry& rEntry = *j;

            bool found = false;
            for (size_t i = 0; i < COUNT; ++i)
            {
                const RoutingTableInfo& rExpectedRoute = rExpectedRoutes[i];
                if ((rEntry.serialNumber == rExpectedRoute.serialNumber) &&
                    (rExpectedRoute.numberOfHops == rEntry.numberOfHops) &&
                    (rExpectedRoute.whiteboardVersion == rEntry.whiteboardVersion)
                    &&
                    (((rExpectedRoute.numberOfHops == 0) &&
                        (rExpectedRoute.pInterface == rEntry.destination.pInterface) &&
                        equalAddresses(rExpectedRoute.address, rEntry.destination.address))
#ifdef WB_HAVE_COMM_ROUTING
                        ||
                        ((rExpectedRoute.numberOfHops != 0) && 
                        (rEntry.nextHop.routingTableEntryHandle.pEntry->serialNumber == rExpectedRoute.nextHopSerialNumber))
#endif
                    )
                    )
                {
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                result = false;
                break;
            }
        }

        return result;
    }

    static bool checkCount(whiteboard::RoutingTable& rRoutingTable, const size_t expectedCount)
    {
        size_t numberOfRoutes = 0;

        for (whiteboard::RoutingTable::EntryPool::Iterator j = rRoutingTable.begin(); j.hasMore(); ++j)
        {
            ++numberOfRoutes;
        }

        return numberOfRoutes == expectedCount;
    }
};

#define EXPECT_EQ_ROUTES(routingTable, routes)                                                                                   \
    do                                                                                                                           \
    {                                                                                                                            \
        bool checkResult = RoutingTableChecker::check(routingTable, routes);                                                     \
        EXPECT_TRUE(checkResult);                                                                                                \
    } while (0)

#define EXPECT_EQ_ROUTE_COUNT(routingTable, count)                                                                               \
    do                                                                                                                           \
    {                                                                                                                            \
        bool checkResult = RoutingTableChecker::checkCount(routingTable, count);                                                 \
        EXPECT_TRUE(checkResult);                                                                                                \
    } while (0)
