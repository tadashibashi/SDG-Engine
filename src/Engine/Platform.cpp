/* =============================================================================
 * Platform
 * 
 * 
 * ===========================================================================*/
#include "Platform.h"

#if defined(SDG_PLATFORM_EMSCRIPTEN) || defined(SDG_PLATFORM_WINDOWS) || defined(SDG_PLATFORM_ANDROID)
    #include <Engine/Graphics/OpenGL/GraphicsDeviceMgr_GL.h>
    #include <Engine/Graphics/OpenGL/SpriteBatch_GL.h>
#endif

// Create Platform Graphics Library
namespace SDG
{
    GraphicsDeviceMgr *CreateNativeGraphicsDeviceMgr()
    {
#if   defined(SDG_PLATFORM_MAC)
        return new GraphicsDeviceMgr_GL; // TODO: Make an Apple Metal implementation
#elif defined(SDG_PLATFORM_WINDOWS) || defined(SDG_PLATFORM_EMSCRIPTEN) || defined(SDG_PLATFORM_ANDROID)
        return new GraphicsDeviceMgr_GL;
#endif
        return nullptr;
    }

    SpriteBatch *CreateNativeSpriteBatch(GraphicsDevice *graphics)
    {
#if   defined(SDG_PLATFORM_WINDOWS) || defined(SDG_PLATFORM_EMSCRIPTEN) || defined(SDG_PLATFORM_ANDROID)
        return new SpriteBatch_GL(graphics);
#else
#error There are no sprite batch subclasses that support the current platform!
#endif
    }
}
