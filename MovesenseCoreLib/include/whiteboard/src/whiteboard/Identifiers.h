// Copyright (c) Suunto Oy 2015-2016. All rights reserved.
#pragma once

#include "whiteboard/integration/port.h"
#include "whiteboard/WhiteboardConfig.h"
#include "whiteboard/metadata/MetadataStructures.h"

namespace whiteboard
{

WB_PACK_BEGIN()

/** Suunto whiteboard serial number */
class WB_PACKED WB_API SuuntoSerial
{
public:
    /** Serial numbers are packed, but most users want it in readable form */
    typedef char UnpackedSerialNumber[WB_MAX_SERIAL_NUMBER_LENGTH + 1];

    /** Default constructor */
    SuuntoSerial();

    /** Constructor that initializes serial number from serial number string
     *
     * @param serialNumber Null terminated serial number string
     */
    SuuntoSerial(const char* pSerialNumber);

    /** Method for getting reference to whiteboard's own serial number.
     *
     * @return const reference to whiteboard's own serial number.
     */
    static SuuntoSerial const & ownSerial();

    /** Assignment operator that initializes serial number value from given serial number string
    *
    * @param serialNumber Null terminated serial number string
    */
    const SuuntoSerial& operator=(const char* pSerialNumber);

    /** Checks whether serial number is valid
    *
    * @return A value indicating whether the serial number is valid
    */
    bool isValid() const;

    /** Gets the serial number as string
    * @param serialStr [out] buffer for serial number string, length WB_MAX_SERIAL_NUMBER_LENGTH + 1
    * @return true if the serial number is valid
    */
    void getSerial(UnpackedSerialNumber serialStr) const;

    /** Returns pointer to serial number data.
    *
    * @note Returned value is not a null terminated string
    * @return Pointer to packed serial number data
    */
    void* getData();

    /** Returns pointer to serial number data.
    *
    * @note Returned value is not a null terminated string
    * @return Pointer to packed serial number data
    */
    const void* getData() const;

    /** Serial number comparison operator
    *
    * @param rOther Other serial number to compare with
    * @return A value indicating whether this serial number equals to the specified serial number
    */
    bool operator==(const SuuntoSerial& rOther) const;

    /** Serial number comparison operator
    *
    * @param rOther Other serial number to compare with
    * @return A value indicating whether this serial number is inequal with the specified serial number
    */
    bool operator!=(const SuuntoSerial& rOther) const;

    /** Serial number comparison operator
    *
    * @param other Other serial number to compare with
    * @return A value indicating whether this serial number equals to the specified serial number
    */
    bool operator==(const char* pSerialNumber) const;

    /** Serial number comparison operator
    *
    * @param other Other serial number to compare with
    * @return A value indicating whether this serial number is inequal with the specified serial number
    */
    bool operator!=(const char* pSerialNumber) const;

private:
    /** Invalidate serial number data */
    void invalidate(uint8* pPackedData) const;

    /** Reset serial number */
    void reset(const char* pSerialNumber);

    /** Pack a validated serial number */
    void pack(const char* pSerialNumber, uint8* pPackedData) const;

    /** Unpack a validated serial number data */
    void unpack(const uint8* pPackedData, char* pSerialNumber) const;

    /** Validate a serial number */
    bool isValid(const char* pSerialNumber) const;

private:
    /** Packed serial number data */
    uint8 mpData[WB_PACKED_SERIAL_NUMBER_SIZE];

}; // SuuntoSerial

WB_PACK_END()

WB_STATIC_VERIFY(sizeof(SuuntoSerial) == WB_PACKED_SERIAL_NUMBER_SIZE, SizeOfSuuntoSerialIsNotCorrect);

/** Type that is used to identify whiteboard requests of single whiteboard. */
typedef uint16 RequestId;

/** ID of invalid request */
static const RequestId ID_INVALID_REQUEST = 0;

/** Type that is used to identify whiteboard instances in whiteboard network.
 * Unique for local application / whiteboard. Shorthand for using actual whiteboard serial number. */
typedef uint8 WhiteboardId;

/** ID of local Whiteboard */
static const WhiteboardId LOCAL_WHITEBOARD_ID = 0;

/** ID of used to mark invalid Whiteboard */
static const WhiteboardId INVALID_WHITEBOARD_ID = 255;

/** Type that is used to identify resource instances. Unique for one requesting client, that access multiple instances of the same
 * resource */
typedef uint8 ResourceInstanceId;

/** Maximum number of cached resource instances per client. Note packed structures below. */
#define WB_MAX_RESOURCE_INSTANCES 15

/** Resource Instance ID that is used to uniquely identify path parameter for a client. */
static const ResourceInstanceId ID_INVALID_RESOURCE_INSTANCE = WB_MAX_RESOURCE_INSTANCES;

/** Type that is used to identify resources in whiteboard network.
 *
 * @note Don't change this structure without also making changes to wbres tool
 */
struct WB_API ResourceId
{
    /** Value of resource ID packed to a single unsigned 32-bit integer. */
    typedef uint32 Value;

