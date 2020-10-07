#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include "whiteboard/Identifiers.h"
#include "whiteboard/metadata/IDataTypeMetadata.h"

WB_HEADER_CHECK_DEFINE(WB_HAVE_UNKNOWN_STRUCTURES)

#ifdef WB_HAVE_UNKNOWN_STRUCTURES

namespace whiteboard
{

// Forward declarations
class UnknownStructureAccessor;
class MetadataMap;

/** Interface of visitors that walk structure contents */
class IStructureDataVisitor : public IDynamicallyAllocatable
{
public:
    /** Destructor. */
    virtual ~IStructureDataVisitor() {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(bool /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int8 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int16 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int32 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(int64 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint8 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint16 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint32 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(uint64 /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(float /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(double /*data*/) {}

    /** Called when structure deserializer deserializes scalar value
    *
    * @param data Data that was deserialized
    */
    virtual void onScalarValue(const char* /*data*/) {}

    /** Called when structure deserializer enters enumeration
    *
    * @param nameId Name ID of the enumeration type or ID_INVALID_STRING if enumeration is anonymous
    */
    virtual void enterEnumeration(metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer exits enumeration
    *
    * @param nameId Name ID of the enumeration type or ID_INVALID_STRING if enumeration is anonymous
    */
    virtual void exitEnumeration(metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int8 /*data*/, metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int16 /*data*/, metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(int32 /*data*/, metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(uint8 /*data*/, metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(uint16 /*data*/, metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer deserializes enumeration value
    *
    * @param data Data that was deserialized
    * @param nameId Name ID of the value
    */
    virtual void onEnumerationValue(uint32 /*data*/, metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer enters sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    */
    virtual void enterStructure(metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer exits sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    */
    virtual void exitStructure(metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer enters property
    *
    * @param nameId Name ID of the property
    * @param hasValue A value indicating whether the property has a value
    */
    virtual void enterProperty(metadata::StringId /*nameId*/, bool /*hasValue*/) {}

    /** Called when structure deserializer exits property
    *
    * @param nameId Name ID of the property
    */
    virtual void exitProperty(metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer enters array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @param numberOfItems Number of items in the array
    */
    virtual void enterArray(metadata::StringId /*nameId*/, size_t /*numberOfItems*/) {}

    /** Called when structure deserializer exits array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    */
    virtual void exitArray(metadata::StringId /*nameId*/) {}

    /** Called when structure deserializer enters new array item
    */
    virtual void enterArrayItem() {}

    /** Called when structure deserializer exits array item
    */
    virtual void exitArrayItem() {}
};

/** Class that represents unknown structure. Receiver can always convert request parameters and
* return values to UnknownStructure instance and then walk the structure contents in a more dynamic
* way. */
class WB_API UnknownStructure FINAL
{
public:
    /** Default constructor */
    UnknownStructure();

    /** Copy constructor.
     *
     * @note UnknownStructure is behaving like unique_ptr. Memory allocated for UnknownStructure
     *       is deallocated when object with ownership of the memory is deallocated (leaves scope).
     *       Ownership of the object is transferred with copy constructor and assignment operator.
     *       This also invalidates data pointer of previous owner.
     *
     * @param rOther UnknownStructure which ownership should be transferred
     */
    UnknownStructure(const UnknownStructure& rOther);

    /** Assignment operator
     *
     * @param rOther UnknownStructure which ownership should be transferred
     * @return Reference to this UnknownStructure instance
     */
    UnknownStructure& operator=(const UnknownStructure& rOther);

    /** Destructor */
    ~UnknownStructure();

    /** Checks whether the structure contains valid information. */
    inline bool isValid() const
    {
        return mDataTypeId != WB_TYPE_NONE;
    }

    /** ID of the datatype. */
    inline LocalDataTypeId getDataTypeId() const
    {
        return mDataTypeId;
    }

    /** Gets protocol version used to encode the structure */
    inline ProtocolVersion getProtocolVersion() const
    {
        return mProtocolVersion;
    }

    /** Deserializes the structure and calls tree visitor members while doing so.
    *
    * @param metadataForSenderDataType A value indicating whether the dataTypeId and rMetadataContainer
    *        are for sender data types
    * @param rMetadataContainer Metadata container that can describe the structure type
    * @param rDataVisitor Structure visitor instance that should handle the data
    * @return A value indicating whether the structure was successfully deserialized
    */
    bool deserialize(
        bool metadataForSenderDataType,
        const IDataTypeMetadata& rMetadataContainer,
        IStructureDataVisitor& rTreeVisitor) const;

    /** Copies unknown structure to a new instance. Both of the instances
     *  will have their own copy of the data.
     **/
    UnknownStructure copy() const;

private:
    /** Constructor 
     *
     * @param dataTypeId ID of the data type
     * @param protocolVersion Protocol version that was used to encode the structure
     * @param deserialized A value indicating whether the is deserialized
     * @param dataOwner Ownership of the data
     * @param pData Encoded data
     */
    UnknownStructure(LocalDataTypeId dataTypeId, ProtocolVersion protocolVersion, bool deserialized, bool dataOwner, const void* pData);

private:
    /** Library internal implementation can access these members */
    friend class UnknownStructureAccessor;

    /** Data type of the value */
    LocalDataTypeId mDataTypeId;

    /** Protocol version */
    ProtocolVersion mProtocolVersion;

    /** A value indicating whether the is deserialized */
    bool mDeserialized : 1;

    /** Ownership of the data */
    mutable bool mDataOwner : 1;

    /** The data */
    mutable const void* mpData;
};

} // namespace whiteboard

#endif
