/* =============================================================================
 * SceneManager
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include "SceneRunner.h"
#include "SceneCache.h"

namespace SDG
{
    class SDG_API SceneMgr {
        friend class Scene;
    public:
        SceneMgr();
        virtual ~SceneMgr();

        // Starts a scene, either replacing or pausing the current.
        template <SceneType T>
        void Start(bool replaceCurrent = true)
        {
            if (Scene *scene = scenes_.GetScene<T>())
            {
                runtime_.StartScene(scene, replaceCurrent);
            }
        }

        // Stops and ends the current Scene.
        void StopCurrent()
        {
            runtime_.StopCurrentScene();
        }

        // Register a scene with the SceneManager for use.
        template <SceneType T>
        void Register()
        {
            scenes_.RegisterScene<T>();
        }

        // Gets a Scene for debuggign purposes.
        template <SceneType T>
        [[nodiscard]] const T *Get() const
        {
            return scenes_.GetScene<T>();
        }

        // Retrieves the number of scenes registered for debugging purposes.
        [[nodiscard]] size_t Count() const
        {
            return scenes_.Count();
        }

        void Update()
        {
            runtime_.Update();
        }

        void Draw()
        {
            runtime_.Draw();
        }

        Scene *CurrentScene()
        {
            return runtime_.CurrentScene();
        }

        [[nodiscard]]
        std::vector<Entity *> GetPersistentEntities() const { return persistent_; }

    protected:
        SceneRunner runtime_;
        SceneCache scenes_;
        std::vector<Entity *> persistent_;
    };

}



