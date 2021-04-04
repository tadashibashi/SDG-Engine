/* =============================================================================
 * Scene
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Graphics/Camera2D.h>
#include <Engine/Components/EntityComponent.h>

namespace SDG
{
    class CollisionMgr;

    class SDG_API Scene : public EntityComponent
    {
        friend class SceneRunner;
    public:
        Scene();
        ~Scene() override;

        Entity &CreateEntity(std::string tag = "")
        {
            return entities_->CreateEntity(std::move(tag));
        }

        Entity &CreateEntity(const std::function<void(Entity &)> &factoryFunction)
        {
            return entities_->CreateEntity(factoryFunction);
        }

        static void MakeEntityPersistent(Entity &entity);
        static void RemoveEntityPersistence(Entity &entity);

        void DestroyEntity(Entity &entity);

        // Scene creation/content loading/interpretation from files
        // Called right before scene start.
        virtual void LoadContent() = 0;
        // Called on the first frame of the scene
        virtual void OnStart() = 0;
        // Called on the last frame of the scene
        virtual void OnEnd() = 0;
        // Called whenever the scene is paused. E.g. another scene
        // such as a menu interface scene causes a pause.
        virtual void OnPause() = 0;
        // Called when scene resumes after a pause.
        virtual void OnResume() = 0;

        void Update() override
        {
            entities_->Update();
        }

        void PostUpdate() override;

        void Draw() override
        {
            GetSpriteBatch()->Begin(camera_->GetMatrix(), SortOrder::BackToFront);
            entities_->Draw();
            GetSpriteBatch()->End();
            GetSpriteBatch()->RenderBatches();
        }

        EntityMgr *GetEntityMgr() { return entities_; }
        Camera2D *GetCamera() { return camera_; }
        CollisionMgr *GetCollisions() { return collisions_; }

        Entity *GetFirstEntityWithTag(const std::string &tag)
        {
            return entities_->GetFirstEntityWithTag(tag);
        }

        [[nodiscard]]
        const std::vector<Entity *> &GetEntityTagList(const std::string &tag) const
        {
            return entities_->GetEntityTagList(tag);
        }

    private:
        void Init() override;
        void Close() override;

        Camera2D *camera_;
        EntityMgr *entities_;
        CollisionMgr *collisions_;
    };
}
