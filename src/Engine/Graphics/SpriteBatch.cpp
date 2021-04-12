/* =============================================================================
 * SpriteBatch_GL
 * 
 * 
 * ===========================================================================*/
#include "SpriteBatch.h"
#include <algorithm>
#include <Engine/Math/Math.h>

#include <Engine/Log.h>

#pragma warning(suppress : 4103)
#include <glm/gtc/matrix_transform.hpp>

void SDG::SpriteBatch::DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color, float depth)
{
    Glyph glyph;
    glyph.texture = texture;
    glyph.depth = depth;

    dest.x = std::round(dest.x);
    dest.w = std::round(dest.w);
    dest.y = std::round(dest.y);
    dest.h = std::round(dest.h);

    dest.x *= scale.x;
    dest.y *= scale.y;
    dest.w *= scale.x;
    dest.h *= scale.y;
    dest.y = -dest.y;

    glyph.topleft.color = color;
    glyph.topleft.position = Vector2(dest.x, dest.y);
    glyph.topleft.uv = Vector2(source.x, source.y + source.h);

    glyph.bottomleft.color = color;
    glyph.bottomleft.position = Vector2(dest.x, dest.y - dest.h);
    glyph.bottomleft.uv = Vector2(source.x, source.y);

    glyph.bottomright.color = color;
    glyph.bottomright.position = Vector2(dest.x + dest.w, dest.y - dest.h);
    glyph.bottomright.uv = Vector2(source.x + source.w, source.y);

    glyph.topright.color = color;
    glyph.topright.position = Vector2(dest.x + dest.w, dest.y);
    glyph.topright.uv = Vector2(source.x + source.w, source.y + source.h);

    glyphs.emplace_back(glyph);
}

void SDG::SpriteBatch::DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color,
                                 float depth, Vector2 anchor, float rotation)
{


    Glyph glyph;
    glyph.texture = texture;
    glyph.depth = depth;

    dest.x *= scale.x;
    dest.y *= scale.y;
    dest.w *= scale.x;
    dest.h *= scale.y;
    anchor.y *= scale.y;
    anchor.x *= -scale.x;

    Vector2 tl(-anchor.x, -anchor.y);
    Vector2 bl(-anchor.x, -dest.h - anchor.y);
    Vector2 br(dest.w - anchor.x, -dest.h - anchor.y);
    Vector2 tr(dest.w - anchor.x, -anchor.y);

    float angle = Math::DegToRad(rotation);
    //float angle = std::acos(right.x * rotation + right.y * rotation);

    tl = Vector2::Rotate(tl, angle) + anchor;
    bl = Vector2::Rotate(bl, angle) + anchor;
    br = Vector2::Rotate(br, angle) + anchor;
    tr = Vector2::Rotate(tr, angle) + anchor;


    dest.y = -dest.y;

    dest.x = std::round(dest.x);
    dest.w = std::round(dest.w);
    dest.y = std::round(dest.y);
    dest.h = std::round(dest.h);

    glyph.topleft.color = color;
    glyph.topleft.position = Vector2(dest.x + tl.x, dest.y + tl.y);
    glyph.topleft.uv = Vector2(source.x, source.y + source.h);

    glyph.bottomleft.color = color;
    glyph.bottomleft.position = Vector2(dest.x + bl.x, dest.y + bl.y);
    glyph.bottomleft.uv = Vector2(source.x, source.y);

    glyph.bottomright.color = color;
    glyph.bottomright.position = Vector2(dest.x + br.x, dest.y + br.y);
    glyph.bottomright.uv = Vector2(source.x + source.w, source.y);

    glyph.topright.color = color;
    glyph.topright.position = Vector2(dest.x + tr.x, dest.y + tr.y);
    glyph.topright.uv = Vector2(source.x + source.w, source.y + source.h);

    glyphs.emplace_back(glyph);
}

void SDG::SpriteBatch::DrawRectangle(FRectangle dest, Color color, float depth)
{
    DrawTexture(GetPixel(), dest, FRectangle(0.f, 0.f, 1.f, 1.f), color, depth);
}

void SDG::SpriteBatch::DrawRectangle(FRectangle dest, Color color, float depth, Vector2 anchor, float rotation)
{
    DrawTexture(GetPixel(), dest, FRectangle(0.f, 0.f, 1.f, 1.f), color, depth, anchor, rotation);
}

SDG::SpriteBatch::SpriteBatch(SDG::GraphicsDevice *graphicsDevice) : graphics(graphicsDevice), glyphs{}, batches{},
    pixel(graphicsDevice->GenerateTexture(1, 1, {{255, 255, 255, 255}}))
{
}
