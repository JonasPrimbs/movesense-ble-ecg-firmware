#pragma once
// Copyright (c) Suunto Oy 2016. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard
{

/** Linked list operations. To avoid dynamic memory use and any awkward inheritance
* patterns Whiteboard objects in linked lists have member variables for linking them
* together.
*
* In addition to using member variables, functions in this class have generic although
* less readable implementations to avoid excess ROM usage of templated code.
*/
class LinkedList
{
public:
    /** Adds new item to first in the linked list
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @param rpFirst Pointer to first item
    * @param pNewValue New value to add
    */
    template <typename T, T* T::*pNext>
    inline static void addFirst(T*& rpFirst, T* pNewValue)
    {
        addFirstImpl(reinterpret_cast<void*&>(rpFirst), pNewValue, getMemberOffset<T, pNext>());
    }

    /** Adds new item to last in the linked list
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @param rpFirst Pointer to first item
    * @param pNewValue New value to add
    */
    template <typename T, T* T::*pNext>
    inline static void addLast(T*& rpFirst, T* pNewValue)
    {
        addLastImpl(reinterpret_cast<void*&>(rpFirst), pNewValue, getMemberOffset<T, pNext>());
    }

    /** Adds new item to sorted linked list
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @param rpFirst Pointer to first item
    * @param pNewValue New value to add
    * @param isLess Function that checks whether first item is less than second. Used for ordering items.
    */
    template <typename T, T* T::*pNext>
    inline static void addSorted(T*& rpFirst, T* pNewValue, bool(*isLess)(const T*, const T*))
    {
        addSortedImpl(
            reinterpret_cast<void*&>(rpFirst),
            pNewValue,
            reinterpret_cast<bool(*)(const void*, const void*)>(isLess),
            getMemberOffset<T, pNext>());
    }

    /** Removes item from the linked list
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @param rpFirst Pointer to first item
    * @param pRemoveValue Value to remove
    * @return A value indicating whther the item was removed
    */
    template <typename T, T* T::*pNext>
    inline static bool remove(T*& rpFirst, const T* pRemoveValue)
    {
        return removeImpl(reinterpret_cast<void*&>(rpFirst), pRemoveValue, getMemberOffset<T, pNext>());
    }

    /** Removes all items from the linked list
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @param rpFirst Pointer to first item
    */
    template <typename T, T* T::*pNext>
    inline static void removeAll(T*& rpFirst)
    {
        removeAllImpl(reinterpret_cast<void*&>(rpFirst), getMemberOffset<T, pNext>());
    }

    /** Finds item from linked list
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @param pFirst Pointer to first item
    * @param compare Function that compares list item with given comparand.
    *                Negative return value continues iteration.
    *                Zero returns current item.
    *                Positive value ends iteraton and returns NULL.
    * @param pComparand Comparand for comparison
    */
    template <typename T, T* T::*pNext>
    inline static T* find(T* pFirst, int(*compare)(const T* pItem, const void* pComparand), const void* pComparand)
    {
        return static_cast<T*>(findImpl(
            pFirst,
            reinterpret_cast<int(*)(const void*, const void*)>(compare),
            pComparand,
            getMemberOffset<T, pNext>()));
    }

private:
    /** Helper function for calculating member offset
    *
    * @tparam T Type of the linked list object
    * @tparam pNext Pointer to member variable that stores link to next object
    * @return Member offset of next pointer
    */
    template <typename T, T* T::*pNext>
    inline static size_t getMemberOffset()
    {
        return reinterpret_cast<size_t>(&(((T*)0)->*pNext));
    }

    /** Helper function for getting pointer to next pointer
    *
    * @param pObject Object which member variable should be returned
    * @param memberOffset Offset of next pointer member variable
    * @return Pointer to next pointer
    */
    inline static void** nextPtr(void* pObject, size_t memberOffset)
    {
        return reinterpret_cast<void**>(reinterpret_cast<size_t>(pObject) + memberOffset);
    }

    /** Generic implementation of adding object to first in linked list
    *
    * @param rpFirst Pointer to first item
    * @param pNewValue New value to add
    * @param memberOffset Offset of next pointer member variable
    */
    static void addFirstImpl(void*& rpFirst, void* pNewValue, size_t memberOffset);

    /** Generic implementation of adding object to last in linked list
    *
    * @param rpFirst Pointer to first item
    * @param pNewValue New value to add
    * @param memberOffset Offset of next pointer member variable
    */
    static void addLastImpl(void*& rpFirst, void* pNewValue, size_t memberOffset);

    /** Generic implementation of adding object to sorted linked list
    *
    * @param rpFirst Pointer to first item
    * @param pNewValue New value to add
    * @param isLess Function that checks whether first item is less than second. Used for ordering items.
    * @param memberOffset Offset of next pointer member variable
    */
    static void addSortedImpl(void*& rpFirst, void* pNewValue, bool(*isLess)(const void*, const void*), size_t memberOffset);

    /** Generic implementation of removing object from linked list
    *
    * @param rpFirst Pointer to first item
    * @param pRemoveValue Value to remove
    * @param memberOffset Offset of next pointer member variable
    * @return A value indicating whther the item was removed
    */
    static bool removeImpl(void*& rpFirst, const void* pRemoveValue, size_t memberOffset);

    /** Generic implementation of removing all objects from linked list
    *
    * @param rpFirst Pointer to first item
    * @param memberOffset Offset of next pointer member variable
    */
    static void removeAllImpl(void*& rpFirst, size_t memberOffset);

    /** Generic implementation for finding an object from linked list
    *
    * @param pFirst Pointer to first item
    * @param compare Function that compares list item with given comparand.
    *                Negative return value continues iteration.
    *                Zero returns currrent item.
    *                Positive value ends iteraton and returns NULL.
    * @param pComparand Comparand for comparison
    * @param memberOffset Offset of next pointer member variable
    */
    static void* findImpl(void* pFirst, int(*compare)(const void*, const void*), const void* pComparand, size_t memberOffset);
};

} // namespace whiteboard
