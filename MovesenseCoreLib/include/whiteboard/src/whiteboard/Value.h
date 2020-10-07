#pragma once
// Copyright (c) Suunto Oy 2014. All rights reserved.

#include "whiteboard/integration/port.h"
#include "whiteboard/builtinTypes/Structures.h"
#include "whiteboard/builtinTypes/TypedEnum.h"
#include "whiteboard/builtinTypes/WrapperFor32BitPointer.h"
#include "quantity/quantity/quantity.h"

// TODO: These are not required by Value class header,
// but are here so that library users don't need to include
// everything. Rethink when library interface is refactored.
#include "whiteboard/builtinTypes/Date.h"
#include "whiteboard/builtinTypes/DateTime.h"
#include "whiteboard/builtinTypes/ByteStream.h"
#ifdef WB_HAVE_HASH_STRING
#include "whiteboard/builtinTypes/HashString.h"
#endif
#include "whiteboard/builtinTypes/Time.h"
#include "whiteboard/builtinTypes/Timestamp.h"
#include "whiteboard/builtinTypes/Vector2D.h"
#include "whiteboard/builtinTypes/Vector3D.h"
#include "whiteboard/builtinTypes/NoType.h"

namespace whiteboard
{

// Forward declarations
class IValueSerializer;
#ifdef WB_HAVE_ALIEN_STRUCTURES
class AlienStructure;
#endif
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
class UnknownStructure;
#endif
class ValueAccessor;
class ValueStorage;

/** Wrapper for accessing binary data of Whiteboard API calls. */
class WB_API Value
{
public:
    /** Empty value */
    static const Value Empty;

    /** Constructor for null type */
    inline Value();

    /** Constructor that initializes value from string
    *
    * @param value String value
    */
    EXPLICIT inline Value(char* value);

    /** Constructor that initializes value from const string
    *
    * @param value String value
    */
    EXPLICIT inline Value(const char* value);

#ifdef WB_HAVE_UNKNOWN_STRUCTURES
    /** Constructor for creating value of type UnknownStructure
    *
    * @param rValue UnknownStructure instance
    */
    EXPLICIT Value(const UnknownStructure& rValue);
#endif

#ifdef WB_HAVE_ALIEN_STRUCTURES
    /** Constructor for creating value of type AlienStructure
    *
    * @param rValue AlienStructure instance constructed from semi-structured data like JSON
    */
    EXPLICIT Value(const AlienStructure& rValue);
#endif

    /** Constructor for creating value from whiteboard::ValueStorage .
    *
    * @param rValueStorage ValueStorage instance used to construct this Value instance.
    */
    EXPLICIT Value(const ValueStorage& rValueStorage);

    /** Constructor that initializes value from reference of
     * a native type
     *
     * @param rValue Reference to the value
     */
    template <typename NativeType> EXPLICIT inline Value(const NativeType& rValue);

    /** Constructor for creating value from another value
    *
    * @param value Source instance to copy content from.
    */
    Value(const Value& value)
    {
        assign(value);
    }

    /** Destructor */
    inline ~Value();

    /** Copies value from another object. Only use if you know what you are doing :) */
    inline void assign(const Value& that) { *this = that; }

    /** Gets the type of the value
    *
    * @return Type of the value
    */
    inline ValueType getType() const;

    /** Gets the value type of by given data type
     *
     * @return Value type of data type
     */
    static inline ValueType getType(LocalDataTypeId dataTypeId);

    /** Gets data type ID of the value.
    *
    * @note This ID should match data types on the receiver
    * side.
    *
    * @return Data type ID of the value. If value's data type
    * is not receiver's data type then this function will return
    * ID_INVALID_LOCAL_DATA_TYPE.
    */
    inline LocalDataTypeId getReceiverDataTypeId() const;

