#pragma once
/******************************************************************************
    Copyright (c) Suunto Oy 2015.
    All rights reserved.
******************************************************************************/

#include "whiteboard/internal/protocol/IProtocol.h"
#include "whiteboard/internal/protocol/v9/MessageDispatcher.h"
#include "whiteboard/internal/protocol/v9/MessageHandler.h"
#include "whiteboard/internal/protocol/v9/StructureDeserializer.h"
#include "whiteboard/internal/protocol/v9/StructureSerializationLengthCalculator.h"
#include "whiteboard/internal/protocol/v9/StructureSerializer.h"
#include "whiteboard/internal/protocol/v9/UnknownStructureDeserializer.h"
#include "whiteboard/internal/protocol/v9/LegacyValueDeserializer.h"
#include "whiteboard/internal/protocol/v9/LegacyValueSerializer.h"

namespace whiteboard
{
namespace protocol_v9
{

/** Interface for protocol specific message dispatching and serialization */
class Protocol FINAL : public IProtocol
{
private:
    // Prevent use of default constructors
    Protocol();
    Protocol(const Protocol&) DELETED;
    Protocol& operator=(const Protocol&) DELETED;

public:
    // Protocol instance
    static const Protocol instance;

    /** Destructor */
    virtual ~Protocol() {}

    /** Gets version of the protocol
    *
    * @return Protocol version
    */
    static inline ProtocolVersion getVersion()
    {
        return 9;
    }

    /** Gets the message dispatcher implementation of the protocol
    *
    * @retun Associated message dispacher instance
    */
    inline const IMessageDispatcher& getMessageDispatcher() const OVERRIDE
    {
        return mMessageDispatcher;
    }

    /** Gets the message handler implementation of the protocol
    *
    * @retun Associated message handler instance
    */
    inline const IMessageHandler& getMessageHandler() const OVERRIDE
    {
        return mMessageHandler;
    }

    /** Gets the structure serialization length calculator implementation of the protocol
    *
    * @retun Associated structure serialization length calculator instance
    */
    inline const IStructureSerializationLengthCalculator& getStructureSerializationLengthCalculator() const OVERRIDE
    {
        return mStructureSerializationLengthCalculator;
    }

    /** Gets the structure serializer implementation of the protocol
    *
    * @retun Associated structure serializer instance
    */
    inline const IStructureSerializer& getStructureSerializer() const OVERRIDE
    {
        return mStructureSerializer;
    }

    /** Gets the structure deserializer implementation of the protocol
    *
    * @retun Associated structure deserializer instance
    */
    inline const IStructureDeserializer& getStructureDeserializer() const OVERRIDE
    {
        return mStructureDeserializer;
    }

    /** Gets the unknown structure deserializer implementation of the protocol
    *
    * @retun Associated unknown structure deserializer instance
    */
    inline const IUnknownStructureDeserializer& getUnknownStructureDeserializer() const OVERRIDE
    {
        return mUnknownStructureDeserializer;
    }

    /** Gets the value deserializer implementation of the protocol
    *
    * @retun Associated value deserializer instance
    */
    inline const ILegacyValueDeserializer& getValueDeserializer() const OVERRIDE
    {
        return mValueDeserializer;
    }

    /** Gets the value serializer implementation of the protocol
    *
    * @retun Associated value serializer instance
    */
    inline const ILegacyValueSerializer& getValueSerializer() const OVERRIDE
    {
        return mValueSerializer;
    }

private:
    /** Associated message dispacher instance */
    MessageDispatcher mMessageDispatcher;

    /** Associated message handler instance */
    MessageHandler mMessageHandler;

    /** Associated structure serialization length calculator instance */
    StructureSerializationLengthCalculator mStructureSerializationLengthCalculator;

    /** Associated structure serializer instance */
    StructureSerializer mStructureSerializer;

    /** Associated structure deserializer instance */
    StructureDeserializer mStructureDeserializer;

    /** Associated unknown structure deserializer instance */
    UnknownStructureDeserializer mUnknownStructureDeserializer;

    /** Associated value serializer instance */
    LegacyValueSerializer mValueSerializer;

    /** Associated value deserializer instance */
    LegacyValueDeserializer mValueDeserializer;
};

} // protocol_v9
} // namespace whiteboard
