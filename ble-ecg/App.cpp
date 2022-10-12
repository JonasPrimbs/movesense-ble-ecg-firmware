// #include "EcgGATTSvcClient.h"
// #include "MovGATTSvcClient.h"
#include "ActivityGATTSvcClient.h"
#include "WakeClient.h"

#include "movesense.h"

MOVESENSE_APPLICATION_STACKSIZE(1024)

// Add Providers:

MOVESENSE_PROVIDERS_BEGIN(2)

// MOVESENSE_PROVIDER_DEF(EcgGATTSvcClient)
MOVESENSE_PROVIDER_DEF(ActivityGATTSvcClient)
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

// Define Application Specification:

APPINFO_NAME("ActivityGATTSvc");
APPINFO_VERSION("0.4.0");
APPINFO_COMPANY("Jonas Primbs");

// Enable Bluetooth Low Energy Communication:

BLE_COMMUNICATION(true)

MOVESENSE_FEATURES_END()
