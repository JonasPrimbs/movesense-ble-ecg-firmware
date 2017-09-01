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
    virtual ~SystemModeProvider();

    // called from the launchable module
    bool wbRegister();
    void wbUnregister();

private:
    void onGetRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
    void onPutRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
};

} // namespace nea
