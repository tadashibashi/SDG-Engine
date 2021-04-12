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

        // Color is #df7126
        void Draw() override
        {
            Vector2 position = GetComponent<Transform>()->GetPosition();
            auto size = GetComponent<Body>()->size * GetScene()->GetCamera()->GetScale();
            GetSpriteBatch()->DrawRectangle(FRectangle(position.x, position.y, size.w, size.h),
                                            Color(0xDF7126FF), -1000.f);
        }

        static void MakeBullet(Entity &entity);
    private:
        int life = 3000;
    };


}