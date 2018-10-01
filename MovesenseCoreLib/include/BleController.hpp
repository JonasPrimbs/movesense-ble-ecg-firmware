#pragma once

#include <stddef.h>

#define DEFAULT_FAST_ADVERTISING_TIME 60        // 60 seconds
#define DEFAULT_SLOW_ADVERTISING_TIME 0         // 0 = Infinite
#define DEFAULT_FAST_ADVERTISING_INTERVAL 40    // * 0.625ms
#define DEFAULT_SLOW_ADVERTISING_INTERVAL 160   // * 0.625ms

#define BLE_PASSKEY_LEN 6


class BleController
{
friend class BleGapService;
friend class BleGattService;

public:

    typedef enum
    {
        BONDING_POLICY_ENABLED = 1,
        BONDING_POLICY_DISABLED = 2,
        BONDING_POLICY_ONCE = 3,
        BONDING_POLICY_SAME_MAC = 4
    } BondingPolicy;

    static BleController* spInstance;
    BleController();

    // Builder for Advertising and Scan Response packets
    static uint32_t AddChunkToBuffer(uint8_t * buffer, uint8_t code,
        const uint8_t * data, uint8_t size);
    const char * GetDeviceIdentifier();
    uint32_t GetCPUIdentifier();

    // BLE security control
    bool SetPassKey(const char * aPassKey);
    void SetBondingSettings(BondingPolicy policy, uint16_t recovery_time);
    BondingPolicy GetBondingPolicy();
    uint16_t GetBondingRecoveryTime();

protected:
    virtual void onBleInit() {};
    virtual uint32_t PrepareAdvertisingData(uint8_t * buffer, uint32_t max_len);
    virtual uint32_t PrepareScanResponseData(uint8_t * buffer, uint32_t max_len);
    virtual void OnHrsNotificationEnabled() {}
    virtual void OnHrsNotificationDisabled() {}
    virtual void OnUartData(uint8_t * p_data, uint16_t length) {}
};

void send_ble_hr_measurement(uint16_t heart_rate, const uint16_t *rr_intervals=nullptr, size_t rr_count=0);
void send_ble_nus_data(uint8_t * p_data, uint16_t length);
