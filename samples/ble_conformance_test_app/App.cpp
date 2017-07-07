#include "movesense.h"

#include "BleConformanceTesterService.h"
#include "BleTestService.hpp"

MOVESENSE_APPLICATION_STACKSIZE(1024)

MOVESENSE_PROVIDERS_BEGIN(2)

MOVESENSE_PROVIDER_DEF(BleTestService)
MOVESENSE_PROVIDER_DEF(BleConformanceTesterService)

MOVESENSE_PROVIDERS_END(2)

MOVESENSE_FEATURES_BEGIN()
// Serial communication must be disabled so that the battery has enough power for running the tests
SERIAL_COMMUNICATION(false)
BLE_COMMUNICATION(true)
MOVESENSE_FEATURES_END()
