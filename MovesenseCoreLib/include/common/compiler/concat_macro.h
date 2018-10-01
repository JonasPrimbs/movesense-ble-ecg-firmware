#pragma once

/**
 * Copyright Suunto Oy 2017
 */

/* Helper macros for C preprocessor token pasting */

#ifndef CONCAT
#define CONCAT2(a, b) a##b
#define CONCAT(a, b) CONCAT2(a, b)
#endif
