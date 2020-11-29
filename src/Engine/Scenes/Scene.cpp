/* =============================================================================
 * Scene
 * 
 * 
 * ===========================================================================*/
#include "Scene.h"
#include <Engine/CollisionMgr.h>

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

        this->LoadContent();
        this->OnStart();
    }

    void Scene::Close()
    {
        this->OnEnd();

        delete entities_;
        entities_ = nullptr;

        delete camera_;
        camera_ = nullptr;

        delete collisions_;
        collisions_ = nullptr;
    }

    void Scene::PostUpdate()
    {
        entities_->PostUpdate();
        collisions_->ProcessCollisions();
    }
}