    union
    {
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used: nameless struct/union
#endif
        struct
        {
            /** Used by the path parameters implementation */
            ResourceInstanceId instanceId : 4;

            /** ID of the execution context */
            ExecutionContextId executionContextId : 4;

            /** ID of the whiteboard instance */
            WhiteboardId whiteboardId;

            /** Resource ID of the resource in that whiteboard instance */
            LocalResourceId localResourceId;
        };
#if _MSC_VER
#pragma warning(pop)
#endif

        /** 32-bit storage value for the above */
        Value value;
    };

    /** Default constructor
    */
    inline ResourceId() {}

    /** Initializes a new instance of ResourceId class
    *
    * @param executionContextId ID of the execution context
    * @param whiteboardId ID of the whiteboard instance
    * @param localResourceId Resource ID of the resource in that whiteboard instance
    */
    inline ResourceId(const ExecutionContextId _executionContextId,
        const WhiteboardId _whiteboardId,
        const LocalResourceId _localResourceId,
        const ResourceInstanceId _resourceInstanceId = ID_INVALID_RESOURCE_INSTANCE)
        : instanceId(_resourceInstanceId),
        executionContextId(_executionContextId),
        whiteboardId(_whiteboardId),
        localResourceId(_localResourceId)
    {
    }

    /** Constructor that initializes structure from 32-bit unsigned integer
    *
    * @param data Structure data as 32-bit unsigned integer
    */
    inline ResourceId(const ResourceId::Value data)
    {
        value = data;
    }

    /** Explicit conversion operator to uint32
    *
    * @return Data structure converted to machine endian 32-bit unsigned integer
    */
    inline EXPLICIT operator ResourceId::Value() const { return value; }

    /** Explicit comparison operator to uint32
    *
    * @return A value indicating whether this resource ID equals to specified id value
    */
    inline bool operator==(const ResourceId::Value other) const
    {
        return other == value;
    }

    /** Explicit comparison operator to uint32
    *
    * @return A value indicating whether this resource ID is inequal with specified id value
    */
    inline bool operator!=(const ResourceId::Value other) const { return !(*this == other); }

    /** @return true if this resourceId contains path parameter allocation reference */
    inline bool isPathParameterRef() const { return instanceId != ID_INVALID_RESOURCE_INSTANCE; }

    /** DEPRECATED: This function will be removed in future Whiteboard versions. Use localResourceId for comparisons instead,
    * because it produces more optimal code. For more information see Whiteboard best practices.
    *
    * @return the raw id without the instanceId; can be used in the provider-end to compare the
    *  ID to generated ID without path parameter allocation reference.
    *
    *  For example:
    *
    *  if (resId.getConstId() == WB_RES::LOCAL::MAPS_MAPID::ID)
    *  ...
    */
    inline Value getConstId() const
    {
        return value | ID_INVALID_RESOURCE_INSTANCE;
    }
};

WB_STATIC_VERIFY(sizeof(ResourceId) == sizeof(uint32), SizeOfResourceIdNotFourBytes);

/** Local client ID type wrapper into local entity ID */
typedef LocalEntityId LocalClientId;

// Forward declare internal clientId flags
struct ClientIdFlags;

/** Local provider ID type wrapper into local entity ID */
typedef LocalEntityId LocalProviderId;

/** Local client ID that is used to indicate invalid client. */
static const LocalClientId ID_INVALID_LOCAL_CLIENT = 0xffff;

/** Local provider ID that is used to indicate invalid provider. */
static const LocalProviderId ID_INVALID_LOCAL_PROVIDER = 0xffff;

/** Local client and provider IDs are from the same ID value range separated by one indicator bit */
static const LocalEntityId ID_LOCAL_CLIENT_MASK_BIT = 0x8000;

/** Type that is used to identify clients in whiteboard network. */
struct WB_API ClientId
{
    /** Value of client ID packed to a single unsigned 32-bit integer. */
    typedef uint32 Value;

