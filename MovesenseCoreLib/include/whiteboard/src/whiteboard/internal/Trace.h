#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "whiteboard/Result.h"
#include "whiteboard/integration/port.h"

namespace whiteboard
{

void WbDebugCheckFail(const Result& rResult, const char* pFile, const size_t line);

inline Result WbDebugCheckResult(const Result& rResult, const char* pFile, const size_t line)
{
    // Check system event logging here, so that compiler can optimize constant check away
    if (WB_IS_SYSEVENT_ENABLED(WB_SYSEVENT_ID_API, WB_SYSEVENT_SEVERITY_WARNING))
    {
        if (rResult != HTTP_CODE_OK && rResult != HTTP_CODE_ACCEPTED &&
            rResult != HTTP_CODE_CONTINUE && rResult != HTTP_CODE_NO_CONTENT)
        {
            WbDebugCheckFail(rResult, pFile, line);
        }
    }

    return rResult;
}

#define WB_API_CHECK_RESULT(wb_api_call) WbDebugCheckResult(wb_api_call, __FILE__, __LINE__)

}
