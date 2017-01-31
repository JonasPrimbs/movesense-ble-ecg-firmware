#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/Value.h"

namespace whiteboard
{

/** Wrapper class to provide a more priviledged access to Value class */
class ValueAccessor
{
public:
    /** Initializes a new instance of the Value class
    *
    * @param dataTypeId ID of the data type specified by the sender of the message
    * @param protocolVersion Protocol version used to encode the value
    * @param pData Pointer to the data
    * @return New Value object
    */
    inline static Value makeValue(LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData);

    /** Gets data type ID of the Value object
     *
     * @param rValue The value to access
     * @return data type of the Value object
     */
    inline static LocalDataTypeId getDataTypeId(const Value& rValue);
    
    /** Gets data pointer value of the Value object
    *
    * @param rValue The value to access
    * @return data pointer of the Value object
    */
    inline static const void* getData(const Value& rValue);

    /** Gets deserialization state of the Value object
    *
    * @param rValue The value to access
    * @return deserialization state of the Value object
    */
    inline static uint8 getDeserializationState(const Value& rValue);

    /** Gets associated protocol version of the Value object
    *
    * @param rValue The value to access
    * @return associated protocol version of the Value object
    */
    inline static ProtocolVersion getProtocolVersion(const Value& rValue);

    /** Gets associated serializer instance of the Value object
    *
    * @param rValue The value to access
    * @return associated serializer instance of the Value object
    */
    inline static const IValueSerializer* getSerializer(const Value& rValue);

    /** Sets data type ID of the Value object
    *
    * @param rValue The value to access
    * @param dataTypeId New data type ID of the Value object
    */
    inline static void setDataTypeId(Value& rValue, LocalDataTypeId dataTypeId);

    /** Sets data pointer value of the Value object
    *
    * @param rValue The value to access
    * @param pData New value for data pointer of the Value object
    */
    inline static void setData(Value& rValue, const void* pData);

    /** Sets deserialization state of the Value object
    *
    * @param rValue The value to access
    * @param newDeserializationState New deserialization state of the Value object
    */
    inline static void setDeserializationState(Value& rValue, uint8 newDeserializationState);

    /** Sets the receiver data type flag of the Value object
    *
    * @param rValue The value to access
    * @param isReceiverDataType New state for the receiver data type flag of the Value object
    */
    inline static void setReceiverDataType(Value& rValue, bool isReceiverDataType);

    /** Sets the sender data type flag of the Value object
    *
    * @param rValue The value to access
    * @param isSenderDataType New state for the sender data type flag of the Value object
    */
    inline static void setSenderDataType(Value& rValue, bool isSenderDataType);
};

inline Value ValueAccessor::makeValue(LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, const void* pData)
{
    return Value(dataTypeId, protocolVersion, pData);
}

inline LocalDataTypeId ValueAccessor::getDataTypeId(const Value& rValue)
{
    return rValue.mDataTypeId;
}

inline const void* ValueAccessor::getData(const Value& rValue)
{
    return rValue.mpData;
}

inline uint8 ValueAccessor::getDeserializationState(const Value& rValue)
{
    return rValue.mDeserializationState;
}

inline ProtocolVersion ValueAccessor::getProtocolVersion(const Value& rValue)
{
    return rValue.mProtocolVersion;
}

inline const IValueSerializer* ValueAccessor::getSerializer(const Value& rValue)
{
    return rValue.mpValueSerializer;
}

inline void ValueAccessor::setDataTypeId(Value& rValue, LocalDataTypeId dataTypeId)
{
    rValue.mDataTypeId = dataTypeId;
}

inline void ValueAccessor::setData(Value& rValue, const void* pData)
{
    rValue.mpData = pData;
}

inline void ValueAccessor::setDeserializationState(Value& rValue, uint8 newDeserializationState)
{
    rValue.mDeserializationState = newDeserializationState;
}

inline void ValueAccessor::setReceiverDataType(Value& rValue, bool isReceiverDataType)
{
    rValue.mReceiverDataType = isReceiverDataType ? 1 : 0;
}

inline void ValueAccessor::setSenderDataType(Value& rValue, bool isSenderDataType)
{
    rValue.mSenderDataType = isSenderDataType ? 1 : 0;
}

} // namespace whiteboard