    union
    {
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used: nameless struct/union
#endif
        struct
        {
            union
            {
                struct
                {
                    /** For internal use */
                    uint8 flags : 4;

                    /** ID of the execution context */
                    ExecutionContextId executionContextId : 4;
                };

                /** Combined execution context ID and flags value */
                uint8 executionContextIdAndFlags;
            };

            /** ID of the whiteboard instance */
            WhiteboardId whiteboardId;

            /** Client ID of the resource in that whiteboard instance */
            LocalEntityId localClientId;
        };
#if _MSC_VER
#pragma warning(pop)
#endif

        /** 32-bit storage value for the above */
        Value value;
    };

    /** Default constructor
    */
    inline ClientId() {}

    /** Initializes a new instance of ClientId class
    *
    * @param executionContextId ID of the execution context
    * @param whiteboardId ID of the whiteboard instance
    * @param localClientId Client ID of the client in that whiteboard instance
    */
    inline ClientId(const ExecutionContextId _executionContextId,
                    const WhiteboardId _whiteboardId,
                    const LocalClientId _localClientId)
        : flags(0),
          executionContextId(_executionContextId),
          whiteboardId(_whiteboardId),
          localClientId(_localClientId)
    {
    }

    /** Constructor that initializes structure from 32-bit unsigned integer
    *
    * @param data Structure data as 32-bit unsigned integer
    */
    inline ClientId(const Value data)
        : value(data)
    {
    }

    /** Explicit conversion operator to uint32
    *
    * @return Data structure converted to machine endian 32-bit unsigned integer
    */
    inline EXPLICIT operator Value() const { return value; }

    /** Explicit comparison operator to uint32
    *
    * @return A value indicating whether this client ID equals to specified id value
    */
    inline bool operator==(const ClientId::Value other) const
    {
        return other == value;
    }

    /** Explicit comparison operator to uint32
    *
    * @return A value indicating whether this client ID is inequal with specified id value
    */
    inline bool operator!=(const ClientId::Value other) const { return !(*this == other); }

    /// @return reference to internal flags of the ID - for internal use only.
    const ClientIdFlags& getFlags() const;

    /// @return reference (modifiable) to internal flags of the ID - for internal use only.
    ClientIdFlags& getFlags();

    /// @return true if client id contains nonCriticalSubscription-flag set
    bool isNonCriticalSubscription() const;
};

WB_STATIC_VERIFY(sizeof(ClientId) == sizeof(uint32), SizeOfClientIdNotFourBytes);

// Forward declare internal ProviderId flags
struct ProviderIdFlags;

/** Type that is used to identify providers in whiteboard network. */
struct WB_API ProviderId
{
    /** Value of provider ID packed to a single unsigned 32-bit integer. */
    typedef uint32 Value;

    union
    {
#if _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201) // warning C4201: nonstandard extension used: nameless struct/union
#endif
        struct
        {
            union
            {
                struct
                {
                    /* Flags for internal use */
                    uint8 flags : 4;

                    /** ID of the execution context */
                    ExecutionContextId executionContextId : 4;
                };

                /** Combined execution context ID and flags value */
                uint8 executionContextIdAndFlags;
            };

            /** Reserved for future use */
            uint8 reserved2;

            /** Provider ID of the resource in that whiteboard instance */
            LocalEntityId localProviderId;
        };
#if _MSC_VER
#pragma warning(pop)
#endif

        /** 32-bit storage value for the above */
        Value value;
    };

    /** Default constructor
    */
    inline ProviderId() {}

    /** Initializes a new instance of ProviderId class
    *
    * @param executionContextId ID of the execution context
    * @param localProviderId Provider ID of the provider in that whiteboard instance
    */
    inline ProviderId(const ExecutionContextId _executionContextId,
                      const LocalProviderId _localProviderId)
        : flags(0),
          executionContextId(_executionContextId),
          reserved2(0),
          localProviderId(_localProviderId)
    {
    }

    /** Constructor that initializes structure from 32-bit unsigned integer
    *
    * @param data Structure data as 32-bit unsigned integer
    */
    inline ProviderId(const Value data)
        : value(data)
    {
    }

    /** Explicit conversion operator to uint32
    *
    * @return Data structure converted to machine endian 32-bit unsigned integer
    */
    inline EXPLICIT operator Value() const { return value; }

