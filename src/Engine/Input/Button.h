/* =============================================================================
 * Button
 * 
 * enum class containing mouse button values
 * ===========================================================================*/
#pragma once
#include <SDL2/SDL_mouse.h>

namespace SDG
{
    enum class Button : Uint8
    {
        Left = SDL_BUTTON_LEFT,
        Right = SDL_BUTTON_RIGHT,
        Middle = SDL_BUTTON_MIDDLE
    };
}