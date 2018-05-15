#pragma once

#include <chrono>
#include <ctime>

/*
    Usage:

    void Function()
    {
        startTimer(timerName);
        ... code to time ...
        float t = stopTimer(timerName);
    }
*/

namespace rt
{
    class Timer
    {
    private:
        std::chrono::time_point<std::chrono::system_clock> mStart;
        std::chrono::time_point<std::chrono::system_clock> mEnd;

    public:
        void start()
        {
            mStart = std::chrono::system_clock::now();
        }

        float stop()
        {
            mEnd = std::chrono::system_clock::now();
            return getTime();
        }

        float getTime()
        {
            return std::chrono::duration<float>(mEnd - mStart).count();
        }
    };
}