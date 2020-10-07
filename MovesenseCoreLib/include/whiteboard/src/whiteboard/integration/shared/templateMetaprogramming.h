#pragma once
/******************************************************************************
Copyright (c) Suunto Oy 2015.
All rights reserved.
******************************************************************************/

#include "whiteboard/integration/shared/types.h"
#include "whiteboard/integration/shared/pack.h"

namespace whiteboard
{

/***
If T and U name the same type with the same const-volatile qualifications,
provides the member constant value equal to true. Otherwise value is false.

TODO: If all compilers are c++11 compliant, this should be replaced with std::is_same
*/
template <typename T, typename U> struct IsSame
{
    enum
    {
        value = 0
    };
};

template <typename T> struct IsSame<T, T>
{
    enum
    {
        value = 1
    };
};

// Template that checks whether given type is a const type using SFINAE test
template <typename T> struct IsConst
{
    static bool const value = false;
};
template <typename T> struct IsConst<const T>
{
    static bool const value = true;
};
template <typename T> struct IsConst<const T&>
{
    static bool const value = true;
};
template <typename T> struct IsConst<const T*>
{
    static bool const value = true;
};

// Template that checks whether given type is a reference type using SFINAE test
template <typename T> struct IsReference
{
    static bool const value = false;
};
template <typename T> struct IsReference<T&>
{
    static bool const value = true;
};

// Template that checks whether given type is a pointer type using SFINAE test
template <typename T> struct IsPointer
{
    static bool const value = false;
};
template <typename T> struct IsPointer<T*>
{
    static bool const value = true;
};

// Template that removes const from the given type
template <typename T> struct RemoveConstRecursive
{
    typedef T type;
};
template <typename T> struct RemoveConstRecursive<T const>
{
    typedef typename RemoveConstRecursive<T>::type type;
};
template <typename T> struct RemoveConstRecursive<T&>
{
    typedef typename RemoveConstRecursive<T>::type& type;
};
template <typename T> struct RemoveConstRecursive<T*>
{
    typedef typename RemoveConstRecursive<T>::type* type;
};

// Template that removes reference from the given type
template <typename T> struct RemoveReference
{
    typedef T type;
};
template <typename T> struct RemoveReference<T&>
{
    typedef T type;
};

// Template that removes pointer from the given type
template <typename T> struct RemovePointer
{
    typedef T type;
};
template <typename T> struct RemovePointer<T*>
{
    typedef T type;
};
template <typename T> struct RemovePointer<T* const>
{
    typedef T type;
};

// Remove consts and references (but not pointers)
template <typename T> struct RemoveAll
{
    typedef typename RemoveReference<typename RemoveConstRecursive<T>::type>::type type;
};

// Template struct that has non-zero value when type is a class, struct or union
template <typename T>
struct IsClassOrUnion
{
private:
    template <typename C>
    static char test(int C::*);
    
    struct two { char c[2]; };
    template <class C>
    static two test(...);

public:
    static const bool value = sizeof(test<T>(0)) == 1;
};

enum _isClassOrUnionTestEnum {};

WB_STATIC_VERIFY(IsClassOrUnion<char>::value == false, IsClassOrUnion_char_failure);
WB_STATIC_VERIFY(IsClassOrUnion<int>::value == false, IsClassOrUnion_int_failure);
WB_STATIC_VERIFY(IsClassOrUnion<float>::value == false, IsClassOrUnion_float_failure);
WB_STATIC_VERIFY(IsClassOrUnion<void*>::value == false, IsClassOrUnion_voidptr_failure);
WB_STATIC_VERIFY(IsClassOrUnion<struct _isClassOrUnionTestA>::value == true, IsClassOrUnion_struct_failure);
WB_STATIC_VERIFY(IsClassOrUnion<class _isClassOrUnionTestB>::value == true, IsClassOrUnion_class_failure);
WB_STATIC_VERIFY(IsClassOrUnion<union _isClassOrUnionTestC>::value == true, IsClassOrUnion_union_failure);
WB_STATIC_VERIFY(IsClassOrUnion<_isClassOrUnionTestEnum>::value == false, IsClassOrUnion_enum_failure);
WB_STATIC_VERIFY(IsClassOrUnion<struct _isClassOrUnionTestE*>::value == false, IsClassOrUnion_struct_voidptr_failure);
WB_STATIC_VERIFY(IsClassOrUnion<class _isClassOrUnionTestF*>::value == false, IsClassOrUnion_class_voidptr_failure);
WB_STATIC_VERIFY(IsClassOrUnion<union _isClassOrUnionTestG*>::value == false, IsClassOrUnion_union_voidptr_failure);
WB_STATIC_VERIFY(IsClassOrUnion<_isClassOrUnionTestEnum*> ::value == false, IsClassOrUnion_enum_voidptr_failure);

/** Template that can be used to pass pass parameters by reference. */
template <typename T>
struct ByRef
{
    typedef typename RemoveReference<T>::type& type;
};

template <typename T>
struct ByRef<T*>
{
    typedef typename RemoveReference<T>::type* type;
};

/** Template for conditionally enabling template functions.
*
* @see C++11 std::enable_if
*/
template<bool B, class T = void>
struct EnableIf {};

template<class T>
struct EnableIf<true, T> { typedef T type; };

// Template struct for calculating real data type alignments
template <typename Type> struct TypeAlignTest
{
    uint8 dummy;
    Type data;
};

/** Macro that calculates alignment of data type at compile time
 *
 * @note This used to be metaprogramming template, but GCC cannot 
 * use constant integers (static const int x = 1) in attribute(aligned) definitions.
 *
 * @param type Type which alingment needs to be calculated
 * @return Data type alignment
 */
#define WB_TYPE_ALIGNMENT(type)  WB_OFFSETOF(::whiteboard::TypeAlignTest<type>, data)

/** Template to get a type which size is given as template parameter and is aligned on that
  * same byte boundary. */
template <size_t TYPE_SIZE_AND_ALIGNMENT> struct TypeOfSizeAndAlignment {};
template <> struct TypeOfSizeAndAlignment<1> { WB_ALIGN(1) uint8 a; };
template <> struct TypeOfSizeAndAlignment<2> { WB_ALIGN(2) uint16 a; };
template <> struct TypeOfSizeAndAlignment<4> { WB_ALIGN(4) uint32 a; };
template <> struct TypeOfSizeAndAlignment<8> { WB_ALIGN(8) uint64 a; };

WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(TypeOfSizeAndAlignment<1>) == 1, TypeOfSizeAndAlignment_1_IsBroken);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(TypeOfSizeAndAlignment<2>) == 2, TypeOfSizeAndAlignment_2_IsBroken);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(TypeOfSizeAndAlignment<4>) == 4, TypeOfSizeAndAlignment_4_IsBroken);
WB_STATIC_VERIFY(WB_TYPE_ALIGNMENT(TypeOfSizeAndAlignment<8>) == 8, TypeOfSizeAndAlignment_8_IsBroken);

/** Macro for reserving storage space for an object without constructing the object
 *
 * @note Remember to use WB_NO_INIT where applicable
 */
#define WB_MEMORY_RESERVATION(identifier, Type) \
    TypeOfSizeAndAlignment<WB_TYPE_ALIGNMENT(Type)> identifier[sizeof(Type) / sizeof(TypeOfSizeAndAlignment<WB_TYPE_ALIGNMENT(Type)>)]

} // namespace whiteboard
