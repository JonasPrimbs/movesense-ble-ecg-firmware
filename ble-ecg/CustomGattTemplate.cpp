#include "debug/CustomGattTemplate.h"
#include "movesense.h"

#include <meas_ecg/resources.h>

#include "comm_ble_gattsvc/resources.h"
#include "comm_ble/resources.h"

constexpr uint16_t service_UUID_16 = 0x0909;

constexpr uint16_t charA_UUID_16 = 0x0010;
constexpr uint16_t charB_UUID_16 = 0x0020;
constexpr uint16_t charC_UUID_16 = 0x0030;

constexpr uint16_t charA_init = 0;
constexpr uint16_t charB_init = 1;
constexpr uint16_t charC_init = 2;

// ECG GATT Service implementations:

const char* const CustomGattTemplate::LAUNCHABLE_NAME = "EcgGattSvc";

CustomGattTemplate::CustomGattTemplate() :
    ResourceClient(WBDEBUG_NAME(__FUNCTION__), WB_EXEC_CTX_APPLICATION),
    LaunchableModule(LAUNCHABLE_NAME, WB_EXEC_CTX_APPLICATION)
{
    // Constructor
}

bool CustomGattTemplate::initModule()
{
    this->mModuleState = WB_RES::ModuleStateValues::INITIALIZED;
    return true;
}

void CustomGattTemplate::deinitModule()
{
    this->mModuleState = WB_RES::ModuleStateValues::UNINITIALIZED;
}

bool CustomGattTemplate::startModule()
{
    this->mModuleState = WB_RES::ModuleStateValues::STARTED;

    // Configure ECG GATT Service.
    this->configGattSvc();
    this->asyncSubscribe(WB_RES::LOCAL::COMM_BLE_PEERS());

    return true;
}

void CustomGattTemplate::stopModule()
{
    // set resources to invalid
    this->charAResource = wb::ID_INVALID_RESOURCE;
    this->charBResource = wb::ID_INVALID_RESOURCE;
    this->charCResource = wb::ID_INVALID_RESOURCE;

    this->asyncUnsubscribe(charAResource);
    this->asyncUnsubscribe(charBResource);
    this->asyncUnsubscribe(charCResource);

    this->mModuleState = WB_RES::ModuleStateValues::STOPPED;
}

void CustomGattTemplate::onGetResult(wb::RequestId requestId,
                                   wb::ResourceId resourceId,
                                   wb::Result resultCode,
                                   const wb::Value& rResultData)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE::LID:
        {
            const WB_RES::GattSvc &svc = rResultData.convertTo<const WB_RES::GattSvc&>();
            for (size_t i = 0; i < svc.chars.size(); i++)
            {
                const WB_RES::GattChar &c = svc.chars[i];
                uint16_t uuid16 = *reinterpret_cast<const uint16_t*>(&(c.uuid[0]));

                switch (uuid16)
                {
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

            // Force subscriptions asynchronously to save stack (will have stack overflow if not)
            this->asyncSubscribe(this->charAResource, AsyncRequestOptions(NULL, 0, true));
            this->asyncSubscribe(this->charBResource, AsyncRequestOptions(NULL, 0, true));
            this->asyncSubscribe(this->charCResource,  AsyncRequestOptions(NULL, 0, true));
            break;
            // GATT setup finished from here on...
        }
    }
}

void CustomGattTemplate::onPostResult(wb::RequestId requestId,
                                    wb::ResourceId resourceId,
                                    wb::Result resultCode,
                                    const wb::Value& rResultData)
{
    if (resultCode == wb::HTTP_CODE_CREATED)
    {
        // Gatt service was created.
        this->svcHandle = (int32_t)rResultData.convertTo<uint16_t>();

        // Request more info about created GATT service so we get the characteristics handles.
        this->asyncGet(
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE(),
            AsyncRequestOptions::Empty,
            this->svcHandle
        );
    }
}

void CustomGattTemplate::onNotify(wb::ResourceId resourceId,
                                const wb::Value& value,
                                const wb::ParameterList& rParameters)
{
    switch(resourceId.localResourceId)
    {
        case WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::LID:
        {
            // Get the GATT characteristic which is handled now.
            WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE::SUBSCRIBE::ParameterListRef parameterRef(rParameters);
            auto charHandle = parameterRef.getCharHandle();

            // Set the current GATT characteristic.
            if (charHandle == this->charBHandle)
            {
            }
            else if (charHandle == this->charCHandle)
            {
            }
            break;
        }
        case WB_RES::LOCAL::COMM_BLE_PEERS::LID: {
            auto &peerChange = value.convertTo<WB_RES::PeerChange>();

            uint32_t handle = peerChange.peer.handle.hasValue() ? peerChange.peer.handle.getValue() : 0;
            if (!handle) {
                setCharCValue(199);
            } else {
                setCharCValue(handle);
                this->connectionHandle = handle;
            }

            break;
        }
    }
}

void CustomGattTemplate::configGattSvc()
{
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
    charA.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&charA_UUID_16), sizeof(uint16_t));

    // Characteristic B
    charB.props = wb::MakeArray<WB_RES::GattProperty>(charBProps, 2);
    charB.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&charB_UUID_16), sizeof(uint16_t));
    charB.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&charB_init), sizeof(uint16_t));

    // Characteristic C
    charC.props = wb::MakeArray<WB_RES::GattProperty>(charCProps, 2);
    charC.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&charC_UUID_16), sizeof(uint16_t));
    charC.initial_value = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&charC_init), sizeof(uint16_t));

    // Service
    GattSvc.uuid = wb::MakeArray<uint8_t>(reinterpret_cast<const uint8_t*>(&service_UUID_16), sizeof(uint16_t));
    GattSvc.chars = wb::MakeArray<WB_RES::GattChar>(characteristics, 3);

    // Create custom GATT Service.
    this->asyncPost(WB_RES::LOCAL::COMM_BLE_GATTSVC(), AsyncRequestOptions::Empty, GattSvc);
}

void CustomGattTemplate::setCharCValue(uint32_t value) {

    WB_RES::Characteristic characteristic;
    characteristic.bytes = wb::MakeArray<uint8_t>(reinterpret_cast<uint8_t *>(&value), sizeof(uint32_t));

    // Set Characteristic Value
    this->asyncPut(
        WB_RES::LOCAL::COMM_BLE_GATTSVC_SVCHANDLE_CHARHANDLE(),
        AsyncRequestOptions::Empty,
        this->svcHandle,
        this->charCHandle,
        characteristic
    );
}
