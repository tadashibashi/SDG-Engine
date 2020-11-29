/* =============================================================================
 * GraphicsDevice
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <cstddef>
#include <Engine/Math/Vector2.h>
#include <Engine/Content/Shader.h>
#include "Color.h"

class SDL_Window;

namespace SDG
{
    class SDG_API GraphicsDevice {
    public:
        GraphicsDevice();
        ~GraphicsDevice();

        // No copying.
        GraphicsDevice(const GraphicsDevice &) = delete;
        GraphicsDevice &operator=(const GraphicsDevice &) = delete;

        void Init(const char *title, int x, int y, int width, int height, unsigned int flags);
        void InitShader();
        void Clear();
        void Close();

        void SetClearColor(float r, float g, float b, float a);
        void SetBackBufferSize(int width, int height);
        void SwapBuffers() const;
        void SetWindowTitle(const char *title);

        Color GetClearColor();
        Point GetBackBufferSize();

        Shader *GetDefaultShader() { return defaultShader_; }
    private:
        SDL_Window *window;
        Shader *defaultShader_;
        void *context;
        Point size{1080, 720};
    };
}
