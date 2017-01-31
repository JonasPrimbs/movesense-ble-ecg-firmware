#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/integration/port.h"
#include "whiteboard/Identifiers.h"
#include "whiteboard/WbVersion.h"
#include "whiteboard/internal/protocol/IMessageDispatcher.h"
#include "whiteboard/internal/protocol/IMessageHandler.h"
#include "whiteboard/internal/protocol/IStructureDeserializer.h"
#include "whiteboard/internal/protocol/IStructureSerializationLengthCalculator.h"
#include "whiteboard/internal/protocol/IStructureSerializer.h"
#include "whiteboard/internal/protocol/IUnknownStructureDeserializer.h"
#include "whiteboard/internal/protocol/ILegacyValueDeserializer.h"
#include "whiteboard/internal/protocol/ILegacyValueSerializer.h"

namespace whiteboard
{

/** Interface for protocol specific message dispatching and serialization */
class IProtocol
{
protected:
    /** Protected destructor. Interface is not used to delete objects. */
    inline ~IProtocol() {}

public:
    /** Gets Protocol instance for specific protocol version
    *
    * @param protocolVersion Version of the protocol which Protocol instance should be returned
    * @return Protocol instance for given protocol version or NULL if protocol version is not supported
    */
    static const IProtocol* getInstance(ProtocolVersion protocolVersion);

    /** Gets protocol instance for internal messaging
    *
    * @return Protocol instance for internal messaging
    */
    static const IProtocol& getInstanceForInternalUse();

    /** Gets the message dispatcher implementation of the protocol
     *
     * @retun Associated message dispacher instance
     */
    virtual const IMessageDispatcher& getMessageDispatcher() const = 0;

    /** Gets the message handler implementation of the protocol
    *
    * @retun Associated message handler instance
    */
    virtual const IMessageHandler& getMessageHandler() const = 0;

    /** Gets the structure serialization length calculator implementation of the protocol
    *
    * @retun Associated structure serialization length calculator instance
    */
    virtual const IStructureSerializationLengthCalculator& getStructureSerializationLengthCalculator() const = 0;

    /** Gets the structure serializer implementation of the protocol
    *
    * @retun Associated structure serializer instance
    */
    virtual const IStructureSerializer& getStructureSerializer() const = 0;

    /** Gets the structure deserializer implementation of the protocol
    *
    * @retun Associated structure deserializer instance
    */
    virtual const IStructureDeserializer& getStructureDeserializer() const = 0;

    /** Gets the unknown structure deserializer implementation of the protocol
    *
    * @retun Associated unknown structure deserializer instance
    */
    virtual const IUnknownStructureDeserializer& getUnknownStructureDeserializer() const = 0;

    /** Gets the value deserializer implementation of the protocol
    *
    * @retun Associated value deserializer instance
    */
    virtual const ILegacyValueDeserializer& getValueDeserializer() const = 0;

    /** Gets the value serializer implementation of the protocol
    *
    * @retun Associated value serializer instance
    */
    virtual const ILegacyValueSerializer& getValueSerializer() const = 0;
};

} // namespace whiteboard
