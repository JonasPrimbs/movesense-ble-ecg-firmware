#pragma once
#include <stdint.h>

typedef uint8_t utf8;

/*
 * utf8_strlen returns number of characters in the string. normal strlen returns the number of bytes
 * in string
 */
int utf8_strlen(const utf8* pStr);

/*
 * utf8_strbytelen is analogous to strlen
 */
int utf8_strbytelen(const utf8* pStr);

/*
 * utf8_chrlen returns number of bytes (1-6) in utf8 encoded character
 */
int utf8_chrlen(const utf8* pStr);

/*
 * decodes utf8 character into 32bit unicode
 */
uint32_t utf8_chrdecode(const utf8* pStr);

/*
 * utf-8 decoding example:
 *
 * char* pString //contains utf-8 encoded string
 *
 * while(*pString)
 * {
 *    uint32 char = utf8_chrdecode(pString);
 *    pString += utf8_chrlen(pString);
 * }
 */

