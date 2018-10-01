#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"

namespace whiteboard
{

// Forward declarations
class WrapperFor32BitPointerAccessor;

#if defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(_M_AMD64) || defined(__LP64__)
    
WB_STATIC_VERIFY(sizeof(void*) > 4, SizeOf_Pointer_Is_Not_Greater_Than_4_Bytes);

#define WB_NEED_POINTER_WRAPPER_POOL // This is so that we can adapt rest of the code to do necessary init/deinit of the pool (mutex) & structure cleanup
#define WB_MAX_64BIT_POINTER_WRAPPERS 1024*16 // Size of this really dont matter, max can be pretty big as initial size is small and size grows dynamically on need basis

/** Type of ID used to map pointers */
typedef uint32 WrappedPointerId;

/** Helper class for doing 32-bit to 64-bit pointer wrapping. Hides actual implementation. */
class PointerWrapperHelper
{
public:
    /** Invalid pointer ID */
    static const WrappedPointerId INVALID_WRAPPER_ID = 0U; // use 0 for invalid ID to allow clear by memset

    /** Create internal allocations */
    static void create();

    /** Destroy internal allocations */
    static void destroy();

    /** Allocates new pointer wrapper for given pointer */
    static WrappedPointerId allocate(const void* pointer);

    /** Assigns new value over existing pointer wrapper
     *
     * @param id ID of existing poiinter wrapper
     * @param pointer New pointer value
     * @return New ID for the wrapper
     */
    static WrappedPointerId overwrite(WrappedPointerId id, const void* pointer);

    /** Gets pointer value of given wrapper */
    static const void* get(WrappedPointerId id);
};

/** Pointer wrapper implementation for >= 32 bit pointers
*
* @tparam ItemType Type of the pointer
*/
template <typename ItemType> class WrapperFor32BitPointer
{
private:
    /** Library internal implementation can access these members */
    friend class WrapperFor32BitPointerAccessor;

    /** Pointer ID in the registry */
    WrappedPointerId mWrappedPointerId;

    /** Internal assign method used by the structure deserializer, that does not free existing pointer from the registry
    *
    * @param pointer New value for the pointer
    * @return Reference to this object
    */
    inline WrapperFor32BitPointer& assign(const void* pointer)
    {
        mWrappedPointerId = PointerWrapperHelper::allocate(pointer);
        return *this;
    }

public:
    /** Default constructor */
    inline WrapperFor32BitPointer() 
        : mWrappedPointerId(PointerWrapperHelper::INVALID_WRAPPER_ID)
    {}

    /** Copy constructor */
    inline WrapperFor32BitPointer(const WrapperFor32BitPointer& rOther)
        : mWrappedPointerId(PointerWrapperHelper::INVALID_WRAPPER_ID)
    {
        *this = rOther.operator const ItemType*();
    }

    /** Constructor that initializes the wrapper using given pointer value
     *
     * @param pointer Pointer value to use
     */
    inline WrapperFor32BitPointer(const void* pointer)
        : mWrappedPointerId(PointerWrapperHelper::INVALID_WRAPPER_ID)
    {
        *this = pointer;
    }

    /** Destructor */
    inline ~WrapperFor32BitPointer()
    { 
        *this = NULL;
    }

    /** Assignment operator
    *
    * @param rOther Another pointer wrapper which value should be used
    * @return Reference to this class
    */
    inline WrapperFor32BitPointer& operator=(const WrapperFor32BitPointer& rOther)
    {
        return *this = rOther.operator const ItemType*();
    }

    /** Assigns pointer value to the wrapper
     *
     * @param pointer Pointer which value should be used
     * @return Reference to this class
     */
    inline WrapperFor32BitPointer& operator=(const void* pointer)
    {
        mWrappedPointerId = PointerWrapperHelper::overwrite(mWrappedPointerId, pointer);
        return *this;
    }

    /** Cast operator. Explicitly casts the wrapped pointer back to its original type
     *
     * @return Value of wrapped pointer
     */
    inline operator const ItemType*() const
    {
        return static_cast<const ItemType*>(PointerWrapperHelper::get(mWrappedPointerId));
    }
};

#else

WB_STATIC_VERIFY(sizeof(void*) == 4, SizeOf_Pointer_Is_Not_4_Bytes);

/** Pointer wrapper implementation for 32 bit pointers
 *
 * @tparam ItemType Type of the pointer
 */
template <typename ItemType> class WrapperFor32BitPointer
{
private:
    /** Library internal implementation can access these members */
    friend class WrapperFor32BitPointerAccessor;

    /** Actual pointer value */
    const void* mPointer;

    /** Assigns a new value to the pointer
     *
     * @note This is to match with the 64bit version, which needs this method
     * @param pointer New value for the pointer
     * @return Reference to this object
     */
    inline WrapperFor32BitPointer& assign(const void* pointer) { return operator=(pointer); }

public:
    /** Default constructor */
    inline WrapperFor32BitPointer() : mPointer(NULL) {}

    /** Copy constructor */
    inline WrapperFor32BitPointer(const WrapperFor32BitPointer& rOther) { *this = rOther.operator const ItemType*(); }

    /** Constructor that initializes the wrapper using given pointer value
    *
    * @param pointer Pointer value to use
    */
    inline WrapperFor32BitPointer(const void* pointer) { *this = pointer; }

    /** Destructor */
    inline ~WrapperFor32BitPointer() {}

    /** Assignment operator
    *
    * @param rOther Another pointer wrapper which value should be used
    * @return Reference to this class
    */
    inline WrapperFor32BitPointer& operator=(const WrapperFor32BitPointer& rOther)
    {
        return * this = rOther.operator const ItemType*();
    }

    /** Assigns pointer value to the wrapper
    *
    * @param pointer Pointer which value should be used
    * @return Reference to this class
    */
    inline WrapperFor32BitPointer& operator=(const void* pointer)
    {
        mPointer = pointer;
        return *this;
    }

    /** Cast operator. Explicitly casts the wrapped pointer back to its original type
    *
    * @return Value of wrapped pointer
    */
    inline operator const ItemType*() const { return static_cast<const ItemType*>(mPointer); }
};

#endif

WB_STATIC_VERIFY(sizeof(WrapperFor32BitPointer<void>) == 4, SizeOf_WrapperFor32BitPointer_Is_Not_4_Bytes);

#ifndef WB_NEED_POINTER_WRAPPER_POOL
#define WB_WHEN_STRUCTURE_CLEANING_NEEDED(stmt)
#else
#define WB_NEED_STRUCTURE_CLEANING
#define WB_WHEN_STRUCTURE_CLEANING_NEEDED(stmt)   stmt
#endif

} // namespace whiteboard
