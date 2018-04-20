#include "BleController.hpp"

class CustomBleController : public BleController
{
public:
    CustomBleController();

protected:
    void onBleInit() override;
    uint32_t PrepareAdvertisingData(uint8_t * buffer, uint32_t max_len) override;
    uint32_t PrepareScanResponseData(uint8_t * buffer, uint32_t max_len) override;
};

