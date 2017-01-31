#pragma once
#include <whiteboard/ResourceProvider.h>

namespace nea
{

/** Whiteboard provider for system-level utilities. */
class SystemProvider : private whiteboard::ResourceProvider
{
public:
    /** Constructor */
    SystemProvider();

    /** Destructor */
    virtual ~SystemProvider();

    /** Update Firmware update progress */
    void updateProgress(const uint8 progress);

    // called from the launchable module
    bool wbRegister();
    void wbUnregister();

private:
    void onGetRequest(const whiteboard::Request& request,
        const whiteboard::ParameterList& parameters) OVERRIDE;
    void onPutRequest(const whiteboard::Request& request,
        const whiteboard::ParameterList& parameters) OVERRIDE;

    bool startFirmwareUpdate(const whiteboard::ClientId clientId,
        const whiteboard::ResourceId resourceId,
        const whiteboard::ParameterList& parameters);
private:
    uint8_t m_updateProgress;
};

} // namespace nea
