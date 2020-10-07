#pragma once
// Copyright (c) Suunto Oy 2017. All rights reserved.

#include "whiteboard/integration/port.h"

namespace whiteboard {

/** Loads library
*
* @param fileName Path to the library
* @return Handle to the library or NULL if the library could not be loaded
*/
void* WbDynamicLibraryLoad(const char* fileName);

/** Unloads library
*
* @param pLibrary Handle to the library
*/
void WbDynamicLibraryUnload(void* pLibrary);

/** Finds function entry point from a library
*
* @param functionName Name of the function
* @return Address of the function or NULL if the entry point was not found
*/
void* WbDynamicLibraryGetFunc(void* pLibrary, const char* functionName);

/** Scoped handling of dynamic-link libraries */
class DynamicLinkLibrary
{
public:
    /** Constructor
    *
    * @param filename Path to the dynamic library
    */
    DynamicLinkLibrary(const char* fileName)
        : mpLibrary(WbDynamicLibraryLoad(fileName))
    {
        if (!isOk())
        {
            WB_DEBUGLOG("Failed to load library %s", fileName);
        }
    }

    /** Destructor */
    ~DynamicLinkLibrary()
    {
        if (isOk())
        {
            WbDynamicLibraryUnload(mpLibrary);
        }
    }

    /** Checks whether library is successfully loaded 
     *
     * @return A value indicating whether the library is usable
     */
    bool isOk() const
    {
        return mpLibrary != NULL;
    }

    /** Creates a new instance of object with type T
    *
    * @param entryPoint Name of the entry point function
    * @param param Parameter to the entry point function
    * @return New instance or NULL if dll could not be loaded or entry point was not found
    */
    template <typename T, typename P>
    T* createInterface(const char* entryPoint, P param)
    {
        void* pEntryPoint = getEntryPoint(entryPoint);
        if (pEntryPoint == NULL)
        {
            return NULL;
        }

        typedef T* (*EntryPointFunc)(P);
        return static_cast<EntryPointFunc>(pEntryPoint)(param);
    }

    /** Creates a new instance of object with type T
    *
    * @param entryPoint Name of the entry point function
    * @param param1 First parameter to the entry point function
    * @param param2 Second parameter to the entry point function
    * @return New instance or NULL if dll could not be loaded or entry point was not found
    */
    template <typename T, typename P1, typename P2>
    T* createInterface(const char* entryPoint, P1 param1, P2 param2)
    {
        void* pEntryPoint = getEntryPoint(entryPoint);
        if (pEntryPoint == NULL)
        {
            return NULL;
        }

        typedef T* (*EntryPointFunc)(P1, P2);
        return static_cast<EntryPointFunc>(pEntryPoint)(param1, param2);
    }

    /** Creates a new instance of object with type T
    *
    * @param entryPoint Name of the entry point function
    * @param param1 First parameter to the entry point function
    * @param param2 Second parameter to the entry point function
    * @param param3 Third parameter to the entry point function
    * @return New instance or NULL if dll could not be loaded or entry point was not found
    */
    template <typename T, typename P1, typename P2, typename P3>
    T * createInterface(const char* entryPoint, P1 param1, P2 param2, P3 param3)
    {
        void* pEntryPoint = getEntryPoint(entryPoint);
        if (pEntryPoint == NULL)
        {
            return NULL;
        }

        typedef T* (*EntryPointFunc)(P1, P2, P3);
        return static_cast<EntryPointFunc>(pEntryPoint)(param1, param2, param3);
    }

    /** Creates a new instance of object with type T
     *
     * @param entryPoint Name of the entry point function
     * @param param1 First parameter to the entry point function
     * @param param2 Second parameter to the entry point function
     * @param param3 Third parameter to the entry point function
     * @param param4 Fourth parameter to the entry point function
     * @return New instance or NULL if dll could not be loaded or entry point was not found
     */
    template <typename T, typename P1, typename P2, typename P3, typename P4>
    T* createInterface(const char* entryPoint, P1 param1, P2 param2, P3 param3, P4 param4)
    {
        void* pEntryPoint = getEntryPoint(entryPoint);
        if (pEntryPoint == NULL)
        {
            return NULL;
        }

        typedef T* (*EntryPointFunc)(P1, P2, P3, P4);
        return static_cast<EntryPointFunc>(pEntryPoint)(param1, param2, param3, param4);
    }


private:
    void* getEntryPoint(const char* entryPoint)
    {
        if (!isOk())
        {
            return NULL;
        }

        void* pEntryPoint = WbDynamicLibraryGetFunc(mpLibrary, entryPoint);
        if (pEntryPoint == NULL)
        {
            WB_DEBUGLOG("Failed to find function %s", entryPoint);
            return NULL;
        }

        return pEntryPoint;
    }

private:
    /** Handle to the library */
    void* mpLibrary;
};

} // namespace whiteboard
