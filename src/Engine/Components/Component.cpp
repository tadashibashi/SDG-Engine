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

    void Component::SetActive(bool active)
    {
        if (active)
            attr_ |= Active;
        else
            attr_ &= ~Active;
    }

    void Component::SetRemoving(bool removing)
    {
        if (removing)
            attr_ |= ToBeRemoved;
        else
            attr_ &= ~ToBeRemoved;
    }
}