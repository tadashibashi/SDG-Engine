/* =============================================================================
 * SpriteBatch_GL
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Graphics/SpriteBatch.h>

namespace SDG
{
    class SpriteBatch_GL : public SpriteBatch {
    public:
        explicit SpriteBatch_GL(GraphicsDevice *graphicsDevice);
        ~SpriteBatch_GL() override;

        void Begin(const glm::mat4 &matrix, SortOrder sort_order) override;
        // Does post-processing like depth-sorting.
        void End() override;
        void RenderBatches() override;
    private:
        Shader *shader;
        SortOrder sortOrder;
        GLuint vbo{0}, vao{0};

        void CreateBatches();
        void SortGlyphs();
        void CreateVertexArray();
        static void DisableVertexArray();
    };
}




