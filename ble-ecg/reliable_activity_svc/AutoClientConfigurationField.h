#pragma once
#include "movesense.h"

/** Definition of the configuration field characteristic structure **/
struct AutoClientConfigurationField final
{
    uint8_t ecgMeasurementInterval;
    uint8_t movMeasurementInterval;
    uint8_t ecgRecordingMode;
    uint8_t movRecordingMode;
    uint8_t recordingOperation;
    uint8_t getDataOperation;
    uint8_t deleteDataOperation;
    uint8_t reserved;
    int64_t timestamp;
};