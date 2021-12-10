#include "FPSCounter.h"

FPSCounter::FPSCounter() : last(0), lastfps(0), frames(1000)
{
}

float FPSCounter::GetFPS()
{
    // Measure speed
    double current = AppTime::GetElapsed();
    frames++;
    if (current - last >= 1.0) { // If last prinf() was more than 1 sec ago
        // printf and reset timer
        lastfps = frames;
        frames = 0;
        last = current;
    }

    return lastfps;
}
