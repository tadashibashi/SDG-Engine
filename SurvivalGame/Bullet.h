/* =============================================================================
 * Bullet
 * 
 * 
 * ===========================================================================*/
#pragma once

#include <Engine/Components/EntityComponent.h>
#include <Engine/Components/Transform.h>
#include <Engine/Components/Body.h>
#include <Engine/Scenes/Scene.h>
#include <Engine/Components/Collider.h>

namespace SDG
{
    class Bullet: public SDG::EntityComponent
    {
    public:
        Bullet(): EntityComponent(true, true)
        {

        }

        void Init() override
        {

        }

        void Update() override
        {
            life -= (int)GetTime()->DeltaTicks();
            if (life <= 0)
                GetScene()->DestroyEntity(*GetEntity());
        }

        void Draw() override
        {
            Vector2 position = GetComponent<Transform>()->GetPosition();
            auto size = GetComponent<Body>()->size;
            GetSpriteBatch()->DrawRectangle(FRectangle(position.x, position.y, size.w, size.h),
                                            Color(255, 0, 0, 255), -1000.f);
        }

    private:
        int life = 3000;
    };

    void MakeBullet(Entity &entity)
    {
        entity.Components()->Add<Transform>(0, 0, 1.f, 1.f);
        auto &body = entity.Components()->Add<Body>();
        body.SetUseSpriteMask(false);
        body.size = Vector2(4.f, 4.f);
        entity.Components()->Add<Collider2D>();
        entity.Components()->Add<Bullet>();
        entity.SetTag("Bullet");
    }
}