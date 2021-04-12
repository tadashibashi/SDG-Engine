/* =============================================================================
 * GraphicsDevice
 * 
 * 
 * ===========================================================================*/
#include "GraphicsDevice_GL.h"
#include <SDL.h>
#include <Engine/Graphics/DefaultShader.h>

namespace SDG
{
    GraphicsDevice_GL::GraphicsDevice_GL() : window(nullptr)
    {
    }

    GraphicsDevice_GL::~GraphicsDevice_GL()
    {
        Close();
    }

    void GraphicsDevice_GL::Init(const char *title, int x, int y, int width, int height, unsigned int flags)
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

    void GraphicsDevice_GL::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        defaultShader_->Use();
    }

    void GraphicsDevice_GL::Close()
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

    void GraphicsDevice_GL::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }

    void GraphicsDevice_GL::SetBackBufferSize(int width, int height)
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

    void GraphicsDevice_GL::SwapBuffers() const
    {
        SDL_GL_SwapWindow(window);
        defaultShader_->Unuse();
    }
    
    Color GraphicsDevice_GL::GetClearColor()
    {
        GLfloat vals[4];
        glGetFloatv(GL_COLOR_CLEAR_VALUE, vals);
        return Color((GLubyte)(vals[0] * 255.f), (GLubyte)(vals[1] * 255.f),
                     (GLubyte)(vals[2] * 255.f), (GLubyte)(vals[3] * 255.f));
    }

    Point GraphicsDevice_GL::GetBackBufferSize()
    {
        Point p;
        SDL_GetWindowSize(window, &p.w, &p.h);
        return p;
    }

    void GraphicsDevice_GL::SetWindowTitle(const char *title)
    {
        SDL_SetWindowTitle(window, title);
    }

    void GraphicsDevice_GL::InitShader()
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

    Texture2D GraphicsDevice_GL::GenerateTexture(uint width, uint height, const std::vector<Pixel> &pixels) const
    {
        GLuint texid;
        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        GLubyte *pix = new GLubyte[pixels.size() * 4];
        GLubyte *current = pix;
        for (const Pixel &pixel : pixels)
        {
            *current++ = static_cast<GLubyte>(pixel.r);
            *current++ = static_cast<GLubyte>(pixel.g);
            *current++ = static_cast<GLubyte>(pixel.b);
            *current++ = static_cast<GLubyte>(pixel.a);
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
        Texture2D texture = Texture2D(texid, 1, 1);
        glBindTexture(GL_TEXTURE_2D, 0);
        delete[] pix;

        return texture;
    }
}