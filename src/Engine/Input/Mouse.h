/* =============================================================================
 * Mouse
 *
 *
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <unordered_set>
#include <unordered_map>
#include <SDL_events.h>
#include <Engine/Math/Vector2.h>
#include "Button.h"

namespace SDG
{
    class  Mouse {
        friend class InputMgr;
    public:
        Mouse();
        ~Mouse();

        bool ButtonPressed(Button button) const
        {
            return just_down.find((Uint8)button) != just_down.cend();
        }

        bool ButtonReleased(Button button) const
        {
            return just_up.find((Uint8)button) != just_up.cend();
        }

        bool ButtonDown(Button button)
        {
            return buttons.count((Uint8)button) > 0 &&
                buttons.at((Uint8)button).type == SDL_MOUSEBUTTONDOWN;
        }

        bool ButtonUp(Button button)
        {
            return !ButtonDown(button);
        }

        // Gets the screen position of the mouse.
        Vector2 GetPosition() const 
        { 
            return position; 
        }

    private:
        void ProcessInput(const SDL_MouseMotionEvent &motion);
        void ProcessInput(const SDL_MouseButtonEvent &button);
        void ProcessInput(const SDL_MouseWheelEvent &wheel);
        void PostUpdate();

        std::unordered_set<SDL_Keycode> just_up, just_down;
        std::unordered_map<Uint8, SDL_MouseButtonEvent> buttons;

        Vector2 position;
    };
}