    /** Gets data type ID of the value specified by the sender.
    *
    * @note This function returns data type ID specified by the
    * SENDER of the message. Returned ID may not match the ID on the
    * RECEIVER side with the exception of whiteboard builtin types.
    *
    * @return Data type ID of the value specified by the sender. 
    * If value's data type is not sender's data type then this
    * function will return ID_INVALID_LOCAL_DATA_TYPE.
    */
    inline LocalDataTypeId getSenderDataTypeId() const;

    /** Compile time ValueType resolver */
    template <typename NativeType> struct NativeValueType;

    /** Compile time DataTypeId resolver */
    template <typename NativeType> struct DataTypeId;

    /** Compile time reference type resolver */
    template <typename NativeType> struct ResultType;

    /** Helper type to get return value type for value conversions */
    template <typename NativeType> struct ConvertResult
    {
        typedef typename ResultType<typename RemoveAll<NativeType>::type>::type type;
    };

    /** Casts the value to given native type
    * @tparam NativeType The associated native type
    */
    template <typename NativeType> inline typename ConvertResult<NativeType>::type cast() const;

    /** Converts the value to given native type
    *
    * @tparam NativeType The associated native type
    */
    template <typename NativeType> inline typename ConvertResult<NativeType>::type convertTo() const;

    /** Converts between units of the same quantity.
    *
    * @param rSourceUnit Source unit of the quantity.
    * @param rTargetUnit Target unit of the quantity.
    */
    template <typename NativeType>
    inline typename ConvertResult<NativeType>::type convertTo(const unit::Info& rSourceUnit, const unit::Info& rTargetUnit) const;

#if WB_HAVE_OPERATION_TIMESTAMP
    /** Get the time of creation of the value.
    * If the value is coming from a separate physical device the time is then from the
    * timebase of that other device.
    * @return OperationTimestamp the time of creation of the value.
    */
    inline OperationTimestamp getCreationTime() const;

    /** Query if the value has been timestamped upon creation.
    * @return True if timestamp is attached.
    */
    inline bool hasCreationTime() const { return mIsTimestampedValue == 1; }
#endif // #if WB_HAVE_OPERATION_TIMESTAMP

private:
    // Disallow some constructors to avoid nasty accidental misusage
    template <typename T>
    Value(const T* value) DELETED;

    // No longs except in 64-bit linux where long == int64
#if !(defined(__GNUC__) && defined(__LP64__))
    Value(long value) DELETED;
#endif

    // Assignment operator is allowed only for internal use
    inline Value& operator=(const Value&);

private:

    /** Casts the value to given native type, works only for primitive types
    * @tparam NativeType The associated native type
    */
    template <typename NativeType> typename ConvertResult<NativeType>::type castTo() const;

    /* Helper for DataTypeId resolver. */
    template <typename Type, bool isStructure> struct DataTypeIdHelper;

#ifdef WB_NEED_STRUCTURE_CLEANING
    /* Deallocates wrapped pointers reserved for deserialized structure. */
    void cleanDeserializedStructure();
#endif

    /* Conversion helper. */
    template <typename Type, bool isStructure> struct ConversionHelper;

    /** Initializes a new instance of the Value class
     *
     * @param dataTypeId ID of the data type specified by the sender of the message
     * @param protocolVersion Protocol version of the protocol used to encode the data
     * @param pData Pointer to the data
     */
    inline Value(LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData);

    /** Helper for convertTo for checking the that the dataType of the Value matches */
    void dataTypeCheck(size_t dataTypeId) const;
    
    /** Internal cast function for casting to bool type, required to dodge certain compiler warnings. */
    bool castToBool() const;

public:
    /** Deserialization state flags */
    enum DeserializationState
    {
        NO_DESERIALIZATION_NEEDED,
        NOT_DESERIALIZED,
        DESERIALIZED,
        DESERIALIZATION_NOT_SUPPORTED
    };

protected:
    /** Library internal implementation can access these members */
    friend class ValueAccessor;

    /** Structure value deserializer
    *
    * @return Pointer to deserialized structure
    */
    const void* deserialize();

    /** Data type of the value */
    LocalDataTypeId mDataTypeId;

