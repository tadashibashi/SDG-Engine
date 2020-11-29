/* =============================================================================
 * Component
 * 
 * 
 * ===========================================================================*/
#include "Component.h"

namespace SDG
{
    SpriteBatch *Component::spriteBatch_ = nullptr;
    const GameTime *Component::gameTime_ = nullptr;
    ContentMgr *Component::contentMgr_ = nullptr;
    InputMgr *Component::inputMgr_ = nullptr;
    GraphicsDeviceMgr *Component::graphics_ = nullptr;
}