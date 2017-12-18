#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/builtinTypes/Structures.h"

namespace whiteboard
{

namespace TypedEnumHelper
{
    /** Converts enumeration value of given datatype to string.

    Example: whiteboard::TypedEnumHelper::toString(WB_RES::OperationType::DATA_TYPE_ID, WB_RES::OperationType::GET)
    returns "GET".

    @param id [in] Datatype id of the enumeration type.
    @param value [in] Enumeration value to convert to string.
    @return the string representation of the value or NULL if invalid value given.
    */
    WB_API const char* toString(LocalDataTypeId id, int32 value);

    /** Converts valueName (string representation of the value) to the actual enumeration value.

    Example: whiteboard::TypedEnumHelper::parseEnum(WB_RES::OperationType::DATA_TYPE_ID, "GET")
    returns 0.

    @param id [in] Datatype id of the enumeration type.
    @param valueName [in] string representation of the value as defined in metadata.
    @param rValue [out] The parsed value if call returned true 
    @return true if the parse succeeded and rValue is valid, else false and rValue is untouched.
    */
    WB_API bool parseEnum(LocalDataTypeId id, const char* valueName, int32& rValue);
}

/** Enumeration class implementation for non C++11 compatible compilers.
 * Enumeration class is a type safe enumeration that stores its value
 * in explicitly defined storage type.
 *
 * @tparam Definition Structure type that contains the enumeration definition 
 * @tparam DefinitionType Enumeration type
 * @tparam BaseType Underlying data storage type
 */
template <typename Definition, typename DefinitionType, typename BaseType>
class TypedEnum : public Definition
{
private:
    /** Type of the enumeration */
    typedef DefinitionType EnumType;

    /** Base type for storing the enumeration value */
    WB_ALIGNED(BaseType) value;

public:
    /** Default constructor */
    inline TypedEnum() {}

    /** Copy constructor */
    inline TypedEnum(const TypedEnum& rOther) : value(rOther.value) {}

    /** Constructor that initializes enum class from enumeration value */
    inline TypedEnum(EnumType v) : value(static_cast<BaseType>(v)) {}

    /** Cast operator that implicitly casts this class to its base type
     *
     * @return Value of the enumeration in its base type
     */
    inline EXPLICIT operator BaseType&() { return value; }

    /** Cast operator that implicitly casts this class to its base type
    *
    * @return Value of the enumeration in its base type
    */
    inline operator BaseType() const { return value; }

    /** And getter to make code more readable than calling cast operators directly
    *
    * @return Value of the enumeration in its base type
    */
    inline BaseType getValue() const { return value; }

    /** Convert enum value to string.
    *
    * @return valueName of the enumeration value as defined in the device metadata.
    */
    inline const char* toString() const
    { 
        return whiteboard::TypedEnumHelper::toString(Definition::DATA_TYPE_ID, value);
    }

    /** Converts given string to enum value.
    * 
    * @param valueName [in] string representation of the value as defined in metadata.
    * @param rValue [out] The parsed value if call returned true
    * @return true if the parse succeeded and rValue is valid, else false and rValue is untouched.
    */
    static inline bool parseEnum(const char* valueName, TypedEnum& rValue)
    {
        int32 value;
        if (whiteboard::TypedEnumHelper::parseEnum(Definition::DATA_TYPE_ID, valueName, value))
        {
            rValue = TypedEnum(static_cast<EnumType>(value));
            return true;
        }
        return false;
    }

    /** Assignment operator.
     *
     * @param rOther Another typed enumeration
     * @return Reference to this object
     */
    inline TypedEnum& operator=(const TypedEnum& rOther)
    {
        value = rOther.value;
        return *this;
    }

    /** Assignment operator.
    *
    * @param rOther Another typed enumeration
    * @return Reference to this object
    */
    inline TypedEnum& operator=(const EnumType other)
    {
        value = static_cast<BaseType>(other);
        return *this;
    }

    /** Comparison operators */
    inline friend bool operator==(const TypedEnum& lhs, const TypedEnum& rhs) { return lhs.value == rhs.value; }
    inline friend bool operator==(const TypedEnum& lhs, const EnumType& rhs) { return lhs.value == rhs; }
    inline friend bool operator==(const EnumType& lhs, const TypedEnum& rhs) { return lhs == rhs.value; }
    inline friend bool operator!=(const TypedEnum& lhs, const TypedEnum& rhs) { return lhs.value != rhs.value; }
    inline friend bool operator!=(const TypedEnum& lhs, const EnumType& rhs) { return lhs.value != rhs; }
    inline friend bool operator!=(const EnumType& lhs, const TypedEnum& rhs) { return lhs != rhs.value; }
    inline friend bool operator<(const TypedEnum& lhs, const TypedEnum& rhs) { return lhs.value < rhs.value; }
    inline friend bool operator<=(const TypedEnum& lhs, const TypedEnum& rhs) { return lhs.value <= rhs.value; }
    inline friend bool operator>(const TypedEnum& lhs, const TypedEnum& rhs) { return lhs.value > rhs.value; }
    inline friend bool operator>=(const TypedEnum& lhs, const TypedEnum& rhs) { return lhs.value >= rhs.value; }
};

} // namespace whiteboard