    /** A value indicating whether the ID is for a data type of the sender */
    uint8 mSenderDataType : 1;

    /** A value indicating whether the ID is for a data type of the receiver */
    uint8 mReceiverDataType : 1;

    /** Deserialization state of the value */
    uint8 mDeserializationState : 2;

    /** A flag indicating whether the value is an UnknownStructure */
    uint8 mIsUnknownStructure : 1;

    /** A flag indicating whether the value is an AlienStructure */
    uint8 mIsAlienStructure : 1;

    /** Flag to indicate that this value instance is wrapped into ValueWithTimestamp
    * In data message serialisation the passed value indicates if a timestamp is also serialised in the message
    * the value itself will not contain the timestamp as there may be multiple values in a single message.
    */
    uint8 mIsTimestampedValue : 1;

    /** Reserved for future use,
     *
     * @note Above flags have their own storage space only for convenience and debuggability.
     *       If we would need more flags we could have use
     *       union { mDeserializationState, struct { mIsUnknownStructure, mIsAlienStructure } }
     */
    uint8 mReserved : 1;

    /** Protocol version */
    ProtocolVersion mProtocolVersion;

    /** The data */
    const void* mpData;
};

#if WB_HAVE_OPERATION_TIMESTAMP
/** Extended Value type for operation timestamps */
class WB_API ValueWithTimestamp : public Value
{
public:
    /** default constructor */
    inline ValueWithTimestamp() : Value() { }

    /** Construct a ValueWithTimestamp instance from a Value instance which may be an instance of ValueWithTimestamp */
    inline ValueWithTimestamp(const Value& value) : Value(value)
    {
        if (value.hasCreationTime())
        {
            mCreationTime = value.getCreationTime();
            Value::mIsTimestampedValue = true;
        }
    }

    /** Construct a ValueWithTimestamp instance from a Value instance and given timestamp
    * @param originalValue The Value instance
    * @creationTime Time of creation of the value
    * @param hasTimestamp Flag to indicate if the timestamp is to used or not
    */
    inline ValueWithTimestamp(const Value& originalValue, const OperationTimestamp& creationTime, bool hasTimestamp=true):
        Value(originalValue), mCreationTime(creationTime)
    {
        Value::mIsTimestampedValue = hasTimestamp;
    }

    /** Get the creation timestamp of the Value instance
    * @return the creation time
    */
    inline OperationTimestamp getCreationTime() const
    {
        return mCreationTime;
    }

    /** ValueWithTimestamp copy constructor
    * @param value The instance to copy from
    */
    inline ValueWithTimestamp(const ValueWithTimestamp& value)
    {
        assign(value);
    }

