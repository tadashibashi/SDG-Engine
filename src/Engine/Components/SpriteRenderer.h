/* =============================================================================
 * SpriteRenderer
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include "EntityComponent.h"
#include <Engine/Math/Vector2.h>
#include <Engine/Graphics/Color.h>

namespace SDG
{
    class Transform;
    class Sprite;

    class SDG_API SpriteRenderer: public EntityComponent {
    public:
        SpriteRenderer();

        void Init() override;
        void Update() override;
        void Draw() override;

        void SetSpriteByKey(const std::string &key);
        [[nodiscard]] const Sprite *GetSprite() const { return sprite; }
        [[nodiscard]] Frame GetCurrentFrame() const
        {
            // Returns a dummy frame if Sprite is null.
            return (sprite)
            ? sprite->At((int)imageIndex % (int)sprite->GetLength()) : Frame();
        }

        float depth;
        float imageIndex;
        float imageSpeed;
        Sprite *sprite;
        Vector2 position;
        Vector2 scale;
        Color color;
    private:
        Transform *transform_;
    };
}



