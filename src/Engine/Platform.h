/* =============================================================================
 * Platform
 *     Contains interfaces for platform specific code pertaining to graphics and audio
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Graphics/GraphicsDeviceMgr.h>
#include <Engine/Graphics/SpriteBatch.h>

namespace SDG
{
    GraphicsDeviceMgr *CreateNativeGraphicsDeviceMgr();
    SpriteBatch *CreateNativeSpriteBatch(GraphicsDevice *graphics);
}
