//
//  WBDefinesObjC
//  Whiteboard
//
//  Created by Nikita Zhuk on 02/07/15.
//  Copyright (c) 2015 Suunto Oy. All rights reserved.
//

/**
   Macro definitions for pure ObjC code
 */

#ifndef WBDEFINES_OBJC_H
#define WBDEFINES_OBJC_H

// Undef explicitly to avoid compiler warnings about redefined macros
#if defined(WB_DEBUGLOG)
#undef WB_DEBUGLOG
#undef WB_ASSERT
#endif

#define WB_DEBUGLOG(format, ...) NSLog(format, ##__VA_ARGS__)
#define WB_ASSERT(cond) NSAssert(cond, @"")

#endif
