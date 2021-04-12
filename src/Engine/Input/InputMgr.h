/* =============================================================================
 * InputMgr
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <map>
#include <Engine/Events/Delegate.h>
#include <SDL_events.h>
#include "Keyboard.h"
#include <Engine/Input/Mouse.h>

namespace SDG
{
    class  InputMgr {
    public:
        InputMgr();
        ~InputMgr();

        // Pipes every user input event to their corresponding listeners.
        void ProcessInput();

        Delegate<const SDL_WindowEvent &>WindowEvent;
        Delegate<const SDL_KeyboardEvent &>KeyboardEvent;
        Delegate<const SDL_TextEditingEvent &>TextEditingEvent;
        Delegate<const SDL_TextInputEvent &>TextInputEvent;
        Delegate<const SDL_MouseMotionEvent &> MouseMotionEvent;
        Delegate<const SDL_MouseButtonEvent &> MouseButtonEvent;
        Delegate<const SDL_MouseWheelEvent &> MouseWheelEvent;
        Delegate<const SDL_JoyAxisEvent &> JoyAxisEvent;
        Delegate<const SDL_JoyBallEvent &> JoyBallEvent;
        Delegate<const SDL_JoyHatEvent &> JoyHatEvent;
        Delegate<const SDL_JoyButtonEvent &> JoyButtonEvent;
        Delegate<const SDL_JoyDeviceEvent &> JoyDeviceEvent;
        Delegate<const SDL_ControllerAxisEvent &> ControllerAxisEvent;
        Delegate<const SDL_ControllerButtonEvent &> ControllerButtonEvent;
        Delegate<const SDL_ControllerDeviceEvent &> ControllerDeviceEvent;
        Delegate<const SDL_AudioDeviceEvent &> AudioDeviceEvent;
        Delegate<const SDL_QuitEvent &> QuitEvent;
        Delegate<const SDL_UserEvent &> UserEvent;
        Delegate<const SDL_SysWMEvent &> SysWMEvent;
        Delegate<const SDL_TouchFingerEvent &> TouchFingerEvent;
        Delegate<const SDL_MultiGestureEvent &> MultiGestureEvent;
        Delegate<const SDL_DollarGestureEvent &> DollarGestureEvent;
        Delegate<const SDL_DropEvent &> DropEvent;

        [[nodiscard]] Keyboard *GetKeyboard() const { return keyboard; }
        [[nodiscard]] Mouse *GetMouse() const { return mouse; }
    private:
        Keyboard *keyboard;
        Mouse *mouse;
    };

}



