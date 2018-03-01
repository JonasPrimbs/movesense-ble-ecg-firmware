#pragma once
#include <cstdlib>
#include <cstdint>

class HeapSizeValidator
{
public:
    /** Check current heap memory against minimum threshold value.
     *
     * Returned values correspond to the enum MemorySufficiencyTestResult
     * defined in api/system/memory.yaml.
     *
     * Result is based on heap use at the time the function is called, the
     * minimum parameter and the call site should be chosen together. This
     * function should be called once when the application has just finished
     * starting up (and done all the baseline heap allocation it's likely to
     * do) and then not called after that during the same program execution.
     */
    static uint8_t checkHeapSizeIsSufficient(const size_t minBytes);
};
