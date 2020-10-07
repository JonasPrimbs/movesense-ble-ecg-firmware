#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/Initialization.h"

#include <map>
#include <string>

#if WB_UNITTEST_BUILD

namespace whiteboard
{

class DynamicExecutionContext;
class Whiteboard;
class WhiteboardCommunication;
class CommAdapterMockup;

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

    /** Terminates all event processing */
    void terminate();

    /** Implicit cast operator for using this class in place of any Whiteboard */
    operator Whiteboard&();

#ifdef WB_HAVE_COMM
    /**
    * Gets associated communication class instance
    *
    * @return Whiteboard communication instance
    */
    WhiteboardCommunication& getCommunication();

    /* Connect to another Whiteboard */
    WhiteboardId connect(Whiteboard& whiteboard);
    /** Disconnect from another Whiteboard */
    void disconnect(Whiteboard& whiteboard);
#endif

    /** Gets simulation execution context of the whiteboard */
    whiteboard::ExecutionContextId getExecutionContextId();

private:
    whiteboard::Whiteboard* mpWhiteboard;
    whiteboard::DynamicExecutionContext* mpExecutionContext;

#ifdef WB_HAVE_COMM
    whiteboard::CommAdapterMockup* mpLocalCommAdapter;
    typedef std::map<std::string, whiteboard::CommAdapterMockup*> RemoteAdapterStore;
    RemoteAdapterStore mpRemoteCommAdapters;
#endif
};

} // namespace whiteboard

#endif
