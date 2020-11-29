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

    class SDG_API GraphicsDeviceMgr {
    public:
        GraphicsDeviceMgr();
        ~GraphicsDeviceMgr();

        // No copying.
        GraphicsDeviceMgr(const GraphicsDeviceMgr &) = delete;
        GraphicsDeviceMgr &operator=(const GraphicsDeviceMgr &) = delete;

        void Init(const char *title, int x, int y, int width, int height, unsigned int flags);
        void SetSwapInterval(SwapInterval interval);
        [[nodiscard]] SwapInterval GetSwapInterval() const;

        GraphicsDevice &GetCurrentDevice() { return *current; }
    private:
        GraphicsDevice *current;
    };

}



