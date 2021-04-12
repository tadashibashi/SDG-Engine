/* =============================================================================
 * GraphicsDevice
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Graphics/GraphicsDevice.h>

struct SDL_Window;

namespace SDG
{
    class GraphicsDevice_GL : public GraphicsDevice {
    public:
        GraphicsDevice_GL();
        ~GraphicsDevice_GL() override;

        // No copying.
        GraphicsDevice_GL(const GraphicsDevice_GL &) = delete;

        void Init(const char *title, int x, int y, int width, int height, unsigned int flags) override;
        void InitShader() override;
        void Clear() override;
        void Close() override;

        void SetClearColor(float r, float g, float b, float a) override;
        void SetBackBufferSize(int width, int height) override;
        void SwapBuffers() const override;
        void SetWindowTitle(const char *title) override;

        [[nodiscard]]
        Texture2D GenerateTexture(uint width, uint height, const std::vector<Pixel> &pixels) const override;

        [[nodiscard]]
        Color GetClearColor() override;
        [[nodiscard]]
        Point GetBackBufferSize() override;

    private:
        SDL_Window *window;
    };
}
