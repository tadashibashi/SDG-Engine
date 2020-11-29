/* =============================================================================
 * GameTime.cpp
 * GameTime Implementation File.
 * ===========================================================================*/
#include "GameTime.h"
#include <SDL.h>

static const int MAX_DELTA_TICKS = 64;

namespace SDG
{
    GameTime::GameTime(unsigned long startingTicks):
            deltaTicks_(0), ticks_(0), startingTicks_(startingTicks) { }
    GameTime::~GameTime() = default;

    void GameTime::Update()
    {
        Uint32 currentTicks = SDL_GetTicks();
        deltaTicks_ = currentTicks - ticks_;
        if (deltaTicks_ > MAX_DELTA_TICKS) deltaTicks_ = MAX_DELTA_TICKS;

        ticks_ = currentTicks;
    }
}
