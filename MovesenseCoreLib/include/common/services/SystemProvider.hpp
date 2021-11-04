#pragma once
#include <whiteboard/ResourceProvider.h>

namespace nea
{

/** Whiteboard provider for system-level utilities. */
class SystemProvider : private whiteboard::ResourceProvider
{
public:
    static const int8_t HEAP_CHECK_NOT_DONE_YET = -1;
    static SystemProvider* spInstance;
    /** Constructor */
    SystemProvider();

    /** Destructor */
    virtual ~SystemProvider();

    /** Update Firmware update progress */
    void updateProgress(const uint8 progress);

    /** Check current heap memory against minimum threshold value.
     *
     * Return SystemProvider::HEAP_CHECK_NOT_DONE_YET if SystemProvider has not
     * been initialized yet and the check cannot be run. Otherwise return 1 if
     * there was sufficient space or 0 if there was too little.
     *
     * Result is based on heap use at the time the function is called, the
     * minimum parameter and the call site should be chosen together. This
     * function should be called once when the application has just finished
     * starting up (and done all the baseline heap allocation it's likely to
     * do) and then not called after that during the same program execution.
     */
    static int checkHeapSizeIsSufficient(size_t minBytes);

    // called from the launchable module
    bool wbRegister();
    void wbUnregister();

private:
    void onGetRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;
    void onPutRequest(const whiteboard::Request& request, const whiteboard::ParameterList& parameters) OVERRIDE;

    bool startFirmwareUpdate(const whiteboard::ClientId clientId,
                             const whiteboard::ResourceId resourceId,
                             const whiteboard::ParameterList& parameters);

private:
    uint8_t m_updateProgress;
    /* SystemProvider::HEAP_CHECK_NOT_DONE_YET: not determined yet,
     * 0: low heap
     * 1: sufficient heap
     * */
    int8_t m_hasSufficientHeap;
};

} // namespace nea
