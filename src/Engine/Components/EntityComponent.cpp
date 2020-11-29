/* =============================================================================
 * EntityComponent
 * 
 * 
 * ===========================================================================*/
#include "EntityComponent.h"
#include <Engine/Scenes/SceneMgr.h>
namespace SDG
{
    EntityComponent::EntityComponent(bool isUpdatable, bool isDrawable):
        Component(isUpdatable, isDrawable)
    {
    }

    SceneMgr *EntityComponent::sceneMgr_ = nullptr;

    Scene *EntityComponent::GetCurrentScene()
    {
        assert(sceneMgr_ != nullptr);
        return sceneMgr_->CurrentScene();
    }
}