/* =============================================================================
 * Keyboard
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <unordered_set>
#include <unordered_map>
#include <SDL_events.h>
#include "Key.h"

namespace SDG
{
    class SDG_API Keyboard {
        friend class InputMgr;
    public:
        Keyboard();
        ~Keyboard();

        // Checks if a key is currently down.
        bool IsKeyDown(Key keycode) const
        {
            return (keys.count((SDL_Keycode)keycode) > 0 &&
                keys.at((SDL_Keycode)keycode).state == SDL_PRESSED);
        }

        // Checks if a key is currently not pressed.
        bool IsKeyUp(Key keycode) const
        {
            return !IsKeyDown(keycode);
        }

        // Checks if a key was just pressed this frame.
        bool KeyPressed(Key keycode) const
        {
            return justDown.find((SDL_Keycode)keycode) != justDown.cend();
        }

        // Checks if a key was just released this frame.
        bool KeyReleased(Key keycode) const
        {
            return justUp.find((SDL_Keycode)keycode) != justUp.cend();
        }

    private:
        void ProcessInput(const SDL_KeyboardEvent &);
        void PostUpdate();
        std::unordered_set<SDL_Keycode> justUp, justDown;
        std::unordered_map<SDL_Keycode, SDL_KeyboardEvent> keys;
    };
}
