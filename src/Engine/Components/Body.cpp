/* =============================================================================
 * Body
 * 
 * 
 * ===========================================================================*/
#include "Body.h"
#include "SpriteRenderer.h"

namespace SDG
{
    void Body::Update()
    {
        float x = 0, y = 0;
        if (useSpriteMask && spr)
        {
            const Sprite *s;
            if ((s = spr->GetSprite()))
            {
                Rectangle maskRect = s->GetMask();
                size.w = (float)maskRect.w;
                size.h = (float)maskRect.h;

                x -= s->GetOffset().x * (float)spr->GetCurrentFrame().ow - maskRect.x;
                y -= s->GetOffset().y * (float)spr->GetCurrentFrame().oh - maskRect.y;
            }
        }

        // Velocity
        tf->position += this->velocity;
        position = Vector2(x, y) + tf->GetPosition();
    }

    void Body::Draw()
    {
        if (show)
        {
            FRectangle rect(position.x, position.y, size.w, size.h);
            GetSpriteBatch()->DrawRectangle(rect, Color(127, 255, 127, 80));
        }
    }

    void Body::Init()
    {
        tf = GetComponent<Transform>();
        spr = GetComponent<SpriteRenderer>();
        Update();
    }
}