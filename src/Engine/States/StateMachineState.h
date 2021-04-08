/* =============================================================================
 * State
 * 
 * 
 * ===========================================================================*/
#pragma once
#ifndef SDG_STATE_H
#define SDG_STATE_H


#include <Engine/Time/GameTime.h>
#include <Engine/Events/Delegate.h>
#include <string>

namespace SDG
{
    template <typename T>
    class StateMachine;

    template <typename T>
    class StateMachineState
    {
        template <class>
        friend class StateMachine;
    public:
        explicit StateMachineState(T key) : key{std::move(key)}, currentTime{0} {}
        StateMachineState() = delete;
        StateMachineState(const StateMachineState &) = delete;

        /**
         * This event is fired when this state is entered.
         */
        StateMachineState<T> &OnEnter(void(*func)())
        {
            EnterEvent.AddListener(func);
            return *this;
        }

        /**
         * This event is fired when this state is entered.
         */
        template <typename Y>
        StateMachineState<T> &OnEnter(Y *object, void(Y::*memfunc)())
        {
            EnterEvent.AddListener(object, memfunc);
            return *this;
        }

        /**
         * This event is fired when this state is exited
         * \delegateparams time {float} parameter is the total time (in seconds) since the state was started.
         */
        template <typename Y>
        StateMachineState<T> &OnExit(Y *object, void(Y::*memfunc)(float))
        {
            ExitEvent.AddListener(object, memfunc);
            return *this;
        }

        /**
         * This event is fired when this state is entered.
         * \delegateparams time {float} parameter is the total time (in seconds) since the state was started.
         */
        StateMachineState<T> &OnExit(void(*func)(float))
        {
            ExitEvent.AddListener(func);
            return *this;
        }

        /**
         * This event is fired when this state is paused or resumed.
         * \param object The object whose member function will be called.
         * \param memfunc The member function of the object to call.
         * \delegateparams paused {bool}  true means this is a pause event. false means this is a resume event.
         * \delegateparams time   {float} parameter is the total time (in seconds) since the state was started.
         */
        template <typename Y>
        StateMachineState<T> &OnPause(Y *object, void(Y::*memfunc)(bool, float))
        {
            PauseEvent.AddListener(object, memfunc);
            return *this;
        }

        /**
         * This event is fired when this state is paused or resumed.
         * \param object The object whose member function will be called.
         * \param memfunc The member function of the object to call.
         * \delegate_parameters paused {bool}  true means this is a pause event. false means this is a resume event.
         * \delegate_parameters time   {float} parameter is the total time (in seconds) since the state was started.
         */
        StateMachineState<T> &OnPause(void(*func)(bool, float))
        {
            PauseEvent.AddListener(func);
            return *this;
        }

        template <typename Y>
        StateMachineState<T> &OnStep(Y *object, void(Y::*memfunc)(float, float))
        {
            StepEvent.AddListener(object, memfunc);
            return *this;
        }

        StateMachineState<T> &OnStep(void(*func)(float, float))
        {
            StepEvent.AddListener(func);
            return *this;
        }



        T GetKey() const { return key; }
    private:
        float currentTime;
        T key;

        [[nodiscard]]
        float GetCurrentTime() const { return currentTime; }
        void ResetTimer() { currentTime = 0; }

        // ========== Events ========================================================================
        /**
         * This event is fired when this state is entered.
         */
        Delegate<> EnterEvent;

        /**
         * This is event is fired every state update.
         * float parameter 1 is the delta time in seconds.
         * float parameter 2 is the current total time (in seconds) taken in this state since it started.
         */
        Delegate<float, float> StepEvent;

        /**
         * This event is fired when this state is exited.
         * float parameter is the total time (in seconds) taken in this state since it started.
         */
        Delegate<float> ExitEvent;

        /**
         * This event is fired when this state is resumed from paused status.
         * bool parameter is whether this event is pausing (true) or resuming (false).
         * float parameter is the total time (in seconds) taken in this state since it started.
         * Note: this time does not increase during its paused status
         */
        Delegate<bool, float> PauseEvent;

        void Update(float deltaSeconds)
        {
            currentTime += deltaSeconds;
            StepEvent.Invoke(deltaSeconds, currentTime);
        }

        void Enter()
        {
            EnterEvent.Invoke();
        }

        void Exit()
        {
            ExitEvent.Invoke(currentTime);
            ResetTimer();
        }

        void Resume()
        {
            PauseEvent.Invoke(false, currentTime);
        }

        void Pause()
        {
            PauseEvent.Invoke(true, currentTime);
        }
    };
}

#endif