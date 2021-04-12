/* =============================================================================
 * Scene
 * 
 * 
 * ===========================================================================*/
#include "Scene.h"
#include <Engine/CollisionMgr.h>
#include <Engine/Scenes/SceneMgr.h>
#include <Engine/Components/Transform.h>

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

        // Move camera to the {0, 0} position
        FRectangle bounds = camera_->GetWorldBounds();
        camera_->SetPosition(bounds.w/2, bounds.h/2);

        if (!collisions_)
            collisions_ = new CollisionMgr();

        // Grab persistent entities from the list
        for (Entity &e: GetSceneMgr()->persistent_)
        {
            if (e.IsPersistent())
            {
                SDG_CORE_LOG("Adding a persistent entity with tag {0}", e.GetTag());
                entities_->AddExistingEntity(e);
            }
        }

        GetSceneMgr()->persistent_.clear();
        entities_->ProcessChanges();

        this->LoadContent();
        this->OnStart();
    }

    void Scene::Close()
    {
        this->OnEnd();

        // Adds entities marked persistent to the SceneMgr persistent entity list.
        // The rest are cleaned up by EntityMgr. (EntityMgr will not clean up
        // entities marked persistent).
        for (Entity *e: *entities_)
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
        GetSceneMgr()->persistent_.emplace_back().Swap(entity);
    }

    Entity &Scene::CreateEntity(std::string tag, Vector2 position)
    {
        Entity &e = entities_->CreateEntity(std::move(tag));
        if (Transform *tf = e.Components()->Get<Transform>())
        {
            tf->SetPositionLocal(position);
        }

        return e;
    }

    Entity &Scene::CreateEntity(const std::function<void(Entity & )> &factoryFunction, Vector2 position)
    {
        Entity &e = entities_->CreateEntity(factoryFunction);
        if (Transform *tf = e.Components()->Get<Transform>())
        {
            tf->SetPositionLocal(position);
        }

        return e;
    }
}
