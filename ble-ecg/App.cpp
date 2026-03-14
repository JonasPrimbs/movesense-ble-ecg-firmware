#include "WakeClient.h"
#include "offline_storage_client/MainClient.h"
#include "offline_storage_client/MeasurementProvider.h"

#include "movesense.h"

MOVESENSE_APPLICATION_STACKSIZE(1024)

// Add Providers:
MOVESENSE_PROVIDERS_BEGIN(3)

MOVESENSE_PROVIDER_DEF(MainClient)
MOVESENSE_PROVIDER_DEF(WakeClient)
MOVESENSE_PROVIDER_DEF(MeasurementProvider)

MOVESENSE_PROVIDERS_END(3)

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

APPINFO_NAME("ReliableActivityService");
APPINFO_VERSION("0.9.0");
APPINFO_COMPANY("Jonas Primbs");

// Enable Bluetooth Low Energy Communication:

MOVESENSE_FEATURES_END()
