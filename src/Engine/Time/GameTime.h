/* =============================================================================
 * GameTime
 * 
 * Class for tracking total time passed since execution and delta time.
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>

namespace SDG
{
    class  GameTime {
    public:
        explicit GameTime(unsigned long startingTicks = 0);
        ~GameTime();

        // Prevent copying.
        GameTime(const GameTime &) = delete;
        GameTime &operator=(const GameTime &) = delete;

        void Update();

        // Returns ticks since execution and any starting ticks assigned at instantiation.
        [[nodiscard]] unsigned long GetTotalTicks() const { return ticks_ + startingTicks_; }
        // Returns only ticks since execution, foregoing calculating starting ticks.
        [[nodiscard]] unsigned long GetTicksSinceExe() const { return ticks_; }
        // Returns ticks since last frame with a maximum of 64 to prevent problems with physics.
        [[nodiscard]] unsigned long DeltaTicks() const { return deltaTicks_; }

    private:
        unsigned long ticks_;
        unsigned long deltaTicks_;
        unsigned long startingTicks_;
    };

}



