/* =============================================================================
 * GraphicsDeviceMgr
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Graphics/GraphicsDevice.h>

namespace SDG
{
    enum class SwapInterval : char {
        Immediate,
        Vsync
    };

    class GraphicsDeviceMgr {
    public:
        GraphicsDeviceMgr() : current{} {}
        virtual ~GraphicsDeviceMgr() = default;

        // No copying.
        GraphicsDeviceMgr(const GraphicsDeviceMgr &) = delete;
        GraphicsDeviceMgr &operator=(const GraphicsDeviceMgr &) = delete;

        virtual void Init(const char *title, int x, int y, int width, int height, unsigned int flags) = 0;
        virtual void SetSwapInterval(SwapInterval interval) = 0;
        [[nodiscard]]
        virtual SwapInterval GetSwapInterval() const = 0;

        GraphicsDevice &GetCurrentDevice() { return *current; }
    protected:
        GraphicsDevice *current;
    };
}
