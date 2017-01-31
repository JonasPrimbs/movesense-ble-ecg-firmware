#include "movesense.h"
#include "HelloWorldService.h"
#include "AccelerometerSampleService.h"


MOVESENSE_PROVIDERS_BEGIN(2)

MOVESENSE_PROVIDER_DEF(HelloWorldService)
MOVESENSE_PROVIDER_DEF(AccelerometerSampleService)

MOVESENSE_PROVIDERS_END(2)

