/* =============================================================================
 * StateMachine
 * 
 * 
 * ===========================================================================*/
#pragma once
#ifndef SDG_STATEMACHINE_H
#define SDG_STATEMACHINE_H

#include "StateMachineState.h"
#include <map>
#include <stack>
#include <xutility>
#include <Engine/Log.h>
#include <Engine/Events/Delegate.h>

namespace SDG
{
    template <typename T>
    class StateMachine {
    public:
        ~StateMachine()
        {
            for (auto &[key, state] : states)
            {
                delete state;
            }
        }

        Delegate<T> OnStateStart;
        Delegate<T, float> OnStateExit;

        StateMachineState<T> &AddState(T key)
        {
            // Delete any preexisting state
            auto it = states.find(key);
            if (it != states.end())
            {
                SDG_CORE_WARN("SDG::StateMachine::AddState: Overwriting duplicate state with key {0}.", key);
                delete it->second;
            }

            // Create, store, and return new state
            StateMachineState<T> *newState = new StateMachineState<T>(key);
            states[key] = newState;
            return *newState;
        }

        StateMachineState<T> *GetState(T key)
        {
            auto it = states.find(key);
            return (it == states.end()) ? nullptr : it->second;
        }

        /**
         * Starts a state with the specified key
         * @param key the key of the state to start
         * @param replaceCurrent whether or not to replace the currently running state. False will simply pause it until
         * this newly called state is stopped
         * @return whether or not state was successfully queued to start
         */
        bool StartState(T key, bool replaceCurrent = true)
        {
            auto it = states.find(key);
            if (it == states.end())
            {
                SDG_CORE_ERR("SDG::StateMachine:RunState: Tried to run state with key {0}, but none was found.", key);
                return false;
            }

            nextState = states[key];
            toReplace = replaceCurrent;
            toStart = true;
            return true;
        }

        void SetPaused(bool paused) { this->paused = paused; }
        [[nodiscard]]
        bool GetPaused() const { return paused; }

        /**
         * @return Current state, or nullptr if none are currently running
         */
        [[nodiscard]]
        StateMachineState<T> *GetCurrentState()
        {
            return running.empty() ? nullptr : running.top();
        }

        void Update(const GameTime &time)
        {
            if (!paused)
            {
                ProcessChanges();

                if (StateMachineState<T> *current = GetCurrentState())
                {
                    current->Update((float)time.DeltaTicks() * 0.001f);
                }
            }
        }

    private:
        std::stack<StateMachineState<T> *> running;
        std::map<T, StateMachineState<T> *> states;
        StateMachineState<T> *nextState{};
        bool toRemove{};
        bool toReplace{};
        bool toStart{};
        bool paused{};

        void ProcessChanges()
        {
            if (toRemove)
            {
                if (StateMachineState<T> *current = GetCurrentState())
                {
                    OnStateExit(current->GetKey(), current->GetCurrentTime());
                    current->Exit();
                    running.pop();

                    if (StateMachineState<T> *remaining = GetCurrentState())
                    {
                        remaining->Resume();
                    }
                }

                toRemove = false;
                toReplace = false; // prevent accidental double removal in one frame.
            }

            // Process Additions
            if (nextState)
            {
                if (toReplace)  // end current state
                {
                    if (StateMachineState<T> *current = GetCurrentState())
                    {
                        current->Exit();
                        running.pop();
                    }

                    toReplace = false;
                }
                else            // pause current state
                {
                    if (StateMachineState<T> *current = GetCurrentState())
                    {
                        current->Pause();
                    }
                }

                running.emplace(nextState);
                OnStateStart(nextState->GetKey());
                toStart = false;
                nextState->Enter();

                // User may have called StartState in the enter event.
                // If so, the flag will be flipped to true and we do not want to overwrite the nextState.
                if (!toStart)
                    nextState = nullptr;
            }
        }

    };
}


#endif



