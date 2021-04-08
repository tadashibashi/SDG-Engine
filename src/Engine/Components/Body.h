/* =============================================================================
 * RigidBody2D
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Math/Vector2.h>
#include "EntityComponent.h"
#include "Transform.h"

namespace SDG
{
    class SpriteRenderer;

    class Body : public Component
    {
    public:
        Body(): Component(true, true),
                velocity       (0, 0),
                show           (false),
                tf             (nullptr),
                spr            (nullptr),
                size           (16, 16)
        {}

        // Whether or not to show debug rect
        bool show;
        // Body size in pixels
        Vector2 size;
        Vector2 velocity;

        void Init() override;

        void Update() override;

        void Draw() override;

        void SetUseSpriteMask(bool use)
        {
            useSpriteMask = use;
        }
        
        [[nodiscard]]
        bool GetUseSpriteMask() const { return useSpriteMask; }

        [[nodiscard]]
        Rectangle GetBounds() const
        {
            return Rectangle((int)position.x, (int)position.y, (int)size.w, (int)size.h);
        }
    private:
        Transform *tf;
        SpriteRenderer *spr;
        bool useSpriteMask{true};

        Vector2 position;
    };
}