    /** Explicit comparison operator to uint32
    *
    * @return A value indicating whether this client ID equals to specified id value
    */
    inline bool operator==(const ProviderId::Value other) const
    {
        return other == value;
    }

    /** Explicit comparison operator to uint32
    *
    * @return A value indicating whether this client ID is inequal with specified id value
    */
    inline bool operator!=(const ProviderId::Value other) const { return !(*this == other); }

    /// @return reference to internal flags of the ID - for internal use only.
    const ProviderIdFlags& getFlags() const;

    /// @return reference (modifiable) to internal flags of the ID - for internal use only.
    ProviderIdFlags& getFlags();
};

WB_STATIC_VERIFY(sizeof(ProviderId) == sizeof(uint32), SizeOfProviderIdNotFourBytes);

/** Local resource ID that is used to indicate invalid resource. */
static const LocalResourceId ID_INVALID_LOCAL_RESOURCE = 0xffff;

/** Local resource ID that is used to indicate root resource. */
static const LocalResourceId ID_LOCAL_ROOT_RESOURCE = 0;

/** Resource ID that is used to indicate root resource. */
WB_API extern const ResourceId::Value ID_ROOT_RESOURCE;

/** Resource ID that is used to indicate invalid resource. */
WB_API extern const ResourceId::Value ID_INVALID_RESOURCE;

/** Local entity ID that is used to indicate invalid entity. */
static const LocalEntityId ID_INVALID_LOCAL_ENTITY = 0xffff;

/** Client ID that is used to indicate invalid client. */
WB_API extern const ClientId::Value ID_INVALID_CLIENT;

/** Provider ID that is used to indicate invalid provider. */
WB_API extern const ProviderId::Value ID_INVALID_PROVIDER;

/** Local subscription ID that is used to indicate invalid subscription. */
static const LocalSubscriptionId ID_INVALID_LOCAL_SUBSCRIPTION = 0xffff;

/** Checks two resource IDs for equality
 *
 * @param firstId First ID to check
 * @param secondId Second ID to check
 * @return A value indicating whether the two IDs are equal.
 */
inline bool operator==(const ResourceId& firstId, const ResourceId& secondId)
{
    return static_cast<uint32>(firstId) == static_cast<uint32>(secondId);
}

/** Checks two resource IDs for inequality
*
* @param firstId First ID to check
* @param secondId Second ID to check
* @return A value indicating whether the two IDs are inequal.
*/
inline bool operator!=(const ResourceId& firstId, const ResourceId& secondId)
{
    return !(firstId == secondId);
}

/** Checks two resource client IDs for equality
*
* @param firstId First ID to check
* @param secondId Second ID to check
* @return A value indicating whether the two IDs are equal.
*/
inline bool operator==(const ClientId& firstId, const ClientId& secondId)
{
    return static_cast<uint32>(firstId) == static_cast<uint32>(secondId);
}

/** Checks two resource client IDs for inequality
*
* @param firstId First ID to check
* @param secondId Second ID to check
* @return A value indicating whether the two IDs are inequal.
*/
inline bool operator!=(const ClientId& firstId, const ClientId& secondId)
{
    return !(firstId == secondId);
}

/** Checks two resource provider IDs for equality
*
* @param firstId First ID to check
* @param secondId Second ID to check
* @return A value indicating whether the two IDs are equal.
*/
inline bool operator==(const ProviderId& firstId, const ProviderId& secondId)
{
    return static_cast<uint32>(firstId) == static_cast<uint32>(secondId);
}

/** Checks two resource provider IDs for inequality
*
* @param firstId First ID to check
* @param secondId Second ID to check
* @return A value indicating whether the two IDs are inequal.
*/
inline bool operator!=(const ProviderId& firstId, const ProviderId& secondId)
{
    return !(firstId == secondId);
}

/** Type that is used to identify timers */
typedef uint16 TimerId;

/** TimerId that is used to indicate invalid timerId */
static const TimerId ID_INVALID_TIMER = 0xffff;

/** Type that is used to identify timers */
typedef uint16 TimedDpcId;

/** TimedDpcId that is used to indicate invalid timedDpcId */
static const TimedDpcId ID_INVALID_TIMED_DPC = 0xffff;

/** Protocol version type */
typedef uint8 ProtocolVersion;

/** Handle type for streams allocated from StreamPool */
typedef uint8 StreamHandle;

/** Invalid stream ID */
static const StreamHandle ID_INVALID_STREAM = static_cast<StreamHandle>(-1);

} // namespace whiteboard
