/* =============================================================================
 * SpriteBatch
 * 
 * 
 * ===========================================================================*/
#include <algorithm>
#include <Engine/Math/Math.h>
#include "SpriteBatch.h"
#include <Engine/Log.h>

#pragma warning(suppress : 4103)
#include <glm/gtc/matrix_transform.hpp>

namespace SDG
{
    SpriteBatch::SpriteBatch(GraphicsDevice *graphicsDevice):
        sortOrder(SortOrder::Texture), graphics(graphicsDevice), shader(graphicsDevice->GetDefaultShader())
    {
        CreateVertexArray();

        // Generate one pixel
        GLuint texid;
        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        GLubyte pix[] = {255u, 255u, 255u, 255u};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
        pixel = Texture2D(texid, 1, 1);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    SpriteBatch::~SpriteBatch()
    {
        // Delete pixel texture
        GLuint tex = pixel.GetID();
        glDeleteTextures(1, &tex);
    }

    void SpriteBatch::Begin(const glm::mat4 &matrix, SortOrder sort_order)
    {
        sortOrder = sort_order;
        batches.clear();
        glyphs.clear();

        auto mat = glm::scale(matrix, glm::vec3(1.f/scale.x, 1.f/scale.y, 1.f));

        // Set the transformation matrix uniform var
        GLint matLocation = shader->GetUniformLocation("P");
        glUniformMatrix4fv(matLocation, 1, GL_FALSE, &mat[0][0]);

        // Reset the texture uniform var.
        GLint texLocation = shader->GetUniformLocation("u_texture");
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(texLocation, 0);
    }

    void SpriteBatch::End()
    {
        SortGlyphs();
        CreateBatches();
    }

    void SpriteBatch::RenderBatches()
    {
        GLint location = shader->GetUniformLocation("u_size");
//        glBindVertexArray(vao);
        CreateVertexArray();
        for (auto &batch : batches) {
            glUniform2iv(location, 1, (GLint *)batch.texture_.SizeData());
            glBindTexture(GL_TEXTURE_2D, batch.texture_.GetID());
            glDrawArrays(GL_TRIANGLES, batch.offset_, batch.vertex_count_);
        }

        DisableVertexArray();
//        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void SpriteBatch::DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color, float depth)
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

    void SpriteBatch::DrawTexture(Texture2D texture, FRectangle dest, FRectangle source, Color color,
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
        anchor.x  *= -scale.x;

        Vector2 tl(-anchor.x, -anchor.y);
        Vector2 bl(-anchor.x, -dest.h-anchor.y);
        Vector2 br(dest.w-anchor.x, -dest.h-anchor.y);
        Vector2 tr(dest.w-anchor.x, -anchor.y);

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

    void SpriteBatch::CreateVertexArray()
    {
        // Generate objects if non-existent
//        if (vao == 0)
//        {
//            glGenVertexArrays(1, &vao);
//        }

        if (vbo == 0)
        {
            glGenBuffers(1, &vbo);
        }

        // Bind objects for sending info
//        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void *)offsetof(Vertex, position)); // tells opengl where data is
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), 
            (void *)offsetof(Vertex, color));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
            (void *)offsetof(Vertex, uv));
    }

    void SpriteBatch::CreateBatches()
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
            }
            else {
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        // upload the data
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void SpriteBatch::SortGlyphs()
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

    void SpriteBatch::DrawRectangle(FRectangle dest, Color color, float depth)
    {
        DrawTexture(GetPixel(), dest, FRectangle(0.f, 0.f, 1.f, 1.f), color, depth);
    }

    void SpriteBatch::DisableVertexArray()
    {
        // Unbind vao
//        glBindVertexArray(0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
}
