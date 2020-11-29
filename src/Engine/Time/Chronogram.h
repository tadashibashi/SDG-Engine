/* =============================================================================
 * Chronogram
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <map>
#include <Engine/Components/Component.h>

namespace SDG
{
    class GameTime;

    class SDG_API Chronogram: public Component {
    public:
        explicit Chronogram():
            Component(false, false),
            time_(nullptr)
        { }

        void Init() override
        {
            time_ = GetTime();
        }

        // Checks the time since a timer was started (in milliseconds)
        [[nodiscard]] unsigned long CheckTicks(unsigned long timerIndex) const;
        [[nodiscard]] double CheckSeconds(unsigned long timerIndex) const;

        // Starts or restarts a timer.
        void StartTimer(unsigned long timerIndex);

    private:
        std::map<unsigned long, unsigned long> timers_;
        const GameTime *time_;
    };

}



