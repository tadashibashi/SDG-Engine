/* =============================================================================
 * SceneRunner
 * 
 * 
 * ===========================================================================*/
#include "SceneRunner.h"
#include "Scene.h"

namespace SDG
{
    SceneRunner::SceneRunner() :
        newScene_(nullptr), isRemoving_(false), isReplacing_(false), isStarting_(false)
    {
    }

    void SceneRunner::ProcessChanges()
    {
        // Process Removals
        if (isRemoving_)
        {
            if (Scene *current = CurrentScene())
            {
                current->Close();
                scenes_.pop();

                if (Scene *remaining = CurrentScene())
                {
                    remaining->OnResume();
                }
            }

            isRemoving_ = false;
            isReplacing_ = false; // prevent accidental double removal in one frame.
        }

        // Process Additions
        if (newScene_)
        {
            if (isReplacing_)  // end current scene
            {
                if (Scene *current = CurrentScene())
                {
                    current->OnEnd();
                    current->Close();
                    scenes_.pop();
                }

                isReplacing_ = false;
            }
            else               // pause current scene
            {
                if (Scene *current = CurrentScene())
                {
                    current->OnPause();
                }
            }
            isStarting_ = false;
            scenes_.emplace(newScene_);
            newScene_->Init();

            if (!isStarting_)
                newScene_ = nullptr;
        }
    }

    void SceneRunner::Update()
    {
        ProcessChanges();

        if (Scene *current = CurrentScene())
        {
            current->Update();
            current->PostUpdate();
        }
    }

    void SceneRunner::Draw()
    {
        if (Scene *current = CurrentScene())
        {
            current->Draw();
        }
    }

    void SceneRunner::StartScene(Scene *scene, bool replaceCurrent)
    {
        isStarting_ = true;
        newScene_ = scene;
        isReplacing_ = replaceCurrent;
    }

    void SceneRunner::StopCurrentScene()
    {
        isRemoving_ = true;
    }
}