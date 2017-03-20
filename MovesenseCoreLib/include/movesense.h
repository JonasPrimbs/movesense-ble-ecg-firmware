/*
 * Main API include for making Movesense sensor apps
 *
 */
 
#pragma once

#include "whiteboard/LaunchableModule.h"

#if __cplusplus
extern "C" {
#endif // __cplusplus

    void* memalloc(size_t size);
    void* memcalloc(size_t size);
    void memfree(void* ptr);

    /* memrealloc function has been removed. Existing implementation
       added sizeof(size_t) amount of extra overhead to every memory
       allocation. Existance of this function also engourages to do
       designs that use dynamic memory management, that should be
       avoid in (semi) life critical applications.
    
       If you have to have memrealloc functionality, don't reimplement
       it here. Instead create another layer for allocating reallocatable
       buffers and this way avoid extra overhead to all other allocations
    
    void* memrealloc(void* ptr, size_t size);
    */
#if __cplusplus
} // extern "C"
#endif // __cplusplus

// Declaration of callback that gets called just before launchable modules are started
typedef void (*PreLaunchCallback)(void);  
extern PreLaunchCallback __preLaunchCallback;

// Array to hold app's own module names
extern const char * const*__MOVESENSE_APP_SPECIFIC_MODULES;

// Main app thread stack size
extern uint16_t g_WB_EXEC_CTX_APPLICATION_STACKSIZE;
// and macro to set it
#define MOVESENSE_APPLICATION_STACKSIZE(stackSizeInWords) uint16_t g_WB_EXEC_CTX_APPLICATION_STACKSIZE = (stackSizeInWords);

// define macros that app use to specify their own launchable modules. Structurally result is a class with preLaunchHappened callback and array of the created modules. Modules are reserved from the heap
#define MOVESENSE_PROVIDERS_BEGIN(NumModules) \
class __MovesenseAppModuleHolder {\
public:\
    static whiteboard::LaunchableModule* __moduleObjs[NumModules+1];\
    static const char * __moduleNames[NumModules+1];\
    __MovesenseAppModuleHolder() {for (int i=0;i<NumModules+1;i++) {__moduleNames[i]=NULL;__moduleObjs[i]=NULL;} __preLaunchCallback = __MovesenseAppModuleHolder::preLaunchHappened;}\
    static void preLaunchHappened() {int __index = 0;


#define MOVESENSE_PROVIDER_DEF(ProviderClass) {\
        whiteboard::LaunchableModule* __module = static_cast< whiteboard::LaunchableModule* >(memalloc(sizeof(ProviderClass))); \
        __moduleObjs[__index] = __module; \
        __moduleNames[__index] = ProviderClass::LAUNCHABLE_NAME;\
        new (__module) ProviderClass; __index++; }

#define MOVESENSE_PROVIDERS_END(NumModules) \
    }  \
};  \
__MovesenseAppModuleHolder __launchableModuleHolder; const char * __MovesenseAppModuleHolder::__moduleNames[NumModules+1]; whiteboard::LaunchableModule* __MovesenseAppModuleHolder::__moduleObjs[NumModules+1];\
const char *const *__MOVESENSE_APP_SPECIFIC_MODULES = __MovesenseAppModuleHolder::__moduleNames;

extern const bool g_enableSerialComm;
extern const bool g_enableBLEComm;

#define MOVESENSE_FEATURES_BEGIN()

#define SERIAL_COMMUNICATION(enable) const bool g_enableSerialComm = (enable);

#define BLE_COMMUNICATION(enable) const bool g_enableBLEComm = (enable);

#define MOVESENSE_FEATURES_END()
