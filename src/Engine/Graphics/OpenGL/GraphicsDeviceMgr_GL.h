/* =============================================================================
 * GraphicsDeviceMgr
 * 
 * 
 * ===========================================================================*/
#pragma once

#include <Engine/Core.h>
#include <Engine/Graphics/GraphicsDeviceMgr.h>
#include <Engine/Graphics/OpenGL/GraphicsDevice_GL.h>

namespace SDG
{
    class GraphicsDeviceMgr_GL : public GraphicsDeviceMgr {
    public:
        GraphicsDeviceMgr_GL();
        ~GraphicsDeviceMgr_GL() override;

        // No copying.
        GraphicsDeviceMgr_GL(const GraphicsDeviceMgr_GL &) = delete;

        void Init(const char *title, int x, int y, int width, int height, unsigned int flags) override;
        void SetSwapInterval(SwapInterval interval) override;

        [[nodiscard]]
        SwapInterval GetSwapInterval() const override;
    };
}
