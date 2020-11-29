/* =============================================================================
 * InputMgr
 * 
 * 
 * ===========================================================================*/
#include "InputMgr.h"
#include <stdexcept>
#include <Engine/Log.h>

namespace SDG
{
    InputMgr::InputMgr() : keyboard(new Keyboard), mouse(new Mouse)
    {
    }

    InputMgr::~InputMgr()
    {
        delete keyboard;
        delete mouse;
    }

    void InputMgr::ProcessInput()
    {
        keyboard->PostUpdate();

        SDL_Event ev;
        while(SDL_PollEvent(&ev))
        {
            switch(ev.type)
            {
                case SDL_WINDOWEVENT:
                    WindowEvent(ev.window);
                    break;
                case SDL_KEYUP: case SDL_KEYDOWN:
                    keyboard->ProcessInput(ev.key);
                    KeyboardEvent(ev.key);
                    break;
                case SDL_TEXTEDITING:
                    TextEditingEvent(ev.edit);
                    break;
                case SDL_TEXTINPUT:
                    TextInputEvent(ev.text);
                    break;
                case SDL_MOUSEMOTION:
                    mouse->ProcessInput(ev.motion);
                    MouseMotionEvent(ev.motion);
                    break;
                case SDL_MOUSEBUTTONUP: case SDL_MOUSEBUTTONDOWN:
                    mouse->ProcessInput(ev.button);
                    MouseButtonEvent(ev.button);
                    break;
                case SDL_MOUSEWHEEL:
                    mouse->ProcessInput(ev.wheel);
                    MouseWheelEvent(ev.wheel);
                    break;
                case SDL_JOYAXISMOTION:
                    JoyAxisEvent(ev.jaxis);
                    break;
                case SDL_JOYBALLMOTION:
                    JoyBallEvent(ev.jball);
                    break;
                case SDL_JOYHATMOTION:
                    JoyHatEvent(ev.jhat);
                    break;
                case SDL_JOYBUTTONUP: case SDL_JOYBUTTONDOWN:
                    JoyButtonEvent(ev.jbutton);
                    break;
                case SDL_JOYDEVICEADDED: case SDL_JOYDEVICEREMOVED:
                    JoyDeviceEvent(ev.jdevice);
                    break;
                case SDL_CONTROLLERAXISMOTION:
                    ControllerAxisEvent(ev.caxis);
                    break;
                case SDL_CONTROLLERBUTTONUP:
                case SDL_CONTROLLERBUTTONDOWN:
                    ControllerButtonEvent(ev.cbutton);
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                case SDL_CONTROLLERDEVICEREMOVED:
                case SDL_CONTROLLERDEVICEREMAPPED:
                    ControllerDeviceEvent(ev.cdevice);
                    break;
                case SDL_AUDIODEVICEADDED:
                case SDL_AUDIODEVICEREMOVED:
                    AudioDeviceEvent(ev.adevice);
                    break;
                case SDL_QUIT:
                    QuitEvent(ev.quit);
                    break;
                case SDL_USEREVENT:
                    UserEvent(ev.user);
                    break;
                case SDL_SYSWMEVENT:
                    SysWMEvent(ev.syswm);
                    break;
                case SDL_FINGERMOTION:
                case SDL_FINGERUP:
                case SDL_FINGERDOWN:
                    TouchFingerEvent(ev.tfinger);
                    break;
                case SDL_MULTIGESTURE:
                    MultiGestureEvent(ev.mgesture);
                    break;
                case SDL_DOLLARGESTURE:
                case SDL_DOLLARRECORD:
                    DollarGestureEvent(ev.dgesture);
                    break;
                case SDL_DROPFILE: case SDL_DROPTEXT:
                case SDL_DROPBEGIN: case SDL_DROPCOMPLETE:
                    DropEvent(ev.drop);
                    break;
                case SDL_CLIPBOARDUPDATE:
                    // not implemented.
                    break;
                default:
                    SDG_ERR("Unknown Event processed.");
                    break;
            }
        }
    }
}