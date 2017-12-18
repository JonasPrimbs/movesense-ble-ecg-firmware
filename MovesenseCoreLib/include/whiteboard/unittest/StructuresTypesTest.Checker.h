#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2016.
All rights reserved.
******************************************************************************/

#include "StructuresTypesTest.Shared.h"
#include "ut-resources/resources.h"

class Checker
{
public:
    template <typename T>
    static void checkEqual(const T& rReference, const T& rActual)
    {
        EXPECT_EQ(rReference, rActual);
    }

    static void checkEqual(const float& rReference, const float& rActual);
    static void checkEqual(const double& rReference, const double& rActual);
    static void checkEqual(const char*& rReference, const char*& rActual);
    static void checkEqual(const whiteboard::Date& rReference, const whiteboard::Date& rActual);
    static void checkEqual(const whiteboard::Time& rReference, const whiteboard::Time& rActual);
    static void checkEqual(const whiteboard::DateTime& rReference, const whiteboard::DateTime& rActual);
    static void checkEqual(const whiteboard::Timestamp& rReference, const whiteboard::Timestamp& rActual);
    static void checkEqual(const whiteboard::IntVector2D& rReference, const whiteboard::IntVector2D& rActual);
    static void checkEqual(const whiteboard::IntVector3D& rReference, const whiteboard::IntVector3D& rActual);
    static void checkEqual(const whiteboard::FloatVector2D& rReference, const whiteboard::FloatVector2D& rActual);
    static void checkEqual(const whiteboard::FloatVector3D& rReference, const whiteboard::FloatVector3D& rActual);

    template <typename T>
    static void checkEqual(const whiteboard::Optional<T>& rReference, const whiteboard::Optional<T>& rActual)
    {
        EXPECT_EQ(rReference.hasValue(), rActual.hasValue());
        if (rReference.hasValue() && rActual.hasValue())
        {
            checkEqual(rReference.getValue(), rActual.getValue());
        }
    }

    template <typename T>
    static void checkEqual(const whiteboard::Array<T>& rReference, const whiteboard::Array<T>& rActual)
    {
        EXPECT_EQ(rReference.getNumberOfItems(), rActual.getNumberOfItems());
        if (rReference.getNumberOfItems() == rActual.getNumberOfItems())
        {
            for (size_t i = 0; i < rReference.getNumberOfItems(); ++i)
            {
                checkEqual(rReference[i], rActual[i]);
            }
        }
    }

    static void checkEqual(
        const whiteboard::WrapperFor32BitPointer<const char>& rReference,
        const whiteboard::WrapperFor32BitPointer<const char>& rActual);

    template <typename T>
    static void checkEqual(const whiteboard::WrapperFor32BitPointer<T>& rReference, const whiteboard::WrapperFor32BitPointer<T>& rActual)
    {
        const T* pReferenceSubStruct = rReference.operator const T *();
        const T* pActualSubStruct = rActual.operator const T *();
        EXPECT_EQ(pReferenceSubStruct != NULL, pActualSubStruct != NULL);
        if ((pReferenceSubStruct != NULL) && (pActualSubStruct != NULL))
        {
            checkEqual(*pReferenceSubStruct, *pActualSubStruct);
        }
    }

    static void checkEqual(const WB_RES::TestStructure& rReference, const WB_RES::TestStructure& rActual);

    static void checkEqual(
        const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rReference,
        const WB_RES::AllPrimitiveAndBuiltinTypesInAStructure& rActual);

    static void checkEqual(
        const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rReference,
        const WB_RES::AllOptionalPrimitiveAndBuiltinTypesInAStructure& rActual);

    static void checkEqual(
        const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rReference,
        const WB_RES::AllArrayPrimitiveAndBuiltinTypesInAStructure& rActual);

    static void checkEqual(
        const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rReference,
        const WB_RES::AllOptionalArrayPrimitiveAndBuiltinTypesInAStructure& rActual);
};
