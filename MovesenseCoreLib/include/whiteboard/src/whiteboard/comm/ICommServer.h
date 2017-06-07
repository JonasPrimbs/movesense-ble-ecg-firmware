#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/Result.h"

namespace whiteboard
{

/** Interface for server instances that act as access points to
 * actual connections
 */
class WB_API ICommServer
{
protected:
    /** Communication server instances should be destroyed through Communication class. */
    friend class Communication;

    /** Destructor. */
    virtual ~ICommServer() {}

public:
    /**
    *   Gets name of server
    *
    *   @return Name of the server
    */
    virtual const char* getName() const = 0;

    /** Enables the server
     *
     * @return Result of the operation
     */
    virtual Result enable() = 0;

    /** Disables the server
    *
    * @return Result of the operation
    */
    virtual Result disable() = 0;
};

} // namespace whiteboard

