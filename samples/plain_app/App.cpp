#include "movesense.h"

MOVESENSE_APPLICATION_STACKSIZE(1024)

MOVESENSE_PROVIDERS_BEGIN(0)
MOVESENSE_PROVIDERS_END(0)

MOVESENSE_FEATURES_BEGIN()
// Explicitly enable or disable Movesense framework core modules.
// List of modules and their default state is found in documentation
OPTIONAL_CORE_MODULE(DataLogger, true)
OPTIONAL_CORE_MODULE(Logbook, true)
OPTIONAL_CORE_MODULE(LedService, true)
OPTIONAL_CORE_MODULE(IndicationService, true)
OPTIONAL_CORE_MODULE(BleService, true)
OPTIONAL_CORE_MODULE(EepromService, true)

// NOTE: It is inadvisable to enable both Logbook/DataLogger and EepromService without 
// explicit definition of Logbook memory are (see LOGBOOK_MEMORY_AREA macro in movesense.h and eeprom_logbook_app).
// Default setting is for Logbook to use the whole EEPROM memory area.
// LOGBOOK_MEMORY_AREA(offset, size);

APPINFO_NAME("Sample Plain");
APPINFO_VERSION("1.0.0");
APPINFO_COMPANY("Movesense");

SERIAL_COMMUNICATION(false) // Warning: enabling this feature will increase power consumption
BLE_COMMUNICATION(true)
MOVESENSE_FEATURES_END()
