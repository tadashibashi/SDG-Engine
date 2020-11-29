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

    class SDG_API Body : public Component
    {
    public:
        Body(): Component(true, true),
                velocity       (0, 0),
                speed          (0),
                show           (false),
                tf             (nullptr),
                spr            (nullptr),
                size           (16, 16)
        {}

        bool show;    // whether or not to show debug rect
        Vector2 size; // in pixels
        Vector2 velocity;  // in degrees
        float speed;  // pixels-per-second

        void Init() override;

        void Update() override;

        void Draw() override;

        void SetUseSpriteMask(bool use)
        {
            useSpriteMask = use;
        }
        
        [[nodiscard]] bool GetUseSpriteMask() const { return useSpriteMask; }

        [[nodiscard]] Rectangle GetBounds() const
        {
            return Rectangle(position.x, position.y, size.w, size.h);
        }
    private:
        Transform *tf;
        SpriteRenderer *spr;
        bool useSpriteMask{true};

        Vector2 position;
    };
}
