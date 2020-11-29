/* =============================================================================
 * SpriteRenderer
 * 
 * 
 * ===========================================================================*/
#include "SpriteRenderer.h"
#include "Transform.h"
#include <cmath>
#include <Engine/Content/Sprite.h>

namespace SDG
{
    SpriteRenderer::SpriteRenderer():
            EntityComponent(true, true),
            imageIndex(0), imageSpeed(1.f), sprite(nullptr),
            scale(1.f, 1.f), position(), transform_(nullptr),
            color(255, 255, 255, 255), depth(0)
    {
    }

    // ========================================================================
    // Init
    // ========================================================================
    void SpriteRenderer::Init()
    {
        if (Owner()) transform_ = Owner()->Get<Transform>();
    }

    // ========================================================================
    // Update
    // ========================================================================
    void SpriteRenderer::Update()
    {
        if (sprite)
        {
            imageIndex = std::fmodf(
                    imageIndex + sprite->GetBaseSpeed() * (float)GetTime()->DeltaTicks() * 0.001f * imageSpeed,
                    sprite->GetLength());
        }

        if (transform_)
        {
            position = transform_->GetPosition();
            scale = transform_->scale;
        }
    }

    // ========================================================================
    // Draw
    // ========================================================================
    void SpriteRenderer::Draw()
    {
        SpriteBatch &spriteBatch = *GetSpriteBatch();

        if (sprite) {
            const Frame &frame = sprite->At((int) imageIndex);

            FRectangle pos(position.x + (((float) frame.ox - (float) frame.ow * sprite->GetOffset().x)) * scale.x,
                           position.y + (((float) frame.oy - (float) frame.oh * sprite->GetOffset().y)) * scale.y,
                           (float) frame.w * scale.x,
                           (float) frame.h * scale.y);
            float w_scale(1.f / (float) frame.texture.GetWidth());
            float h_scale(1.f / (float) frame.texture.GetHeight());

            FRectangle uv(w_scale * (float) frame.x,
                          h_scale * ((float) frame.texture.GetHeight() - (float) frame.h - (float) frame.y),
                          w_scale * (float) frame.w,
                          h_scale * (float) frame.h);
            spriteBatch.DrawTexture(frame.texture, pos, uv, color, depth);
        }
    }

    void SpriteRenderer::SetSpriteByKey(const std::string &key)
    {
        Sprite *spr = GetContent()->GetSprite(key);
        if (spr)
            sprite = spr;
    }
}
