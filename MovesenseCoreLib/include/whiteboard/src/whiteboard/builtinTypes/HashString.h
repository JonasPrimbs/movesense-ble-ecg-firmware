#pragma once
// Copyright (c) Suunto Oy 2018. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/integration/shared/Ascii7String.h"

#if defined(WB_HAVE_HASH_STRING)

namespace whiteboard
{

/** Hash string wrapper class using FNV-1 (32-bit) hash algorithm */
class HashString
{
public:
    /** Convenience type to make code more clearer */
    typedef uint32 ValueType;

#ifdef WB_HAVE_CPLUSPLUS_11

    /** Wrapper structure that helps compiler to determine whether
     * to use compile time or runtime operations
     */
    struct Wrapper
    {
        /** Constructor 
         *
         * @param str String value
         */
        Wrapper(const char* str) : mStr(str) {}
        
        /** Wrapped string value */
        const char* mStr;
    };

    /** Initializes a HashString instance at run-time
    *
    * @param wrapper Wrapper instance for string
    */
    HashString(const Wrapper wrapper)
        : mValue(hash(wrapper.mStr)) {}
    
    /** Initializes a HashString instance at compile-time
    *
    * @tparam N Length of the string
    * @param str String for which the hash should be calculated
    */
    template <size_t N> constexpr HashString(const char(&str)[N])
        : mValue(hash<N>(str, N - 1)) {}
#else
    /** Initializes a HashString instance at run-time
    *
    * @param str String for which the hash should be calculated
    */
    HashString(const char* str)
        : mValue(hash(str)) {}
#endif

    /** Initializes hashstring for empty string */
    CONSTEXPR HashString() : mValue(OFFSET_BASIS) {}

    /** Initializes hashstring with hash */
    CONSTEXPR HashString(HashString::ValueType hashValue) : mValue(hashValue) {}

    /** Gets the calculated hash value
     *
     * @return Calculated hash value
     */
    CONSTEXPR operator ValueType() const { return mValue; }

    /**
    * Calculates hash for zero terminated string at runtime
    *
    * @param str String for which the hash should be calculated
    * @return Hash value
    */
    static ValueType hash(const char* str, ValueType startValue = OFFSET_BASIS);

    /**
    * Calculates hash for zero terminated string at runtime, converting the string to lower case first.
    *
    * @param str String for which the hash should be calculated
    * @return Hash value
    */
    static ValueType hashLc(const char* str, ValueType startValue = OFFSET_BASIS);

    /**
    * Calculates hash for string at runtime
    *
    * @param str String for which the hash should be calculated
    * @param len Length of the string
    * @return Hash value
    */
    static ValueType hashN(const char* str, size_t len, ValueType startValue = OFFSET_BASIS);

    /**
    * Calculates hash for string at runtime, converting string to lower case first.
    *
    * @param str String for which the hash should be calculated
    * @param len Length of the string
    * @return Hash value
    */
    static ValueType hashLcN(const char* str, size_t len, ValueType startValue = OFFSET_BASIS);

#ifdef WB_HAVE_CPLUSPLUS_11

    /**
    * Calculates hash for zero terminated string (or char array) at compile time
    *
    * @tparam N Length of the string
    * @param str String for which the hash should be calculated
    * @param offset Current character offset in the string
    * @return Hash value
    */
    template <size_t N>
    static constexpr ValueType hash(const char(&str)[N], size_t offset = N - 1)
    {
        // uint64 cast here is to avoid "warning C4307: '*' integral constant overflow". Use of 64-bit operation
        // does not matter here, because we are doing this at compile time.
        return offset == 0 ? OFFSET_BASIS : 
            static_cast<ValueType>((hash<N>(str, offset - 1) * static_cast<uint64>(FNV_PRIME)) ^ str[offset - 1]);
    }

    /// @see HashString::hash compile time hash for strings for path parameter hash comparison
    template <size_t N>
    static constexpr ValueType hashLc(const char(&str)[N], size_t offset = N - 1)
    {
        // uint64 cast here is to avoid "warning C4307: '*' integral constant overflow". Use of 64-bit operation
        // does not matter here, because we are doing this at compile time.
        return offset == 0 ? OFFSET_BASIS :
            static_cast<ValueType>((hashLc<N>(str, offset - 1) * static_cast<uint64>(FNV_PRIME)) ^ Ascii7String::caseLow(str[offset - 1]));
    }

#endif

    /** Prime number constant used in hashing */
    static CONSTEXPR_DATA ValueType FNV_PRIME = 0x01000193;

    /** Hash base offset constant */
    static CONSTEXPR_DATA ValueType OFFSET_BASIS = 0x811c9dc5;

private:
    /** Hash value */
    ValueType mValue;
};

#ifdef WB_HAVE_CPLUSPLUS_11

/** Convenience function for calculating hashes at compile time
*
* @tparam N Length of the string
* @param str String for which the hash should be calculated
* @return Calculated hash value
*/
template <size_t N>
inline constexpr HashString::ValueType Hash(const char(&str)[N])
{
    return HashString::hash<N>(str, N - 1);
}

/** Convenience function for calculating path parameter hashes at compile time.
*
* @tparam N Length of the string
* @param str String for which the hash should be calculated (will be converted to lower case)
* @return Calculated hash value
*/
template <size_t N>
inline constexpr HashString::ValueType PathParamHash(const char(&str)[N])
{
    return HashString::hashLc<N>(str, N - 1);
}

#endif // WB_HAVE_CPLUSPLUS_11
} // namespace whiteboard
#endif // WB_HAVE_HASH_STRING

