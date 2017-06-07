#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/Initialization.h"

#if WB_UNITTEST_BUILD

namespace whiteboard
{

class DynamicExecutionContext;
class Whiteboard;
class WhiteboardCommunication;

// Custom whiteboard for having multiple whiteboard instances
// and doing some simple requests
class WhiteboardMockup
{
public:
    /** Constructor 
     *
     * @param serialNumber Serial number of Whiteboard
     * @param rConfiguration Configuration
     */
    WhiteboardMockup(const char* serialNumber, const whiteboard::Config& rConfiguration);
    
    /** Destructor */
    ~WhiteboardMockup();

    /** Implicit cast operator for using this class in place of any Whiteboard */
    operator Whiteboard&();

    /**
    * Gets associated communication class instance
    *
    * @return Whiteboard communication instance
    */
    WhiteboardCommunication& getCommunication();

    /** Gets simulation execution context of the whiteboard */
    whiteboard::ExecutionContextId getExecutionContextId();

private:
    whiteboard::Whiteboard* mpWhiteboard;
    whiteboard::DynamicExecutionContext* mpExecutionContext;
};

} // namespace whiteboard

#endif
