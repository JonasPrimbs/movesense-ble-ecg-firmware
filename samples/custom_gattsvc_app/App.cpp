#include "CustomGATTSvcClient.h"
#include "movesense.h"

MOVESENSE_APPLICATION_STACKSIZE(1024)

MOVESENSE_PROVIDERS_BEGIN(1)

MOVESENSE_PROVIDER_DEF(CustomGATTSvcClient)

MOVESENSE_PROVIDERS_END(1)

MOVESENSE_FEATURES_BEGIN()
// Explicitly enable or disable Movesense framework core modules.
// List of modules and their default state is found in documentation
OPTIONAL_CORE_MODULE(DataLogger, false)
OPTIONAL_CORE_MODULE(Logbook, false)
OPTIONAL_CORE_MODULE(LedService, true)
OPTIONAL_CORE_MODULE(IndicationService, true)
OPTIONAL_CORE_MODULE(BleService, true)
OPTIONAL_CORE_MODULE(EepromService, false)
OPTIONAL_CORE_MODULE(CustomGattService, true)

APPINFO_NAME("Sample CustomGATTSvc");
APPINFO_VERSION("1.0.0");
APPINFO_COMPANY("Movesense");

SERIAL_COMMUNICATION(true) // Warning: enabling this may increase power consumption (depends on UART setting)
BLE_COMMUNICATION(true)
MOVESENSE_FEATURES_END()
