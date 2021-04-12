/* =============================================================================
 * GraphicsDevice
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <cstddef>
#include <Engine/Math/Vector2.h>
#include <Engine/Content/Pixel.h>
#include <Engine/Content/Shader.h>
#include <Engine/Content/Texture2D.h>
#include <vector>

#include "Color.h"

struct SDL_Window;

namespace SDG
{
    class GraphicsDevice {
    public:
        GraphicsDevice() : defaultShader_{}, context{}, size{} {}
        virtual ~GraphicsDevice() = default;

        // Prevent copying.
        GraphicsDevice(const GraphicsDevice &) = delete;
        GraphicsDevice &operator=(const GraphicsDevice &) = delete;



        // Lifetime
        virtual void Init(const char *title, int x, int y, int width, int height, unsigned int flags) = 0; // TODO: Wrap flags into enum for cross-platform flag usage
        virtual void InitShader() = 0;
        virtual void Clear() = 0;
        virtual void Close() = 0;

        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void SetBackBufferSize(int width, int height) = 0;
        virtual void SwapBuffers() const = 0;
        virtual void SetWindowTitle(const char *title) = 0;

        [[nodiscard]]
        virtual Texture2D GenerateTexture(uint width, uint height, const std::vector<Pixel> &pixels) const = 0;

        [[nodiscard]]
        virtual Color GetClearColor() = 0;
        [[nodiscard]]
        virtual Point GetBackBufferSize() = 0;

        Shader *GetDefaultShader() { return defaultShader_; }
    protected:
        Shader *defaultShader_;
        void *context;
        Point size{1080, 720};
    };
}
