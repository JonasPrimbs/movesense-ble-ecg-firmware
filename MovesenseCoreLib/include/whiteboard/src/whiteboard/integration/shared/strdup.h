#pragma once
// Copyright (c) Suunto Oy 2019. All rights reserved.

/**
 * Create a copy of the input string.
 * Uses new operator to reserve the memory. The memory needs to be released
 * using delete[] operator
 *
 * @param str		input string
 * @returns pointer to copied string. NULL if operation did not succeed.
 */
char* wbstrdup(const char* str);
