/* =============================================================================
 * SpriteRenderer
 * 
 * 
 * ===========================================================================*/
#include "SpriteRenderer.h"
#include "Transform.h"
#include <cmath>
#include <iostream>
namespace SDG
{
    SpriteRenderer::SpriteRenderer():
            EntityComponent(true, true),
            imageIndex(0), imageSpeed(1.f), sprite(nullptr),
            scale(1.f, 1.f), position(), transform_(nullptr),
            color(255, 255, 255, 255), depth(0), rotation(0)
    {
    }

    // ========================================================================
    // Init
    // ========================================================================
    void SpriteRenderer::Init()
    {
        if (Owner()) {
            transform_ = Owner()->Get<Transform>();
            if (transform_)
            {
                position = transform_->GetPosition();
                scale = transform_->scale;
            }
            //transform_->OnPositionChange.AddListener()
        }
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
                    (float)sprite->GetLength());
        }
    }

    // ========================================================================
    // Draw
    // ========================================================================
    void SpriteRenderer::Draw()
    {
        SpriteBatch &spriteBatch = *GetSpriteBatch();
        if (transform_)
        {
            position = transform_->GetPosition();
            scale = transform_->scale;
        }

        if (sprite) {
            const Frame &frame = sprite->At((int) imageIndex);
            float rot = this->rotation;
            Vector2 offset;
            FRectangle pos;
            float w_scale = (1.f / (float) frame.texture.GetWidth());
            float h_scale = (1.f / (float) frame.texture.GetHeight());

            // Calculate the uv (normalized proportion within the subimage that this frame exists)
            FRectangle uv(w_scale * (float) frame.x,
                            h_scale * ((float) frame.texture.GetHeight() - (float) frame.h - (float) frame.y),
                            w_scale * (float) frame.w,
                            h_scale * (float) frame.h);

            if (frame.rotated)
            {
                rot += 90;

                offset = Vector2( ( -(float)frame.w - (float)frame.oy + (float)frame.oh * (frame.hasPivot ? frame.pivY: sprite->GetOffset().y) ) * scale.y,
                                  ( (float)frame.ox - (float)frame.ow * (frame.hasPivot ? frame.pivX: sprite->GetOffset().x) ) * scale.x);
                pos = FRectangle(position.x + offset.x,
                               position.y + offset.y,
                               (float) frame.w * scale.y,
                               (float) frame.h * scale.x);
            }
            else
            {
                offset = Vector2( ( (float)frame.ox - (float)frame.ow * (frame.hasPivot ? frame.pivX: sprite->GetOffset().x) ) * scale.x,
                                  ( (float)frame.oy - (float)frame.oh * (frame.hasPivot ? frame.pivY: sprite->GetOffset().y) ) * scale.y);
                pos = FRectangle(position.x + offset.x,
                                 position.y + offset.y,
                                 (float) frame.w * scale.x,
                                 (float) frame.h * scale.y);
            }








            //SDG_LOG("{0}", offset.ToString().c_str());
            if (rot != 0)
                spriteBatch.DrawTexture(frame.texture, pos, uv, color, depth, offset, rot);
            else
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
