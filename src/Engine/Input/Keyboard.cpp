/* =============================================================================
 * Keyboard
 * 
 * 
 * ===========================================================================*/
#include "Keyboard.h"
#include "InputMgr.h"

namespace SDG
{
    Keyboard::Keyboard()
    {
    }

    Keyboard::~Keyboard()
    {
    }

    void Keyboard::ProcessInput(const SDL_KeyboardEvent &key)
    {
        SDL_Keycode code = key.keysym.sym;

        switch(key.type) {
            case SDL_KEYDOWN:
                if (key.repeat == 0) // only act on first 'real' keypress
                    justDown.emplace(code);
                keys[code] = key;
                break;
            case SDL_KEYUP:
                justUp.emplace(code);
                keys[code] = key;
                break;
        }
    }

    void Keyboard::PostUpdate()
    {
        justUp.clear();
        justDown.clear();
    }
}