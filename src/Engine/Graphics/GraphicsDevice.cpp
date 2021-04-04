/* =============================================================================
 * GraphicsDevice
 * 
 * 
 * ===========================================================================*/
#include "GraphicsDevice.h"
#include <SDL.h>
#include <Engine/Graphics/DefaultShader.h>

namespace SDG
{
    GraphicsDevice::GraphicsDevice() : window(nullptr), context(nullptr), defaultShader_(nullptr)
    {
    }

    GraphicsDevice::~GraphicsDevice()
    {
        Close();
    }

    void GraphicsDevice::Init(const char *title, int x, int y, int width, int height, unsigned int flags)
    {
        SDL_Window *win = SDL_CreateWindow(
                title,
                x,
                y,
                width, height,
                SDL_WINDOW_OPENGL | flags);

        if (!win)
        {
            SDG_ERR("Error creating SDL Window: {0}", SDL_GetError());
            return;
        }

        SDL_GLContext ctx = SDL_GL_CreateContext(win);
        if (!ctx)
        {
            SDG_ERR("Error creating GL Context: {0}", SDL_GetError());
            SDL_DestroyWindow(window);
            return;
        }

        window = win;
        context = ctx;
    }

    void GraphicsDevice::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        defaultShader_->Use();
    }

    void GraphicsDevice::Close()
    {
        if (context)
        {
            SDL_GL_DeleteContext(context);
            context = nullptr;
        }

        if (window)
        {
            SDL_DestroyWindow(window);
            window = nullptr;
        }

    }

    void GraphicsDevice::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void GraphicsDevice::SetBackBufferSize(int width, int height)
    {
        if (window)
        {
            SDL_SetWindowSize(window, width, height);
            glViewport(0, 0, width, height);
            SwapBuffers();
        }
        else
        {
            size = Point(width, height);
        }

    }

    void GraphicsDevice::SwapBuffers() const
    {
        SDL_GL_SwapWindow(window);
        defaultShader_->Unuse();
    }

    Color GraphicsDevice::GetClearColor()
    {
        GLfloat vals[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, vals);
        return Color(vals[0] * 255.f, vals[1] * 255.f, vals[2] * 255.f, vals[3] * 255.f);
    }

    Point GraphicsDevice::GetBackBufferSize()
    {
        Point p;
        SDL_GL_GetDrawableSize(window, &p.w, &p.h);
        return p;
    }

    void GraphicsDevice::SetWindowTitle(const char *title)
    {
        SDL_SetWindowTitle(window, title);
    }

    void GraphicsDevice::InitShader()
    {
        if (defaultShader_)
            return;

        auto *shader = new Shader();

        // Load default shader
        bool shaderLoadResult;
        shaderLoadResult = shader->LoadFromString(DEFAULT_VERTEX_SHADER,
                                                  DEFAULT_FRAGMENT_SHADER);
        if (!shaderLoadResult)
        {
            delete shader;
            return;
        }

        shaderLoadResult = shader->LinkProgram();
        if (!shaderLoadResult)
        {
            delete shader;
            return;
        }

        shader->AddAttribute("vertex_position");
        shader->AddAttribute("vertex_color");
        shader->AddAttribute("vertex_uv");

        defaultShader_ = shader;
        shader->Use(); // Use for any initialization logic needing to access shader
    }
}