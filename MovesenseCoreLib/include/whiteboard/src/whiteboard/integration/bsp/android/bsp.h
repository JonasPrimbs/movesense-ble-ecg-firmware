#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <errno.h>

#ifndef UINT16_MAX
#define UINT16_MAX 65535
#endif

#include "debug.h"
#include "whiteboard/integration/bsp/shared/systemEventConfig.h"
#include "whiteboard/integration/bsp/shared/systemEvent.h"
