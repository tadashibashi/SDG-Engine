/* =============================================================================
 * EntityComponent
 * 
 * 
 * ===========================================================================*/
#include "EntityComponent.h"
#include <Engine/Scenes/SceneMgr.h>
namespace SDG
{
    EntityComponent::EntityComponent(bool isUpdatable, bool isDrawable) :
        Component(isUpdatable, isDrawable)
    {
    }

    // Static Scene Manager
    SceneMgr *EntityComponent::sceneMgr_ = nullptr;

    Scene *EntityComponent::GetScene()
    {
        assert(sceneMgr_ != nullptr);
        return sceneMgr_->CurrentScene();
    }
}