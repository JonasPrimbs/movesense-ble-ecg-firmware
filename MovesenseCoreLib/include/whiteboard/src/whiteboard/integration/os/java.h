// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "whiteboard/integration/port.h"

#ifdef WB_HAVE_JAVA_INTERFACE

/** Attaches calling thread to Java virtual machine
 *
 * @note These functions are called by system for every execution context that is configured
 *       with 'javaSupport' option set to true. There is no need to call these functions
 *       multiple times.
 * @return A value indicating whether thread was successfully attached
 */
bool WbJavaAttachThread();

/** Detaches calling thread from Java virtual machine */
void WbJavaDetachThread();

#endif

