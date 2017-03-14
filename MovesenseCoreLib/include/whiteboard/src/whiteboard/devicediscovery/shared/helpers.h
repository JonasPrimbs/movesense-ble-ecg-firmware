#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <algorithm>
#include <string>
#include <vector>

template<class TContainer, class TComparator>
bool exists_if(const TComparator &comp, const TContainer &v)
{
    return std::find_if(v.begin(), v.end(), comp) != v.end();
}

/** Splits given string to tokens that are separated by given separator
 *
 * @param str String to split
 * @param separator Separator string
 * @return List of tokens
 */
std::vector<std::string> tokenize(const std::string& str, const std::string& separator);

/** Helper function for converting string to lower case
*
* @param str String to convert
* @return Lower case string
*/
std::string tolower(const std::string& str);

/** Helper function for converting string to upper case
*
* @param str String to convert
* @return Upper case string
*/
std::string toupper(const std::string& str);
