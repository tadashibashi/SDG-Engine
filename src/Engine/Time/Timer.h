/* =============================================================================
 * Timer
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Components/Component.h>
#include <Engine/Events/Delegate.h>
#include <array>

static const int MAX_TIMERS = 10;

namespace SDG
{

    class Timer: public Component {
        struct Clock
        {
            bool isPaused{false};
            long ticksLeft{0};
            long setTime{};

            Delegate<int> onFinish;
            // Helpers
            void Reset(bool pause)
            {
                isPaused = pause;
                ticksLeft = setTime;
            }

            void Set(long ticks, bool pause = false)
            {
                assert(ticks > 0);

                setTime = ticks;
                Reset(pause);
            }

            void Pause()
            {
                isPaused = true;
            }

            void Resume()
            {
                isPaused = false;
            }
        };

    public:
        Timer();

        void Update() override;

        void Pause(int index)
        {
            timers_[index].Pause();
        }

        long GetTicksLeft(int index)
        {
            return timers_[index].ticksLeft;
        }

        bool IsPaused(int index)
        {
            return timers_[index].isPaused;
        }

        void Resume(int index)
        {
            timers_[index].Resume();
        }

        void AddListener(int index, void(*listener)(int))
        {
            timers_[index].onFinish.AddListener(listener);
        }

        template <typename T>
        void AddListener(int index, T *obj, void(T::*listener)(int))
        {
            timers_[index].onFinish.AddListener(obj, listener);
        }

        void Set(int index, long ticks, bool pause = false)
        {
            timers_[index].Set(ticks, pause);
        }

        void Reset(int index, bool pause = false)
        {
            timers_[index].Reset(pause);
        }
    private:
        std::array<Clock, MAX_TIMERS> timers_;
    };

}



