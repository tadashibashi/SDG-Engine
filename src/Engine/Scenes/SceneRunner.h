/* =============================================================================
 * SceneRunner
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <stack>
#include "Scene.h"

namespace SDG
{
    class Scene;

    class SceneRunner {
        friend class SceneMgr;
    public:
        SceneRunner();

        // Returns current scene or nullptr if there is none.
        Scene *CurrentScene()
        {
            return scenes_.empty() ? nullptr : scenes_.top();
        }

        // Start scene
        // replaceCurrent, if true, will stop the current scene.
        // If false, it will pause the current scene instead until
        // this one ends.
        void StartScene(Scene *scene, bool replaceCurrent = true);

        // Stop current scene.
        void StopCurrentScene();
        void Update();
        void Draw();
    private:

        std::stack<Scene *> scenes_;
        void ProcessChanges();
        bool isRemoving_;
        bool isReplacing_;
        bool isStarting_;
        Scene *newScene_;
    };
}



