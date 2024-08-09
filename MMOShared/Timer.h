//
// Created by Keegan Beaulieu on 2024-07-13.
//

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
public:
    Timer();

    static void wait_seconds(float seconds);

    void reset();
    float read_seconds();

private:
    std::chrono::time_point<std::chrono::steady_clock> start;
};

#endif //TIMER_H
