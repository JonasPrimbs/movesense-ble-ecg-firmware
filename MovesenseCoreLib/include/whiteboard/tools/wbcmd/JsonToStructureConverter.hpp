#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <stack>
#include <whiteboard/builtinTypes/AlienStructure.h>
#include "plugin-api/plugin-api.hpp"

// Helper class for building whiteboard structures from JSON data
class JsonDataAccessor final : public whiteboard::IStructureDataAccessor
{
public:
    JsonDataAccessor(wbjson::Json& json, const whiteboard::IDataTypeMetadataContainer& rMetadataContainer)
        : mrMetadataContainer(rMetadataContainer)
    {
        mStack.push(json);
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(bool& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_bool())
        {
            return false;
        }

        rValue = rTopJson.bool_value();
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(int8& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = static_cast<int8>(rTopJson.int_value());
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(uint8& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = static_cast<uint8>(rTopJson.uint_value());
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(int16& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = static_cast<int16>(rTopJson.int_value());
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(uint16& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = static_cast<uint16>(rTopJson.uint_value());
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(int32& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = static_cast<int32>(rTopJson.int_value());
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(uint32& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = static_cast<uint32>(rTopJson.uint_value());
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(int64& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = rTopJson.int_value();
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(uint64& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = rTopJson.uint_value();
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(float& rValue) override final
    {
        double value;
        if (!getScalarValue(value))
        {
            return false;
        }

        rValue = static_cast<float>(value);
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(double& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_number())
        {
            return false;
        }

        rValue = rTopJson.number_value();
        return true;
    }

    /** Gets scalar value of current member
    *
    * @param rValue On output contains the value of the member
    * @return A value indicating whether the data was retrieved
    */
    bool getScalarValue(const char*& rValue) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_string())
        {
            return false;
        }

        rValue = rTopJson.string_value().c_str();
        return true;
    }

    /** Called when structure serializer enters enumeration
    *
    * @param nameId Name ID of the enumeration type or ID_INVALID_STRING if enumeration is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    bool enterEnumeration(WB_RES::StringId) override final
    {
        return true;
    }

    /** Called when structure serializer exits enumeration
    *
    * @return A value indicating whether the operation completed successfully
    */
    bool exitEnumeration(WB_RES::StringId) override final
    {
        return true;
    }

    /** Called when structure serializer enters sub structure
    *
    * @param nameId Name ID of the structure type or ID_INVALID_STRING if structure is anonymous
    * @return A value indicating whether the operation completed successfully
    */
    bool enterStructure(WB_RES::StringId) override final
    {
        const auto rTopJson = mStack.top();
        return rTopJson.is_object() ? true : false;
    }

    /** Called when structure serializer exits sub structure
    *
    * @return A value indicating whether the operation completed successfully
    */
    bool exitStructure(WB_RES::StringId) override final
    {
        return true;
    }

    /** Called when structure serializer enters property
    *
    * @param nameId Name ID of the property
    * @param required A value indicating whether the property is required
    * @param rHasValue On output contains a value indicating whether the optional has a value
    * @return A value indicating whether the operation completed successfully
    */
    bool enterProperty(WB_RES::StringId nameId, bool /*required*/, bool& rHasValue) override final
    {
        const char* name = mrMetadataContainer.getString(nameId);
        if (name == NULL)
        {
            return false;
        }

        rHasValue = false;

        const wbjson::Json& rTopJson = mStack.top();
        for (const auto property : rTopJson.object_items())
        {
            if (property.first == name)
            {
                rHasValue = true;
                mStack.push(property.second);
                return true;
            }
        }

        // Missing optional has no value (null)
        mStack.push(wbjson::Json());
        return true;
    }

    /** Called when structure serializer exits property
    *
    * @param nameId Name ID of the property
    * @return A value indicating whether the operation completed successfully
    */
    bool exitProperty(WB_RES::StringId) override final
    {
        mStack.pop();
        return true;
    }

    /** Called when structure serializer enters array
    *
    * @param nameId Name ID of the array type or ID_INVALID_STRING if array is anonymous
    * @param rNumberOfItems On output contains number of items in the array
    * @return A value indicating whether the data was retrieved
    */
    bool enterArray(WB_RES::StringId nameId, uint16& rNumberOfItems) override final
    {
        const auto rTopJson = mStack.top();
        if (!rTopJson.is_array())
        {
            return false;
        }

        rNumberOfItems = static_cast<uint16>(rTopJson.array_items().size());
        return true;
    }

    /** Called when structure serializer exits array
    *
    * @return A value indicating whether the operation completed successfully
    */
    bool exitArray(WB_RES::StringId) override final
    {
        return true;
    }

    /** Called when structure serializer enters new array item
    *
    * @param index Array item index
    * @return A value indicating whether the operation completed successfully
    */
    bool enterArrayItem(size_t index) override final
    {
        const auto rTopJson = mStack.top();
        WB_ASSERT(rTopJson.is_array());

        mStack.push(rTopJson.array_items()[index]);
        return true;
    }

    /** Called when structure serializer exits array item
    *
    * @return A value indicating whether the operation completed successfully
    */
    bool exitArrayItem() override final
    {
        mStack.pop();
        return true;
    }

private:
    const whiteboard::IDataTypeMetadataContainer& mrMetadataContainer;
    std::stack<wbjson::Json> mStack;
};

