#pragma once
// Copyright (c) Suunto Oy 2015. All rights reserved.

#include <mutex>
#include <condition_variable>
#include <cassert>

namespace whiteboard {
namespace device_discovery {

class semaphore
{
public:

    semaphore(const size_t count_ = 0, const size_t maxCount_ = UINT32_MAX)
        : count{ count_ }, maxCount{ maxCount_ }
    {
        assert(maxCount_ >= count_);
    }

    void notify(const size_t increment = 1u)
    {
        assert(increment >= 1u);
        std::unique_lock<std::mutex> lck(mtx);

        size_t newCount = count + increment;
        
        // Check agains maximum and overflow
        if ((newCount > maxCount) && (newCount < count))
        {
            newCount = maxCount;
        }

        while (count < newCount)
        {
            ++count;
            cv.notify_one();
        }
    }

    void wait()
    {
        std::unique_lock<std::mutex> lck(mtx);
        while (count == 0)
        {
            cv.wait(lck);
        }

        --count;
    }

    bool try_wait()
    {
        std::unique_lock<std::mutex> lck(mtx);

        if (count > 0)
        {
            --count;
            return true;
        }

        return false;
    }

    template <class Rep, class Period>
    bool try_wait_for(const std::chrono::duration<Rep, Period>& timeout_duration)
    {
        std::unique_lock<std::mutex> lck(mtx);

        if (!cv.wait_for(lck, timeout_duration, [&]() { return count != 0; }))
        {
            return false;
        }

        if (count > 0)
        {
            --count;
            return true;
        }

        return false;
    }

private:

    std::mutex mtx;
    std::condition_variable cv;
    size_t count;
    size_t maxCount;
};

} // namespace device_discovery
} // namespace whiteboard