    /** Copies value from another object. Only use if you know what you are doing :) */
    inline void assign(const ValueWithTimestamp& that)
    {
        memcpy(this, &that, sizeof(*this));
    }

private: 
    OperationTimestamp mCreationTime;
};
#endif

inline Value::Value()
    : mDataTypeId(WB_TYPE_NONE),
      mSenderDataType(1),
      mReceiverDataType(1),
      mDeserializationState(NO_DESERIALIZATION_NEEDED),
      mIsUnknownStructure(0),
      mIsAlienStructure(0),
      mIsTimestampedValue(0),
      mReserved(0),
      mProtocolVersion(0),
      mpData(NULL)
{
}

inline Value::Value(char* rValue)
    : mDataTypeId(WB_TYPE_STRING),
      mSenderDataType(1),
      mReceiverDataType(1),
      mDeserializationState(NO_DESERIALIZATION_NEEDED),
      mIsUnknownStructure(0),
      mIsAlienStructure(0),
      mIsTimestampedValue(0),
      mReserved(0),
      mProtocolVersion(0), 
      mpData(rValue)
{
}

inline Value::Value(const char* rValue)
{
    new (this) Value(const_cast<char*>(rValue));
}

template <typename NativeType>
inline Value::Value(const NativeType& rValue)
    : mDataTypeId(Value::DataTypeId<NativeType>::value),
      mSenderDataType(1),
      mReceiverDataType(1),
      mDeserializationState(NO_DESERIALIZATION_NEEDED),
      mIsUnknownStructure(0),
      mIsAlienStructure(0),
      mIsTimestampedValue(0),
      mReserved(0),
      mProtocolVersion(0), 
      mpData(&rValue)
{
}

inline Value::Value(LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData)
    : mDataTypeId(dataTypeId),
      mSenderDataType(1),
      mReceiverDataType(1),
      mDeserializationState(NO_DESERIALIZATION_NEEDED),
      mIsUnknownStructure(0),
      mIsAlienStructure(0),
      mIsTimestampedValue(0),
      mReserved(0),
      mProtocolVersion(protocolVersion),
      mpData(pData)
{
}

inline Value::~Value()
{
#ifdef WB_NEED_STRUCTURE_CLEANING
    if (mDeserializationState == DESERIALIZED)
    {
        cleanDeserializedStructure();
    }
#endif
}

inline Value& Value::operator=(const Value& rOther)
{
    memcpy(this, &rOther, sizeof(Value));
    mIsTimestampedValue = 0;
    return *this;
}

inline ValueType Value::getType(LocalDataTypeId dataTypeId)
{
    return (dataTypeId >= static_cast<LocalDataTypeId>(WB_TYPE_STRUCTURE)) ? static_cast<ValueType>(WB_TYPE_STRUCTURE)
        : static_cast<ValueType>(dataTypeId);
}

inline ValueType Value::getType() const
{
    return getType(mDataTypeId);
}

LocalDataTypeId Value::getReceiverDataTypeId() const
{
    return mReceiverDataType ? mDataTypeId : ID_INVALID_LOCAL_DATA_TYPE;
}

LocalDataTypeId Value::getSenderDataTypeId() const
{
    return mSenderDataType ? mDataTypeId : ID_INVALID_LOCAL_DATA_TYPE;
}

template <typename NativeType> inline typename Value::ConvertResult<NativeType>::type Value::convertTo() const
{
    dataTypeCheck(DataTypeId<NativeType>::value);
    typedef ConversionHelper<typename RemoveAll<NativeType>::type,
                             IsStructure<typename RemoveAll<NativeType>::type>::value> Helper;
    return Helper::convertTo(*this);
}

template <> struct Value::NativeValueType<NoType>
{
    static const ValueType value = WB_TYPE_NONE;
};
template <> struct Value::NativeValueType<bool>
{
    static const ValueType value = WB_TYPE_BOOL;
};
template <> struct Value::NativeValueType<int8>
{
    static const ValueType value = WB_TYPE_INT8;
};
template <> struct Value::NativeValueType<uint8>
{
    static const ValueType value = WB_TYPE_UINT8;
};
template <> struct Value::NativeValueType<int16>
{
    static const ValueType value = WB_TYPE_INT16;
};
template <> struct Value::NativeValueType<uint16>
{
    static const ValueType value = WB_TYPE_UINT16;
};
template <> struct Value::NativeValueType<int32>
{
    static const ValueType value = WB_TYPE_INT32;
};
template <> struct Value::NativeValueType<uint32>
{
    static const ValueType value = WB_TYPE_UINT32;
};
template <> struct Value::NativeValueType<int64>
{
    static const ValueType value = WB_TYPE_INT64;
};
template <> struct Value::NativeValueType<uint64>
{
    static const ValueType value = WB_TYPE_UINT64;
};
template <> struct Value::NativeValueType<float>
{
    static const ValueType value = WB_TYPE_FLOAT;
};
template <> struct Value::NativeValueType<double>
{
    static const ValueType value = WB_TYPE_DOUBLE;
};
template <> struct Value::NativeValueType<char*>
{
    static const ValueType value = WB_TYPE_STRING;
};

#ifdef WB_HAVE_DEPRECATED_BYTE_STREAM
template <> struct Value::NativeValueType<ByteStream>
{
    static const ValueType value = WB_TYPE_BYTE_STREAM;
};
#endif

#ifdef WB_HAVE_HASH_STRING
template <> struct Value::NativeValueType<HashString>
{
    static const ValueType value = WB_TYPE_HASH_STRING;
};
#endif

#ifdef WB_HAVE_UNKNOWN_STRUCTURES
template <> struct Value::NativeValueType<UnknownStructure>
{
    static const ValueType value = WB_TYPE_STRUCTURE;
};
#endif

// Enumerations
template <typename Definition, typename DefitionType, typename ScalarBaseType>
struct Value::NativeValueType<TypedEnum<Definition, DefitionType, ScalarBaseType> >
{
    static const ValueType value = NativeValueType<ScalarBaseType>::value;
};

// Default to structure
template <typename NativeType> struct Value::NativeValueType
{
    WB_STATIC_VERIFY(IsStructure<NativeType>::value, Not_A_Structure_Type);

    static const ValueType value = WB_TYPE_STRUCTURE;
};

template <typename NativeType> struct Value::DataTypeId
{
    static const LocalDataTypeId value = Value::DataTypeIdHelper<typename RemoveAll<NativeType>::type,
                                                                 IsStructure<typename RemoveAll<NativeType>::type>::value>::value;
};

template <> struct Value::ResultType<NoType>
{
    typedef NoType type;
};
template <> struct Value::ResultType<bool>
{
    typedef bool type;
};
template <> struct Value::ResultType<int8>
{
    typedef int8 type;
};
template <> struct Value::ResultType<uint8>
{
    typedef uint8 type;
};
template <> struct Value::ResultType<int16>
{
    typedef int16 type;
};
template <> struct Value::ResultType<uint16>
{
    typedef uint16 type;
};
template <> struct Value::ResultType<int32>
{
    typedef int32 type;
};
template <> struct Value::ResultType<uint32>
{
    typedef uint32 type;
};
template <> struct Value::ResultType<int64>
{
    typedef int64 type;
};
template <> struct Value::ResultType<uint64>
{
    typedef uint64 type;
};
template <> struct Value::ResultType<float>
{
    typedef float type;
};
template <> struct Value::ResultType<double>
{
    typedef double type;
};
template <> struct Value::ResultType<char*>
{
    typedef const char* type;
};
#ifdef WB_HAVE_UNKNOWN_STRUCTURES
template <> struct Value::ResultType<UnknownStructure>
{
    // UnknownStructure is passed by value
    typedef UnknownStructure type;
};
#endif

#ifdef WB_HAVE_HASH_STRING
template <> struct Value::ResultType<HashString>
{
    // HashString is passed by value
    typedef HashString type;
};
#endif

// Enumerations
template <typename Definition, typename DefitionType, typename ScalarBaseType>
struct Value::ResultType<TypedEnum<Definition, DefitionType, ScalarBaseType> >
{
    typedef TypedEnum<Definition, DefitionType, ScalarBaseType> type;
};

// Default to structure and others that are passed by reference
template <typename NativeType> struct Value::ResultType
{
    typedef const typename RemoveAll<NativeType>::type& type;
};

/* DataTypeID helper. Default case structures */
template <typename NativeType, bool isStructure> struct Value::DataTypeIdHelper
{
    static const LocalDataTypeId value = NativeType::DATA_TYPE_ID;
};

/* DataTypeID helper. Other types */
template <typename NativeType> struct Value::DataTypeIdHelper<NativeType, false>
{
    static const LocalDataTypeId value = static_cast<LocalDataTypeId>(NativeValueType<NativeType>::value);
};

/* Conversion helper. Default case (Alignment problematic native types) */
template <typename NativeType, bool isStructure> struct Value::ConversionHelper
{
    /* Avoid alignment errors by copying to temporary variable */
    static inline NativeType convertTo(const Value& rValue)
    {
        NativeType value;
        memcpy(&value, rValue.mpData, sizeof(NativeType));
        return value;
    }
};

template <> struct WB_API Value::ConversionHelper<NoType, false>
{
    static NoType convertTo(const Value& rValue);
};

template <> struct WB_API Value::ConversionHelper<bool, false>
{
    static bool convertTo(const Value& rValue);
};

template <> struct WB_API Value::ConversionHelper<int8, false>
{
    static int8 convertTo(const Value& rValue);
};

template <> struct WB_API Value::ConversionHelper<uint8, false>
{
    static uint8 convertTo(const Value& rValue);
};

template <> struct WB_API Value::ConversionHelper<char*, false>
{
    static const char* convertTo(const Value& rValue);
};

#ifdef WB_HAVE_DEPRECATED_BYTE_STREAM
template <> struct WB_API Value::ConversionHelper<ByteStream, false>
{
    static const ByteStream& convertTo(const Value& rValue);
};
#endif

#ifdef WB_HAVE_UNKNOWN_STRUCTURES
template <> struct WB_API Value::ConversionHelper<UnknownStructure, false>
{
    static UnknownStructure convertTo(const Value& rValue);
};
#endif

#ifdef WB_HAVE_HASH_STRING
template <> struct WB_API Value::ConversionHelper<HashString, false>
{
    static HashString convertTo(const Value& rValue);
};
#endif


/* Structure conversion */
template <typename StructureType> struct Value::ConversionHelper<StructureType, true>
{
    static inline const StructureType& convertTo(const Value& rValue)
    {
        return *static_cast<const StructureType*>(const_cast<Value&>(rValue).deserialize());
    }
};

template <typename NativeType>
inline typename Value::ConvertResult<NativeType>::type Value::convertTo(const unit::Info& rSourceUnit,
                                                                        const unit::Info& rTargetUnit) const
{
    NativeType value = convertTo<NativeType>();
    unit::Value result;
    bool success = unit::tryConvert(rSourceUnit, rTargetUnit, static_cast<unit::Value>(value), result);
    WB_NOT_USED(success);
    WB_ASSERT(success);
    return static_cast<NativeType>(result);
}

// Explicitly specialize the primitive types

template <> inline Value::ConvertResult<bool>::type Value::cast<bool>() const
{
    // Special handling for bool to dodge compiler warning 
    // 'type': forcing value to bool true or false (performance warning)
    return castToBool();
}

template <> inline Value::ConvertResult<int8>::type Value::cast<int8>() const
{
    return castTo<int8>();
}

template <> inline Value::ConvertResult<uint8>::type Value::cast<uint8>() const
{
    return castTo<uint8>();
}

template <> inline Value::ConvertResult<int16>::type Value::cast<int16>() const
{
    return castTo<int16>();
}

template <> inline Value::ConvertResult<uint16>::type Value::cast<uint16>() const
{
    return castTo<uint16>();
}

template <> inline Value::ConvertResult<int32>::type Value::cast<int32>() const
{
    return castTo<int32>();
}

template <> inline Value::ConvertResult<uint32>::type Value::cast<uint32>() const
{
    return castTo<uint32>();
}

template <> inline Value::ConvertResult<int64>::type Value::cast<int64>() const
{
    return castTo<int64>();
}

template <> inline Value::ConvertResult<uint64>::type Value::cast<uint64>() const
{
    return castTo<uint64>();
}

template <> inline Value::ConvertResult<float>::type Value::cast<float>() const
{
    return castTo<float>();
}

template <> inline Value::ConvertResult<double>::type Value::cast<double>() const
{
    return castTo<double>();
}

// For the types that are not primitive types, use the default convertTo()
template <typename NativeType> inline typename Value::ConvertResult<NativeType>::type Value::cast() const
{
    return convertTo<NativeType>();
}

#if WB_HAVE_OPERATION_TIMESTAMP
OperationTimestamp Value::getCreationTime() const
{
    return mIsTimestampedValue ? static_cast<const ValueWithTimestamp*>(this)->getCreationTime() : 0;
}
#endif

} // namespace whiteboard

