#pragma once
#include <whiteboard/ResourceProvider.h>
#include <whiteboard/ResourceClient.h>

namespace nea
{

/** Whiteboard provider for system mode change functions */
class SystemModeProvider FINAL : private whiteboard::ResourceProvider, private whiteboard::ResourceClient
{
public:
    SystemModeProvider();
    virtual ~SystemModeProvider() OVERRIDE;

    // called from the launchable module
    bool wbRegister();
    void wbUnregister();

protected:
    virtual void onGetRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
    virtual void onPutRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;

    virtual void onTimer(whiteboard::TimerId timerId) OVERRIDE;

private:
    uint8_t nextSysMode;
};

} // namespace nea
