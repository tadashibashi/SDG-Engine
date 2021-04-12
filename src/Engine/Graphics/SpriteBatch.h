/* =============================================================================
 * SpriteBatch
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Content/Shader.h>
#include "Vertex.h"
#include "GraphicsDevice.h"
#include <Engine/Content/Texture2D.h>
#include <glm/mat4x4.hpp>
#include <Engine/Math/Rectangle.h>
#include <vector>
#include <Engine/Core.h>

namespace SDG
{
    enum class SortOrder {
        None,
        FrontToBack,
        BackToFront,
        Texture
    };

    // TODO: Try to move all graphics library-specific code into GraphicsDevice, which SpriteBatch could then call
    // generically without needing base classes.
    class SpriteBatch {
    protected:
        // Data to render a sprite.
        struct Glyph {
            Glyph() : texture(), depth(0), topleft(), bottomleft(), topright(),
                      bottomright() { }
            Vertex topleft, bottomleft, topright, bottomright;
            Texture2D texture;
            float depth;
        };

        // Data to render a batch of glyphs.
        class RenderBatch {
        public:
            RenderBatch(uint offset, uint vertex_count, Texture2D texture)
                    : offset_(offset), vertex_count_(vertex_count), texture_(texture)
            {}
            uint offset_;
            uint vertex_count_;
            Texture2D texture_;
        };
    public:
        explicit SpriteBatch(GraphicsDevice *graphicsDevice);
        virtual ~SpriteBatch() = default;

        virtual void Begin(const glm::mat4 &matrix, SortOrder sort_order) = 0;
        virtual void End() = 0;

        void DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color, float depth);
        void DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color, float depth, Vector2 anchor, float rotation);
        void DrawRectangle(FRectangle dest, Color color, float depth);
        void DrawRectangle(FRectangle dest, Color color, float depth, Vector2 anchor, float rotation);

        virtual void RenderBatches() = 0;

        Texture2D GetPixel() const { return pixel; }
    protected:
        std::vector<Glyph> glyphs;
        std::vector<RenderBatch> batches;
        GraphicsDevice *graphics;

        Vector2 scale{10.f, 10.f};
        Texture2D pixel;
    };
}



