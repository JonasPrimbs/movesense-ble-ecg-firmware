#include "EcgGATTSvcClient.h"
#include "WakeClient.h"

#include "movesense.h"

MOVESENSE_APPLICATION_STACKSIZE(1024)

// Add Providers:

MOVESENSE_PROVIDERS_BEGIN(2)

MOVESENSE_PROVIDER_DEF(EcgGATTSvcClient)
MOVESENSE_PROVIDER_DEF(WakeClient)

MOVESENSE_PROVIDERS_END(2)

// Load Features:

MOVESENSE_FEATURES_BEGIN()

OPTIONAL_CORE_MODULE(DataLogger, false)
OPTIONAL_CORE_MODULE(Logbook, false)
OPTIONAL_CORE_MODULE(LedService, true)
OPTIONAL_CORE_MODULE(IndicationService, true)
OPTIONAL_CORE_MODULE(BleService, true)
OPTIONAL_CORE_MODULE(EepromService, false)
OPTIONAL_CORE_MODULE(BypassService, false)
OPTIONAL_CORE_MODULE(SystemMemoryService, false)
OPTIONAL_CORE_MODULE(DebugService, false)
OPTIONAL_CORE_MODULE(BleStandardHRS, true)
OPTIONAL_CORE_MODULE(BleNordicUART, false)
OPTIONAL_CORE_MODULE(CustomGattService, true)

// NOTE: If building a simulator build, these macros are obligatory!
// DEBUGSERVICE_BUFFER_SIZE(6, 120); // 6 lines, 120 characters total
// DEBUG_EEPROM_MEMORY_AREA(false, 0, 2048) // EEPROM storage disabled
// LOGBOOK_MEMORY_AREA(0, MEMORY_SIZE_FILL_REST);

// Define Application Specification:

APPINFO_NAME("EcgGATTSvc");
APPINFO_VERSION("0.2.0");
APPINFO_COMPANY("Jonas Primbs");

// Enable Bluetooth Low Energy Communication:

BLE_COMMUNICATION(true)

MOVESENSE_FEATURES_END()
