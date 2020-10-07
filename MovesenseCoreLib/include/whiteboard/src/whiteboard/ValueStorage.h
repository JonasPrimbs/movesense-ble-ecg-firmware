#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/Value.h"
#ifdef WB_HAVE_HASH_STRING
#include "whiteboard/builtinTypes/HashString.h"
#endif

namespace whiteboard
{

// Forward declarations
class ValueStorageAccessor;

/**
Generic storage class for primitive datatypes (scalar types & strings) for 
Value-objects. Can be be used in normal API functions.

@see whiteboard::Value
*/
class ValueStorage
{
public:

    /**
    Ctor that takes byte-array for extra storage in. 
    Use only if you want to store strings in addition to scalar types.

    @param pStorage [in] memory area for the string storage.
    */
    template <size_t SIZE>
    EXPLICIT ValueStorage(uint8(&pStorage)[SIZE])
      : mValueType(WB_TYPE_NONE)
      , mpStorage(pStorage)
      , mStorageSize(SIZE)
    {
    }

    /**
    Ctor that takes memory area for extra storage.

    @param pStorage [in] memory area for the string storage.
    @size [in] Size of the given memory area.
    */
    ValueStorage(uint8* pStorage, size_t size)
        : mValueType(WB_TYPE_NONE)
        , mpStorage(pStorage)
        , mStorageSize(size)
    {
    }

    /**
    * Default constructor for value storage
    *
    * @note This does not allocate memory for storing string values (string literals can still be used)
    */
    ValueStorage()
        : mValueType(WB_TYPE_NONE)
        , mpStorage(NULL)
        , mStorageSize(0)
    {
    }

    /**
    * Constructor for value storage that is initialized with specific value.
    * @note These do not allocate memory for storing string values (string literals can still be used)
    * @param data The initialization value
    */
    ValueStorage(const NoType&);
    ValueStorage(bool data);
    ValueStorage(int8 data);
    ValueStorage(uint8 data);
    ValueStorage(int16 data);
    ValueStorage(uint16 data);
    ValueStorage(int32 data);
    ValueStorage(uint32 data);
    ValueStorage(int64 data);
    ValueStorage(uint64 data);
    ValueStorage(float data);
    ValueStorage(double data);
#ifdef WB_HAVE_HASH_STRING
    ValueStorage(HashString data);
#endif
    /** Constructor for initializing value storage with string literal (or const char array)
    *
    * @tparam N Length of the string literal including zero terminator
    * @param data The initialization value
    */
    template <size_t N>
    ValueStorage(const char (& data) [N])
        : mValueType(WB_TYPE_NONE)
        , mpStorage(NULL)
        , mStorageSize(0)
    {
        operator=<N>(data);
    }

    /// Dtor
    ~ValueStorage()
    {
    }

    /// @return The type of the value this ValueStorage stores currently.
    ValueType getType() const
    {
        return mValueType;
    }

    /**
    Assignment operator to store scalar type or string or to copy whiteboard::Value to this value 
    storage.

    @param data [in] Data to assing to this ValueStorage.
    @return *this
    */
    ValueStorage& operator=(const NoType&);
    ValueStorage& operator=(bool data);
    ValueStorage& operator=(int8 data);
    ValueStorage& operator=(uint8 data);
    ValueStorage& operator=(int16 data);
    ValueStorage& operator=(uint16 data);
    ValueStorage& operator=(int32 data);
    ValueStorage& operator=(uint32 data);
    ValueStorage& operator=(int64 data);
    ValueStorage& operator=(uint64 data);
    ValueStorage& operator=(float data);
    ValueStorage& operator=(double data);
#ifdef WB_HAVE_HASH_STRING
    ValueStorage& operator=(HashString data);
#endif
    /** Assignment operator that stores string literal (or const char array) in this value storage
    *
    * @tparam N Length of the string literal including zero terminator
    * @param data The value
    */
    template <size_t N>
    inline ValueStorage& operator=(const char (& data)[N])
    {
        mValueType = WB_TYPE_STRING;
        mValue.mString = const_cast<char*>(data);
        return *this;
    }

    ValueStorage& operator=(const char* const data);
    ValueStorage& operator=(const Value& data);

    bool operator==(const Value& data) const;

    /**
    Converts this ValueStorage to native type.

    @tparam T [in] Type to convert to; const char* or any scalar type up to int64 and double
    */
    template <typename T> inline T convertTo() const
    {
        return ValueStorage::ResultType<T>::convertTo(convertToNative<typename ResultType<T>::NativeType>());
    }

private:
    // Prevent use of constructors that would need dynamically allocated storage
    template <size_t N>
    ValueStorage(char(&data)[N]) DELETED;
    template <typename T>
    ValueStorage(T, typename EnableIf<IsSame<T, char*>::value>::type* = 0) DELETED;
    template <typename T>
    ValueStorage(T, typename EnableIf<IsSame<T, const char*>::value>::type* = 0) DELETED;

    /** Generalized result type (for TypedEnums) */
    template <typename T> struct ResultType
    {
        typedef T NativeType;
        inline static T convertTo(T value)
        {
            return value;
        }
    };

    /**
    Converts the ValueStorage object to native value.

    @tparam Native type to convert to
    @return native value
    */
    template <typename T>
    T convertToNative() const;

    friend class whiteboard::Value;
    friend class whiteboard::ValueStorageAccessor;

    /// @see whiteboard::ValueType
    ValueType mValueType;

    /// The stored value (union), type determined by mValueType
    union
    {
        bool mBool;
        int8 mInt8;
        uint8 mUint8;
        int16 mInt16;
        uint16 mUint16;
        int32 mInt32;
        uint32 mUint32;
        int64 mInt64;
        uint64 mUint64;
        float mFloat;
        double mDouble;
        char* mString;
#ifdef WB_HAVE_HASH_STRING
        HashString::ValueType mHash;
#endif
    } mValue;

    /// The extra storage for strings
    uint8* const mpStorage;

    /// Size of the extra storage
    const size_t mStorageSize;
};

// Specialization for TypedEnum
template <typename Definition, typename DefinitionType, typename BaseType>
struct ValueStorage::ResultType<whiteboard::TypedEnum<Definition, DefinitionType, BaseType> >
{
    typedef BaseType NativeType;
    typedef whiteboard::TypedEnum<Definition, DefinitionType, BaseType> EnumType;

    static inline EnumType convertTo(BaseType value)
    {
        return EnumType(static_cast<DefinitionType>(value));
    }
};

}
