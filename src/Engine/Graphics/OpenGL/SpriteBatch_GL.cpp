/* =============================================================================
 * SpriteBatch_GL
 * 
 * 
 * ===========================================================================*/
#include "SpriteBatch_GL.h"
#include <Engine/Content/Shader.h>
#include <Engine/Log.h>
#include <Engine/Math/Math.h>

#pragma warning(suppress : 4103)
#include <glm/gtc/matrix_transform.hpp>

namespace SDG
{
    SpriteBatch_GL::SpriteBatch_GL(GraphicsDevice *graphicsDevice) : SpriteBatch(graphicsDevice),
            sortOrder(SortOrder::Texture), shader(graphicsDevice->GetDefaultShader())
    {
        CreateVertexArray();
    }

    SpriteBatch_GL::~SpriteBatch_GL()
    {
        // Delete pixel texture
        GLuint tex = pixel.GetID();
        glDeleteTextures(1, &tex);
    }

    void SpriteBatch_GL::Begin(const glm::mat4 &matrix, SortOrder sort_order)
    {
        sortOrder = sort_order;
        batches.clear();
        glyphs.clear();

        auto mat = glm::scale(matrix, glm::vec3(1.f / scale.x, 1.f / scale.y, 1.f));

        // Set the transformation matrix uniform var
        GLint matLocation = shader->GetUniformLocation("P");
        glUniformMatrix4fv(matLocation, 1, GL_FALSE, &mat[0][0]);

        // Reset the texture uniform var.
        GLint texLocation = shader->GetUniformLocation("u_texture");
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(texLocation, 0);
    }

    void SpriteBatch_GL::End()
    {
        SortGlyphs();
        CreateBatches();
    }

    void SpriteBatch_GL::RenderBatches()
    {
        GLint location = shader->GetUniformLocation("u_size");
//        glBindVertexArray(vao);
        CreateVertexArray();
        for (auto &batch : batches) {
            glUniform2iv(location, 1, (GLint *) batch.texture_.SizeData());
            glBindTexture(GL_TEXTURE_2D, batch.texture_.GetID());
            glDrawArrays(GL_TRIANGLES, (GLint) batch.offset_, (GLint) batch.vertex_count_);
        }
        DisableVertexArray();
//        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void SpriteBatch_GL::CreateVertexArray()
    {
        // Generate objects if non-existent
//        if (vao == 0) { glGenVertexArrays(1, &vao); }

        if (vbo == 0) { glGenBuffers(1, &vbo); }

        // Bind objects for sending info
//        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, position)); // tells opengl where data is
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex),
                              (void *) offsetof(Vertex, color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) offsetof(Vertex, uv));
    }

    void SpriteBatch_GL::CreateBatches()
    {
        if (glyphs.empty())
            return;
        std::vector<Vertex> vertices;
        vertices.resize(glyphs.size() * 6);

        int offset = 0;
        int current_vertex = 0;
        batches.emplace_back(offset, 6, glyphs[0].texture);
        vertices[current_vertex++] = glyphs[0].topleft;
        vertices[current_vertex++] = glyphs[0].bottomleft;
        vertices[current_vertex++] = glyphs[0].bottomright;
        vertices[current_vertex++] = glyphs[0].bottomright;
        vertices[current_vertex++] = glyphs[0].topright;
        vertices[current_vertex++] = glyphs[0].topleft;
        offset += 6;

        size_t glyph_size = glyphs.size();
        for (size_t cg = 1; cg < glyph_size; ++cg) {
            if (glyphs[cg].texture != glyphs[cg - 1].texture) {
                batches.emplace_back(offset, 6, glyphs[cg].texture);
            } else {
                batches.back().vertex_count_ += 6;
            }
            vertices[current_vertex++] = glyphs[cg].topleft;
            vertices[current_vertex++] = glyphs[cg].bottomleft;
            vertices[current_vertex++] = glyphs[cg].bottomright;
            vertices[current_vertex++] = glyphs[cg].bottomright;
            vertices[current_vertex++] = glyphs[cg].topright;
            vertices[current_vertex++] = glyphs[cg].topleft;
            offset += 6;
        }

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        // orphan the bufer
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        // upload the data
        glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr) vertices.size() * sizeof(Vertex), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SpriteBatch_GL::SortGlyphs()
    {
        switch (sortOrder) {
            case SortOrder::BackToFront:
                std::stable_sort(glyphs.begin(), glyphs.end(),
                                 [](const Glyph &a, const Glyph &b) {
                                     return a.depth > b.depth;
                                 });
                break;
            case SortOrder::FrontToBack:
                std::stable_sort(glyphs.begin(), glyphs.end(),
                                 [](const Glyph &a, const Glyph &b) {
                                     return a.depth < b.depth;
                                 });
                break;
            case SortOrder::Texture:
                std::stable_sort(glyphs.begin(), glyphs.end(),
                                 [](const Glyph &a, const Glyph &b) {
                                     return a.texture.GetID() < b.texture.GetID();
                                 });
                break;
            case SortOrder::None:
                break;
        }
    }

    void SpriteBatch_GL::DisableVertexArray()
    {
        // Unbind vao
//        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
}
