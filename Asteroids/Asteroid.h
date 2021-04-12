/* =============================================================================
 * Asteroid
 *
 *
 * ===========================================================================*/
#pragma once

#include <Engine/Components/EntityComponent.h>
#include <Engine/Components/Transform.h>
#include <Engine/Components/Body.h>
#include <Engine/Scenes/Scene.h>
#include <Engine/Components/Collider.h>
#include <Engine/Components/SpriteRenderer.h>

using namespace SDG;

    class Asteroid: public SDG::EntityComponent
    {
    public:
        Asteroid(): EntityComponent(true, true)
        {

        }

        void Init() override
        {
            Collider2D *coll = this->GetComponent<Collider2D>();
            coll->SetCallback(Asteroid::OnCollision);
        }

        void Update() override
        {
            GetComponent<SpriteRenderer>()->rotation += 1;
            auto roomSz = GetScene()->GetCamera()->GetWorldBounds();
            auto tf = GetComponent<Transform>();

            while (tf->position.x > roomSz.w + 16) tf->position.x -= (roomSz.w + 32);
            while (tf->position.x < -16) tf->position.x += (roomSz.w + 32);
            while (tf->position.y > roomSz.h + 16) tf->position.y -= (roomSz.h + 32);
            while(tf->position.y < -16) tf->position.y += (roomSz.h + 32);
        }

        void Draw() override
        {
        }

        static void OnCollision(Entity *thiz, Entity *other)
        {
            if (other->GetTag() == "Bullet")
            {
                GetScene()->DestroyEntity(*other);

                auto spr = thiz->Components()->Get<SpriteRenderer>();
                auto ast = thiz->Components()->Get<Asteroid>();
                ast->SetHealth(ast->GetHealth() - 1);
                int health = ast->GetHealth();

                if (health <= 0)
                {
                    GetScene()->DestroyEntity(*thiz);
                }
                else
                {
                    Entity &nast = GetScene()->CreateEntity(MakeAsteroid);
                    nast.Components()->Get<Transform>()->SetPositionLocal(
                            thiz->Components()->Get<Transform>()->GetPosition());
                    nast.Components()->Get<Asteroid>()->SetHealth(health);
                    auto spr2 = nast.Components()->Get<SpriteRenderer>();
                    switch (health)
                    {
                        case 2:
                            spr->SetSpriteByKey("asteroid-med");
                            spr2->SetSpriteByKey("asteroid-med");
                            break;
                        case 1:
                            spr->SetSpriteByKey("asteroid-small");
                            spr2->SetSpriteByKey("asteroid-small");
                            break;
                    }
                }

            } // end if (other->GetTag() == "Bullet")

        }

        static void MakeAsteroid(Entity &entity)
        {
            entity.Components()->Add<Transform>(32.f, 32.f, 1.f, 1.f);
            auto &spr = entity.Components()->Add<SpriteRenderer>();
            spr.SetSpriteByKey("asteroid-large");
            auto &body = entity.Components()->Add<Body>();
            body.SetUseSpriteMask(true);
            body.size = Vector2(24.f, 24.f);
            body.velocity = Vector2(Rand::Next(), Rand::Next());
            body.show = true;

            entity.Components()->Add<Collider2D>();
            entity.Components()->Add<Asteroid>();
            entity.SetTag("Asteroid");
        }

        void SetHealth(int health) { this->health = health; }
        int GetHealth() const { return health; }

    private:
        int health = 3;
    };


