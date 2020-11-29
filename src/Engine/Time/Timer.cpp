/* =============================================================================
 * Timer
 * 
 * 
 * ===========================================================================*/
#include "Timer.h"

namespace SDG
{
    Timer::Timer(): Component(false, false)
    {
    }

    void Timer::Update() {
        long deltaTicks = (long)GetTime()->DeltaTicks();

        for (int i = 0; i < MAX_TIMERS; ++i)
        {
            auto &timer = timers_[i];

            if (timer.isPaused)
                continue;

            timer.ticksLeft -= deltaTicks;

            if (timer.ticksLeft <= 0)
            {
                timer.onFinish(i);
                timer.Reset(true);
            }
        }
    }
}