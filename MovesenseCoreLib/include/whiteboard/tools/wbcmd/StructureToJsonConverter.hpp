#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include <stack>
#include "plugin-api/json11/json11.hpp"

// Helper class for building JSON using visitor pattern
class StructureJsonConverter final : public wb::IStructureDataVisitor
{
    const wb::IDataTypeMetadataContainer& mrMetadataContainer;

public:
    StructureJsonConverter(const wb::IDataTypeMetadataContainer& metadataContainer)
        : mrMetadataContainer(metadataContainer)
    {
    }

    virtual ~StructureJsonConverter() {}

    void onScalarValue(bool data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(int8 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(int16 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(int32 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(int64 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(uint8 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(uint16 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(uint32 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(uint64 data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(float data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(double data) override final { mCurrent = wbjson::Json(data); }
    void onScalarValue(const char* data) override final { mCurrent = wbjson::Json(data); }

    void enterEnumeration(WB_RES::StringId) override final {}
    void exitEnumeration(WB_RES::StringId) override final {}

    void onEnumerationValue(int8 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void onEnumerationValue(int16 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void onEnumerationValue(int32 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void onEnumerationValue(int64 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void onEnumerationValue(uint8 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void onEnumerationValue(uint16 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void onEnumerationValue(uint32 data, WB_RES::StringId nameId) override final
    {
        mCurrent = wbjson::Json(wbjson::Json::array{data, mrMetadataContainer.getString(nameId)});
    }

    void enterStructure(WB_RES::StringId) override final
    {
        mStructureStack.push(wbjson::Json::object());
    }

    void exitStructure(WB_RES::StringId) override final
    {
        mCurrent = mStructureStack.top();
        mStructureStack.pop();
    }

    void enterProperty(WB_RES::StringId /*nameId*/, bool hasValue) override final 
    { 
        if (!hasValue) {
            mCurrent = wbjson::Json();
        }
    }

    void exitProperty(WB_RES::StringId nameId) override final
    {
        const std::string name = mrMetadataContainer.getString(nameId);
        mStructureStack.top().push_back(std::make_pair(name, mCurrent));
    }

    void enterArray(WB_RES::StringId, size_t /*numberOfItems*/) override final
    {
        mArrayStack.push(wbjson::Json::array());
    }

    void exitArray(WB_RES::StringId) override final
    {
        mCurrent = mArrayStack.top();
        mArrayStack.pop();
    }

    void enterArrayItem() override final {}

    void exitArrayItem() override final { mArrayStack.top().push_back(mCurrent); }

    wbjson::Json mCurrent;
    std::stack<wbjson::Json::array> mArrayStack;
    std::stack<wbjson::Json::object> mStructureStack;
};
