/* =============================================================================
 * TextureCache
 * 
 * 
 * ===========================================================================*/
#include "TextureCache.h"
#include <Engine/File/IO.h>
#include <Engine/Log.h>
#include <iostream>
#include <fstream>

namespace SDG
{
    TextureCache::TextureCache()
    {

    }

    TextureCache::~TextureCache()
    {
        if (!textures.empty())
        {
            UnloadAll();
        }
    }

    bool TextureCache::LoadPNG(const std::string &filepath, Texture2D *texture, unsigned filterType)
    {
        if (!texture)
        {
            SDG_ERR("Could not load PNG because texture parameter was a nullptr");
            return false;
        }

        std::vector<unsigned char> buf;
        unsigned long width, height;
        if (!IO::DecodePNG(filepath, &buf, &width, &height))
        {
            SDG_ERR("Failed to decode PNG file \"" + filepath + "\"");
            return false;
        }

        GLuint textureID = GenerateTexture(width, height, &buf, filterType);
        if (textureID == 0)
        {
            SDG_ERR("Failed to create GL texture.");
            return false;
        }

        // Load success: commit changes
        *texture = Texture2D(textureID,
             static_cast<int>(width), static_cast<int>(height));
        return true;
    }

    bool TextureCache::LoadPVRCCZ(const std::string &filepath, Texture2D *texture, unsigned filterType)
    {
        if (!texture)
        {
            SDG_ERR("Could not load PRVCCZ because texture parameter was a nullptr");
            return false;
        }

        std::vector<unsigned char> buf;
        unsigned long width, height;
        if (!IO::DecodePVRCCZ(filepath, &buf, &width, &height))
        {
            SDG_ERR("Failed to decode PVRCCZ file \"" + filepath + "\"");
            return false;
        }

        GLuint textureID = GenerateTexture(width, height, &buf, filterType);
        if (textureID == 0)
        {
            SDG_ERR("Failed to create GL texture.");
            return false;
        }

        // Load success: commit changes
        *texture = Texture2D(textureID,
            static_cast<int>(width), static_cast<int>(height));

        return true;
    }

    GLuint 
    TextureCache::GenerateTexture(unsigned long width, unsigned long height,
            std::vector<unsigned char> *buf, unsigned filterType)
    {
        if (buf->empty())
        {
            SDG_ERR("Could not generate texture: data buffer was empty.");
            return 0;
        }

        // Create OpenGL texture object
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // Pass texture the image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(width),
                     static_cast<GLsizei>(height), 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, buf->data());

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterType);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterType);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0); // unbind

        return textureID;
    }

    bool TextureCache::WriteToBinary(const std::string &filename, std::vector<unsigned char> *buf)
    {
        if (buf->empty())
        {
            SDG_ERR("Could not write " + filename + " to binary format "
                    "because the buffer was empty.");
            return false;
        }

        using std::ios;
        std::ofstream out(filename, ios::binary | ios::trunc);
        out.write((char *)buf->data(), buf->size());

        return true;
    }

    Texture2D TextureCache::GetTexture(const std::string &filepath, unsigned filterType)
    {
        auto it = textures.find(filepath);
        if (it == textures.end()) // Generate new Texture
        {
            Texture2D t;
            if (filepath.ends_with("pvr.ccz"))
            {
                LoadPVRCCZ(filepath, &t, filterType);
            }
            else if (filepath.ends_with(".png"))
            {
                LoadPNG(filepath, &t, filterType);
            }
            else
            {
                throw std::runtime_error("Could not load texture. The format "
                    "of file " + filepath + " is not supported.");
            }

            textures[filepath] = t;

            return t;
        }
        else                      // Load existing texture
        {
            return textures[filepath];
        }
    }

    void TextureCache::UnloadTexture(const Texture2D &texture)
    {
        for(auto it = textures.begin(), end = textures.end(); it != end; ++it)
        {
            if (it->second == texture)
            {
                textures.erase(it);
                GLuint id = texture.GetID();
                glDeleteTextures(1, &id);

                return;
            }
        }
        SDG_ERR("Could not unload texture because it was not found in cache.");
    }

    void TextureCache::UnloadTexture(const std::string &filepath)
    {
        auto it = textures.find(filepath);
        if (it != textures.end())
        {
            GLuint id = it->second.GetID();
            glDeleteTextures(1, &id);
            textures.erase(it);
        }
        else
        {
            SDG_ERR("Could not unload texture " + filepath + " because "
                    "it was not found in the TextureCache.");
        }
    }

    void TextureCache::UnloadAll()
    {
        for (auto &[filepath, texture] : textures)
        {
            GLuint id = texture.GetID();
            glDeleteTextures(1, &id);
        }

        textures.clear();
    }
}