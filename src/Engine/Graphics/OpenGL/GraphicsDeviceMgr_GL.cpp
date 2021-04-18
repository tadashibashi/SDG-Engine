/* =============================================================================
 * GraphicsDeviceMgr
 * 
 * 
 * ===========================================================================*/
#include "GraphicsDeviceMgr_GL.h"
#include <SDL.h>
#include <iostream>

namespace SDG
{
    GraphicsDeviceMgr_GL::GraphicsDeviceMgr_GL() : GraphicsDeviceMgr()
    {
        current = new GraphicsDevice_GL;
    }

    GraphicsDeviceMgr_GL::~GraphicsDeviceMgr_GL()
    {
        delete current;
    }

    void
    GraphicsDeviceMgr_GL::Init(const char *title, int x, int y, int width, int height, unsigned int flags)
    {
        // ============ SDL ===========
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
        {
            std::cerr << "Error initializing SDL: " << SDL_GetError() << '\n';
        }

        // ============ Set GL Attributes (before window creation) ============
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
#if defined(SDG_PLATFORM_ANDROID) || defined(SDG_PLATFORM_EMSCRIPTEN)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#elif defined(SDG_PLATFORM_WINDOWS) || defined(SDG_PLATFORM_APPLE)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
        // Create GL Window
        current->Init(title, x, y, width, height, flags);

        // VSync.
        SetSwapInterval(SwapInterval::Vsync);

        // Alpha blending.
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if !defined(SDG_PLATFORM_EMSCRIPTEN)
        // ============ Initialize GLEW =============
        glewExperimental = GL_TRUE;
        GLenum result = glewInit();
        if (result != GLEW_OK)
        {
            std::cerr << "Error initializing GLEW: " << glewGetErrorString(result)
                      << '\n';
            current->Close();
            SDL_Quit();
        }
#endif
        current->InitShader();
    }
    
    void GraphicsDeviceMgr_GL::SetSwapInterval(SwapInterval interval)
    {
        if (interval == SwapInterval::Immediate) 
        {
            SDL_GL_SetSwapInterval(0);
        }
        else
        {
            // Attempts adaptive vsync (-1), and uses regular vsync if unsupported.
            if (SDL_GL_SetSwapInterval(-1) == -1)
            {
                SDL_GL_SetSwapInterval(1);
            }
        } 
    }

    SwapInterval GraphicsDeviceMgr_GL::GetSwapInterval() const
    {
        return SDL_GL_GetSwapInterval() == 0
            ? SwapInterval::Immediate
            : SwapInterval::Vsync;
    }
}