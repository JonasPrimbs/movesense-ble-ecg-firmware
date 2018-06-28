#include "movesense.h"

#include "BleConformanceTesterService.h"
#include "BleTestService.hpp"

MOVESENSE_APPLICATION_STACKSIZE(1024)

MOVESENSE_PROVIDERS_BEGIN(2)

MOVESENSE_PROVIDER_DEF(BleTestService)
MOVESENSE_PROVIDER_DEF(BleConformanceTesterService)

MOVESENSE_PROVIDERS_END(2)

MOVESENSE_FEATURES_BEGIN()
// Explicitly enable or disable Movesense framework core modules.
// List of modules and their default state is found in documentation
OPTIONAL_CORE_MODULE(DataLogger, true)
OPTIONAL_CORE_MODULE(Logbook, true)
OPTIONAL_CORE_MODULE(LedService, true)
OPTIONAL_CORE_MODULE(IndicationService, true)
OPTIONAL_CORE_MODULE(BleService, true)
OPTIONAL_CORE_MODULE(EepromService, false)
OPTIONAL_CORE_MODULE(BypassService, false)
OPTIONAL_CORE_MODULE(SystemMemoryService, false)
OPTIONAL_CORE_MODULE(DebugService, false)
OPTIONAL_CORE_MODULE(BleStandardHRS, false)
OPTIONAL_CORE_MODULE(BleNordicUART, false)

APPINFO_NAME("Sample BLE Conformance Test");
APPINFO_VERSION("1.0.0");
APPINFO_COMPANY("Movesense");

// Serial communication must be disabled so that the battery has enough power for running the tests
// NOTE: SERIAL_COMMUNICATION macro has been DEPRECATED
BLE_COMMUNICATION(true)
MOVESENSE_FEATURES_END()
