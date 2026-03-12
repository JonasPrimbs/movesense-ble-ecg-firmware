#include "RateTestClient.h"
#include "movesense.h"

#include <meas_ecg/resources.h>
#include <meas_imu/resources.h>
#include <ui_ind/resources.h>

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"

// Gatt service and characteristics definitions:
constexpr uint16_t service_UUID_16 = 0x1909;

constexpr uint16_t charA_UUID_16 = 0x0010;
constexpr uint16_t charB_UUID_16 = 0x0020;
constexpr uint16_t charC_UUID_16 = 0x0030;

constexpr uint16_t charA_init = 0;
constexpr uint16_t charB_init = 0;
constexpr uint16_t charC_init = 0;


// GATT Service implementations:
const char *const RateTestClient::LAUNCHABLE_NAME = "RateTestClient";

RateTestClient::RateTestClient() : ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
                                   LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION) {}

bool RateTestClient::initModule() {
    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void RateTestClient::deinitModule() {
    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool RateTestClient::startModule() {
    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Configure ECG GATT Service.
    this->configGattSvc();
    this->asyncSubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    return true;
}

void RateTestClient::stopModule() {
    // set resources to invalid
    this->charAResource = wb::ID_INVALID_RESOURCE;
    this->charBResource = wb::ID_INVALID_RESOURCE;
    this->charCResource = wb::ID_INVALID_RESOURCE;

    this->asyncUnsubscribe(charAResource);
    this->asyncUnsubscribe(charBResource);
    this->asyncUnsubscribe(charCResource);

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void RateTestClient::onGetResult(wb::RequestId requestId,
                                 wb::ResourceId resourceId,
                                 wb::Result resultCode,
                                 const wb::Value &rResultData) {
    switch (resourceId.localResourceId) {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID: {
            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc &>();
            for (size_t i = 0; i < svc.chars.size(); i++) {
                const WB_RES::GattChar &c = svc.chars[i];
                uint16_t uuid16 = *reinterpret_cast<const uint16_t *>(&(c.uuid[0]));

                switch (uuid16) {
                    case charA_UUID_16:
                        this->charAHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case charB_UUID_16:
                        this->charBHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                    case charC_UUID_16:
                        this->charCHandle = c.handle.hasValue() ? c.handle.getValue() : 0;
                        break;
                }
            }
            if (!charAHandle || !charBHandle || !charCHandle) return;

            char pathBuffer[32];

            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", gattSvcHandle, charAHandle);
            getResource(pathBuffer, charAResource);

            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", gattSvcHandle, charBHandle);
            getResource(pathBuffer, charBResource);

            snprintf(pathBuffer, sizeof(pathBuffer), "/Comm/Ble/GattSvc/%d/%d", gattSvcHandle, charCHandle);
            getResource(pathBuffer, charCResource);

            if (charAResource != wb::ID_INVALID_RESOURCE)
                this->asyncSubscribe(this->charAResource, AsyncRequestOptions::ForceAsync);
            if (charBResource != wb::ID_INVALID_RESOURCE)
                this->asyncSubscribe(this->charBResource, AsyncRequestOptions::ForceAsync);
            if (charCResource != wb::ID_INVALID_RESOURCE)
                this->asyncSubscribe(this->charCResource, AsyncRequestOptions::ForceAsync);
            break;
            // GATT setup finished from here on...
        }
        case WB_RES::LOCAL::COMM_BLE_PEERS::LID: {
            auto &peerList = rResultData.convertTo<WB_RES::PeerList &>();
            WB_RES::PeerEntry firstPeer = peerList.connectedPeers[0];
            const uint8_t length = static_cast<uint8_t>(peerList.connectedPeers.size());
            const int32_t peerHandle = firstPeer.handle.hasValue() ? firstPeer.handle.getValue() : -1;

            if (peerHandle < 0) {
                const uint32_t value = length + (198 << 8);
            } else {
                this->connectionHandle = peerHandle;
            }
            break;
        }
        // Update in Peer Parameters
        case WB_RES::LOCAL::COMM_BLE_PEERS_CONNHANDLE_PARAMS::LID: {
            auto &connParams = rResultData.convertTo<WB_RES::ConnParams &>();
            savedConnParams = connParams;
            uint16_t arr[4];
            arr[0] = connParams.sup_timeout;
            arr[1] = connParams.max_conn_interval;
            arr[2] = connParams.min_conn_interval;
            arr[3] = connParams.slave_latency;

            // Write update parameters to characteristic C:
            this->setCharValue(gattSvcHandle, charCHandle, reinterpret_cast<uint8_t *>(arr), 4 * sizeof(uint16_t));
            break;
        }
    }
}

void RateTestClient::onPostResult(wb::RequestId requestId,
                                  wb::ResourceId resourceId,
                                  wb::Result resultCode,
                                  const wb::Value &rResultData) {
    if (resultCode == wb::HTTP_CODE_CREATED) {
        // Gatt service was created.
        this->gattSvcHandle = (int32_t) rResultData.convertTo<uint16_t>();

        // Request more info about created GATT service so we get the characteristics handles.
        this->asyncGet(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(),
            AsyncRequestOptions::Empty,
            this->gattSvcHandle
        );
    }
}

void RateTestClient::onNotify(wb::ResourceId resourceId,
                              const wb::Value &value,
                              const wb::ParameterList &rParameters) {
    switch (resourceId.localResourceId) {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID: {
            // Get the GATT characteristic which is handled now.
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
            auto charHandle = parameterRef.getCharHandle();
            auto &characteristic = value.convertTo<WB_RES::Characteristic &>();

            const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
            this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type);

            if (charHandle == this->charAHandle) {
                if (!characteristic.notifications.hasValue()) break;

                bool notificationsEnabled = characteristic.notifications.getValue();

                if (notificationsEnabled && !charAIsSubscribed) {
                    this->startDataStreaming();
                    // TODO: update on disconnects to false
                    charAIsSubscribed = true;
                }
                if (!notificationsEnabled && charAIsSubscribed) {
                    this->stopDataStreaming();
                    charAIsSubscribed = false;
                }
            }
            // Set the current GATT characteristic.
            if (charHandle == this->charBHandle) {
                // trigger action:
                processCommand(characteristic);
            } else if (charHandle == this->charCHandle) {
            }
            break;
        }
        case WB_RES::LOCAL::COMM_BLE_PEERS::LID: {
            auto &peerChange = value.convertTo<WB_RES::PeerChange &>();

            switch (peerChange.state) {
                case WB_RES::PeerStateValues::CONNECTED:
                    startBlinker(2);
                    break;
                case WB_RES::PeerStateValues::DISCONNECTED:
                    charAIsSubscribed = false;
                    startBlinker(5);
                    break;
            }

            // get handle of connection
            const int32_t handle = peerChange.peer.handle.hasValue() ? peerChange.peer.handle.getValue() : -1;
            this->connectionHandle = handle;
            // get connection params -> will write current config to char C
            this->asyncGet(WB_RES::LOCAL::COMM_BLE_PEERS_CONNHANDLE_PARAMS(), AsyncRequestOptions::Empty, handle);
            break;
        }
        case WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE::LID: {
            auto &ecgData = value.convertTo<WB_RES::ECGData &>();
            uint8_t buffr[155];
            size_t numberOfBytes = ecgData.samples.getNumberOfItems() * sizeof(int32_t);
            memcpy(buffr, ecgData.samples.begin(), numberOfBytes);
            setCharValue(this->gattSvcHandle, this->charAHandle, buffr, numberOfBytes);
            break;
        }
    }
}

void RateTestClient::onPutResult(whiteboard::RequestId requestId, whiteboard::ResourceId resourceId,
                                 whiteboard::Result resultCode, const whiteboard::Value &rResultData) {
    switch (resourceId.localResourceId) {
        case WB_RES::LOCAL::COMM_BLE_PEERS_CONNHANDLE_PARAMS::LID: {
            asyncGet(WB_RES::LOCAL::COMM_BLE_PEERS_CONNHANDLE_PARAMS(), AsyncRequestOptions::ForceAsync,
                     this->connectionHandle);
            break;
        }
    }
}

void RateTestClient::onTimer(wb::TimerId timerId) {
    if (timerId == this->blinkTimer) {
        if (this->blinkCounter == 0) {
            this->stopTimer(this->blinkTimer);
            this->blinkTimer = wb::ID_INVALID_TIMER;
            return;
        }
        const WB_RES::VisualIndType type = WB_RES::VisualIndTypeValues::SHORT_VISUAL_INDICATION;
        this->asyncPut(WB_RES::LOCAL::UI_IND_VISUAL(), AsyncRequestOptions::Empty, type);
        this->blinkCounter -= 1;
    } else if (timerId == this->rateTestTimer) {
        // send <streamingRate> bytes of pseudo data via Characteristic A
        if (switchData) {
            WB_RES::Characteristic characteristic;
            setCharValue(gattSvcHandle, this->charAHandle, RateTestClient::dataPacket + 1, streamingRate);
            switchData = false;
        } else {
            WB_RES::Characteristic characteristic;
            setCharValue(gattSvcHandle, this->charAHandle, RateTestClient::dataPacket, streamingRate);
            switchData = true;
        }
    }
}

void RateTestClient::configGattSvc() {
    // Define ECG GATT Service and its Characteristics.
    WB_RES::GattSvc GattSvc;
    WB_RES::GattChar characteristics[3];
    WB_RES::GattChar &charA = characteristics[0];
    WB_RES::GattChar &charB = characteristics[1];
    WB_RES::GattChar &charC = characteristics[2];

    // Specify Characteristics's properties.
    WB_RES::GattProperty charAProp = WB_RES::GattProperty::NOTIFY;
    WB_RES::GattProperty charBProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };
    WB_RES::GattProperty charCProps[2] = {
        WB_RES::GattProperty::READ,
        WB_RES::GattProperty::WRITE
    };

    // Characteristic A
    charA.props = wb::MakeArray<WB_RES::GattProperty>(&charAProp, 1);
    charA.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t *>(&charA_UUID_16), sizeof(uint16_t));

    // Characteristic B
    charB.props = wb::MakeArray<WB_RES::GattProperty>(charBProps, 2);
    charB.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t *>(&charB_UUID_16), sizeof(uint16_t));
    charB.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t *>(&charB_init), sizeof(uint16_t));

    // Characteristic C
    charC.props = wb::MakeArray<WB_RES::GattProperty>(charCProps, 2);
    charC.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t *>(&charC_UUID_16), sizeof(uint16_t));
    charC.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t *>(&charC_init), sizeof(uint16_t));

    // Service
    GattSvc.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t *>(&service_UUID_16), sizeof(uint16_t));
    GattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 3);

    // Create custom GATT Service.
    this->asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, GattSvc);
}

