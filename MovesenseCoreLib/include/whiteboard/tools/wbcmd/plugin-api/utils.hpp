#pragma once
/******************************************************************************

    Copyright (c) Suunto Oy 2015.
    All rights reserved.

******************************************************************************/

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace utils
{

template<class TContainer, class Ty>
bool exists(const Ty &val, const TContainer &v)
{
    return std::find(v.begin(), v.end(), val) != v.end();
}

template<class TContainer, class TComparator>
bool exists_if(const TComparator &comp, const TContainer &v)
{
    return std::find_if(v.begin(), v.end(), comp) != v.end();
}

inline std::ifstream::pos_type filesize(const std::string& filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

/**
* @brief Left Trim
*
* Trims whitespace from the left end of the provided std::string
*
* @param[out] s The std::string to trim
*
* @return The modified std::string&
*/
template<class TString, typename TValidChar>
TString& ltrim(TString& s, TValidChar validChar) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        std::ptr_fun<int, int>(validChar)));
    return s;
}

/**
* @brief Right Trim
*
* Trims whitespace from the right end of the provided std::string
*
* @param[out] s The std::string to trim
*
* @return The modified std::string&
*/
template<class TString, typename TValidChar>
TString& rtrim(TString& s, TValidChar validChar) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
        std::ptr_fun<int, int>(validChar)).base(), s.end());
    return s;
}

/**
* @brief Trim
*
* Trims whitespace from both ends of the provided std::string
*
* @param s The std::string to trim
*
* @return The modified std::string
*/
template<int(*ValidChar)(int) = std::isgraph, class TString>
TString trim(TString s) {
    return ltrim(rtrim(s, ValidChar), ValidChar);
}

/** Helper function for converting string to lower case
*
* @param str String to convert
* @return Lower case string
*/
inline std::string tolower(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

/** Helper function for converting string to upper case
*
* @param str String to convert
* @return Lower case string
*/
inline std::string toupper(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

template <typename T> inline std::vector<T> split(const T& str, const T& delimiters)
{
    std::vector<T> v;
    typename T::size_type start = 0;
    auto pos = str.find_first_of(delimiters, start);
    while (pos != T::npos) {
        if (pos != start) // ignore empty tokens
            v.emplace_back(str, start, pos - start);
        start = pos + 1;
        pos = str.find_first_of(delimiters, start);
    }
    if (start < str.length())                             // ignore trailing delimiter
        v.emplace_back(str, start, str.length() - start); // add what's left of the string
    return v;
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::false_type, Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_helper(std::true_type, Args&&... args)
{
    static_assert(std::extent<T>::value == 0,
        "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");

    typedef typename std::remove_extent<T>::type U;
    return std::unique_ptr<T>(new U[sizeof...(Args)]{ std::forward<Args>(args)... });
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return make_unique_helper<T>(std::is_array<T>(), std::forward<Args>(args)...);
}

inline bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

template <
    class InputIterator, class OutputIterator,
    class UnaryOperator, class Pred
>
OutputIterator transform_if(InputIterator first1, InputIterator last1,
    OutputIterator result, UnaryOperator op, Pred pred)
{
    while (first1 != last1)
    {
        if (pred(*first1)) {
            *result = op(*first1);
            ++result;
        }
        ++first1;
    }
    return result;
}

template<class Ty1, class Ty2>
void append_to(Ty1 &to, const Ty2 &from)
{
    to.insert(std::end(to), std::begin(from), std::end(from));
}

inline void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

} // namespace utils
