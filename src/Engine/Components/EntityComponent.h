/* =============================================================================
 * EntityComponent
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include "Component.h"

#include <cassert>

// Convenient access for sub-classes.
#include <Engine/Graphics/SpriteBatch.h>
#include <Engine/Content/ContentMgr.h>
#include <Engine/EntityMgr.h>
#include <Engine/Time/GameTime.h>
#include <Engine/Input/InputMgr.h>

namespace SDG
{
    class SceneMgr;
    class Scene;

    class EntityComponent : public Component {
    public:
        EntityComponent(bool isUpdatable, bool isDrawable);
        static void Provide(SceneMgr *sceneMgr) { sceneMgr_ = sceneMgr; }

    protected:

        static SceneMgr *GetSceneMgr()
        {
            assert(sceneMgr_ != nullptr);
            return sceneMgr_;
        }

        // Gets the current Scene
        static Scene *GetScene();

    private:
        static SceneMgr *sceneMgr_;
    };
}
