#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

/** Infinite timeout (avoid overflow with ms to tick conversion) */
static const size_t WB_INFINITE = 1000000u; // TODO: Move to implementation so that API has single definition of WB_INFINITE
