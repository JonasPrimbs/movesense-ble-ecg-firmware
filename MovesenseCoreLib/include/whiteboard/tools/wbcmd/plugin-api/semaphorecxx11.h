#pragma once
#include <mutex>
#include <condition_variable>

class semaphore
{
public:

    semaphore(int count_ = 0) : count{ count_ }
    {}

    void notify()
    {
        std::unique_lock<std::mutex> lck(mtx);
        ++count;
        cv.notify_one();
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
    
    bool try_wait() {
        std::unique_lock<std::mutex> lck(mtx);

        if (count > 0) {
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
    int count;
};
