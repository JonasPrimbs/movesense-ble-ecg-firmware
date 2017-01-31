#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"

namespace whiteboard
{

WB_PACK_BEGIN()

/** Structure that defines whiteboard version number */
struct WB_PACKED WhiteboardVersion
{
    uint8 majorVersion;
    uint8 minorVersion;
    uint8 patchVersion;
    uint8 reserved;
    uint8 hash[4];

    /** Default ctor */
    inline WhiteboardVersion() :
        majorVersion(0),
        minorVersion(0),
        patchVersion(0),
        reserved(0)
    {
        for (size_t i = 0; i < sizeof(hash); ++i) 
        { // use loop to set to bypass header dependency with stdlibraries
            this->hash[i] = 0;
        }
    }

    /** @return true if version number is valid (unequal to 0.0.0) */
    inline bool isValid() const
    {
        if ((this->majorVersion != 0) ||
            (this->minorVersion != 0) ||
            (this->patchVersion != 0))
        {
            return true;
        }
        return false;
    }

    /** Logical equality operator: does not check has equality */
    inline bool operator==(const WhiteboardVersion& rhs) const 
    {
        if ((this->majorVersion == rhs.majorVersion) && (this->minorVersion == rhs.minorVersion) && (this->patchVersion == rhs.patchVersion))
        { // don't care about the hash
            return true;
        }
        return false;
    }

    /** Assignment operator */
    inline WhiteboardVersion& operator=(const WhiteboardVersion& rhs)
    {
        if (this != &rhs)
        {
            this->majorVersion = rhs.majorVersion;
            this->minorVersion = rhs.minorVersion;
            this->patchVersion = rhs.patchVersion;
            for (size_t i = 0; i < sizeof(hash); ++i)
            { // use loop to set to bypass header dependency with stdlibraries
                this->hash[i] = rhs.hash[i];
            }
        }
        return *this;
    }

};

/** @return current version information of this whiteboard build, set at compile time. 
    @note: the first call is not thread-safe (returns static struct that is created on first call). 
*/
const WhiteboardVersion& GetWhiteboardVersion();

WB_PACK_END()

} // namespace whiteboard