void RateTestClient::setCharValue(int32_t svcHandle, int32_t charHandle, uint32_t value) {
    WB_RES::Characteristic characteristic;
    characteristic.bytes = wb::MakeArray<uint8_t>(reinterpret_cast<uint8_t *>(&value), sizeof(uint32_t));

    // Set Characteristic Value
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        svcHandle,
        charHandle,
        characteristic
    );
}

void RateTestClient::setCharValue(int32_t svcHandle, int32_t charHandle, uint8_t *array, uint32_t len) {
    WB_RES::Characteristic characteristic;
    characteristic.bytes = wb::MakeArray<uint8_t>(array, len);

    // Set Characteristic Value
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        svcHandle,
        charHandle,
        characteristic
    );
}

void RateTestClient::startBlinker(uint32_t n) {
    constexpr size_t BLINK_PERIOD = 250;
    this->blinkCounter = n;
    // do not allow a new blinker while the old is not finished.
    if (this->blinkTimer != wb::ID_INVALID_TIMER) return;
    this->blinkTimer = this->startTimer(BLINK_PERIOD, true);
}

void RateTestClient::processCommand(WB_RES::Characteristic characteristic) {
    uint8_t cmd = characteristic.bytes[0];
    switch (cmd) {
        case 0:
        // do nothing
        case 1:
            // get request -> will write current config to char C
            asyncGet(WB_RES::LOCAL::COMM_BLE_PEERS_CONNHANDLE_PARAMS(), AsyncRequestOptions::Empty,
                     this->connectionHandle);
            break;
        case 2: {
            // set new supervision timeout from parameter
            uint8_t parameter = characteristic.bytes[1];
            this->savedConnParams.sup_timeout = static_cast<uint16_t>(parameter);
            asyncPut(WB_RES::LOCAL::COMM_BLE_PEERS_CONNHANDLE_PARAMS(), AsyncRequestOptions::Empty,
                     this->connectionHandle, this->savedConnParams);
            break;
        }
        case 3: {
            // set new streaming rate parameter
            uint8_t parameter = characteristic.bytes[1];
            // cap streaming rate at MTU size
            if (parameter > 155) parameter = 155;
            this->streamingRate = parameter;
            break;
        }
        case 4: {
            uint8_t parameter = characteristic.bytes[1];
            this->streaming_mode = parameter;
            break;
        }
        case 5: {
            uint8_t parameter = characteristic.bytes[1];
            this->measMultiplicator = parameter;
            break;
        }
    }
}
void RateTestClient::startDataStreaming() {
    switch (this->streaming_mode) {
        case 0:
            startPseudoData();
            break;
        case 1:
            startEcgData(this->measMultiplicator);
            break;
    }
}
void RateTestClient::stopDataStreaming() {
    switch (this->streaming_mode) {
        case 0:
            stopPseudoData();
            break;
        case 1:
            stopEcgData(this->measMultiplicator);
            break;
    }
}

void RateTestClient::startPseudoData() {
    if (rateTestTimer != wb::ID_INVALID_TIMER) return;

    size_t sendPeriod = 10;
    rateTestTimer = startTimer(sendPeriod, true);
}

void RateTestClient::stopPseudoData() {
    stopTimer(rateTestTimer);
    rateTestTimer = wb::ID_INVALID_TIMER;
}

void RateTestClient::startEcgData(uint8_t multiplicator) {
    asyncSubscribe(WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(), AsyncRequestOptions::Empty, 125 * multiplicator);
}

void RateTestClient::stopEcgData(uint8_t multiplicator) {
    asyncUnsubscribe(WB_RES::LOCAL::MEAS_ECG_REQUIREDSAMPLERATE(), AsyncRequestOptions::Empty, 125 * multiplicator);
}


