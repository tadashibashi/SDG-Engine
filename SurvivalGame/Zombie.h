/* =============================================================================
 * Zombie
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Components/EntityComponent.h>
#include <Engine/Components/SpriteRenderer.h>
#include <Engine/Components/Transform.h>
#include <Engine/Components/Collider.h>
#include <Engine/Math/Rand.h>
#include <Engine/Math/Math.h>

namespace SDG
{
    class Zombie : public EntityComponent
    {
    public:
        Zombie(): EntityComponent(true, false),
                   player_(nullptr)
        {}

        void Init() override
        {
            GetComponent<SpriteRenderer>()->SetSpriteByKey("rob-idle");
            player_ = GetSceneMgr()->CurrentScene()->GetFirstEntityWithTag("Player");
            GetComponent<Collider2D>()->SetCallback(OnCollision);
        }

        static void OnCollision(Entity *thiz, Entity *other)
        {
            if (other->GetTag() == "Zombie")
            {
                Vector2 &pos = thiz->Components()->Get<Transform>()->position;
                Vector2 &otherPos = other->Components()->Get<Transform>()->position;
                if (otherPos.x > pos.x) {pos += Vector2(-1, 0);} else {pos += Vector2(1, 0);}
                if (otherPos.y > pos.y) {pos += Vector2(0, -1);} else {pos += Vector2(0, 1);}
            }
            else if (other->GetTag() == "Bullet")
            {
                GetScene()->DestroyEntity(*thiz);
                GetScene()->DestroyEntity(*other);
            }
        }

        void Update() override
        {
            auto mytf = GetComponent<Transform>();
            if (Rand::Next() < .5)
            {

                if (player_)
                {
                    auto tf = player_->Components()->Get<Transform>();

                    auto res = (tf->GetPosition() - mytf->GetPosition()).Normalize();
                    direction = Math::PointDirection(mytf->position.x, mytf->position.y, tf->position.x, tf->position.y);
                    mytf->position += res;
                }
                else
                {
                    player_ = GetSceneMgr()->CurrentScene()->GetFirstEntityWithTag("Player");
                }
            }
            auto spr = this->GetComponent<SpriteRenderer>();

            if (direction > 90 && direction < 270)
            {
                spr->rotation = Math::Mod<float>((direction + 180), 360);
                targetScalex = -1;
            }
            else
            {
                spr->rotation = direction;
                targetScalex = 1;
            }

            mytf->scale = Math::Lerp(mytf->scale, Vector2(targetScalex, 1), .1f);

        }

    private:
        Entity *player_;
        float rotation{};
        float direction;
        float targetScalex{};
    };
}