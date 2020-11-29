/* =============================================================================
 * Collider
 * 
 * 
 * ===========================================================================*/
#pragma once
#include "EntityComponent.h"
#include "Body.h"
#include <Engine/Scenes/Scene.h>

#include <unordered_set>
#include <functional>
#include <utility>

namespace SDG
{
    class Collider2D : public EntityComponent
    {
        friend class CollisionMgr;
    public:
        Collider2D(): EntityComponent(true, true), body_(nullptr)
        {
        }

    public:
        void Init() override;
        void Close() override;
        void Update() override;
        void Draw() override;
        bool CheckCollision(Collider2D *other);
        void SetCallback(std::function<void(Entity *thiz, Entity *other)> callback)
        {
            callback_ = std::move(callback);
        }
    private:
        std::unordered_set<Collider2D *> collided_;
        Body *body_;
        std::function<void(Entity *thiz, Entity *other)> callback_;
    };
}