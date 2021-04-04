/* =============================================================================
 * Scene
 * 
 * 
 * ===========================================================================*/
#include "Scene.h"
#include <Engine/CollisionMgr.h>
#include <Engine/Scenes/SceneMgr.h>

namespace SDG
{
    Scene::Scene(): EntityComponent(true, true),
                     camera_(nullptr),
                     entities_(new EntityMgr),
                     collisions_(new CollisionMgr(Point(64, 64)))
    { }

    Scene::~Scene()
    {
        delete camera_;
        delete entities_;
        delete collisions_;
    }

    void Scene::Init()
    {
        if (!entities_)
            entities_ = new EntityMgr;
        if (!camera_)
            camera_ = new Camera2D(*GetGraphicsDeviceMgr());
        if (!collisions_)
            collisions_ = new CollisionMgr(Point(64, 64));

        // Grab persistent entities from the list
        for (auto &e: GetSceneMgr()->persistent_)
        {
            if (e->IsPersistent())
            {
                entities_->AddExistingEntity(*e);
            }
        }

        GetSceneMgr()->persistent_.clear();

        this->LoadContent();
        this->OnStart();
    }

    void Scene::Close()
    {
        this->OnEnd();

        // Adds entities marked persistent to the SceneMgr persistent entity list.
        // The rest are cleaned up by EntityMgr. (EntityMgr will not clean up
        // entities marked persistent).
        for (auto &e: *entities_)
        {
            if (e->IsPersistent())
                MakeEntityPersistent(*e);
        }

        delete entities_;
        entities_ = nullptr;

        delete camera_;
        camera_ = nullptr;

        delete collisions_;
        collisions_ = nullptr;
    }

    void Scene::DestroyEntity(Entity &entity)
    {
        if (entity.IsPersistent())
            entity.SetPersistent(false);
        entities_->DestroyEntity(entity);
    }

    void Scene::PostUpdate()
    {
        entities_->PostUpdate();
        collisions_->ProcessCollisions();
    }

    void Scene::MakeEntityPersistent(Entity &entity)
    {
        entity.SetPersistent(true);
        GetSceneMgr()->persistent_.emplace_back(&entity);
    }
}