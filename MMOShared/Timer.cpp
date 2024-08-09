//
// Created by Keegan Beaulieu on 2024-07-13.
//

#include "Timer.h"

#include <thread>

#define SECONDS_TO_NS 1e+9

Timer::Timer()
{
    start = std::chrono::high_resolution_clock::now();
}

void Timer::wait_seconds(float seconds)
{
    std::this_thread::sleep_for(std::chrono::nanoseconds((long)(seconds*SECONDS_TO_NS)));
}

float Timer::read_seconds()
{
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> seconds = now - start;

    return seconds.count();
}

void Timer::reset()
{
    start = std::chrono::high_resolution_clock::now();
}
