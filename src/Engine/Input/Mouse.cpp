#include "Mouse.h"

namespace SDG
{
    Mouse::Mouse()
    {

    }

    Mouse::~Mouse()
    {

    }

    void Mouse::ProcessInput(const SDL_MouseMotionEvent &motion)
    {
        position = Vector2((float)motion.x, (float)motion.y);
    }

    void Mouse::ProcessInput(const SDL_MouseButtonEvent &button)
    {
        if (button.type == SDL_MOUSEBUTTONDOWN)
        {
            just_down.emplace(button.button);
            buttons[button.button] = button;
        }
        else
        {
            just_up.emplace(button.button);
            buttons[button.button] = button;
        }
    }

    void Mouse::ProcessInput(const SDL_MouseWheelEvent &wheel)
    {
        // Unimplemented.
    }

    void Mouse::PostUpdate()
    {
        just_down.clear();
        just_up.clear();
    }


}