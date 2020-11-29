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
    // Data to render a sprite.
    struct SDG_API Glyph {
        Glyph() : texture(), depth(0), topleft(), bottomleft(), topright(),
            bottomright() { }
        Texture2D texture;
        float depth;
        Vertex topleft, bottomleft, topright, bottomright;
    };

    enum class SortOrder {
        None,
        FrontToBack,
        BackToFront,
        Texture
    };

    class SDG_API RenderBatch {
    public:
        RenderBatch(GLuint offset, GLuint vertex_count, Texture2D texture)
                : offset_(offset), vertex_count_(vertex_count), texture_(texture)
        {}
        GLuint offset_;
        GLuint vertex_count_;
        Texture2D texture_;
    };

    class SDG_API SpriteBatch {
    public:
        explicit SpriteBatch(GraphicsDevice *graphicsDevice);
        ~SpriteBatch();

        void Begin(const glm::mat4 &matrix, SortOrder sort_order = SortOrder::Texture);
        // Does post-processing like depth-sorting.
        void End();
        void DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color, float depth = 0);
        void DrawRectangle(FRectangle dest, Color color, float depth = 0);
        [[nodiscard]] Texture2D GetPixel() const { return pixel; }
        void RenderBatches();
    private:
        GraphicsDevice *graphics;
        Shader *shader;
        SortOrder sortOrder;
        std::vector<Glyph> glyphs;
        std::vector<RenderBatch> batches;
        GLuint vbo{0}, vao{0};
        Vector2 scale = Vector2(10.f, 10.f);

        Texture2D pixel;

        void CreateBatches();

        void SortGlyphs();
        void CreateVertexArray();
        void DisableVertexArray();
    };
}



