#include "BleController.hpp"

class CustomBleController : public BleController
{
public:
    CustomBleController();

protected:
    uint32_t PrepareAdvertisingData(uint8_t * buffer, uint32_t max_len) override;
    uint32_t PrepareScanResponseData(uint8_t * buffer, uint32_t max_len) override;
    void OnHrsNotificationEnabled() override;
    void OnHrsNotificationDisabled() override;
    void OnUartData(uint8_t * p_data, uint16_t length) override;
};

