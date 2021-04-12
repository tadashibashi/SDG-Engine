/* =============================================================================
 * TextureCache
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Content/Texture2D.h>
#include <string>
#include <vector>
#include <Engine/GraphicsLibrary.h>
#include <map>

namespace SDG
{
    // Manages loading and storing Texture2D
    class  TextureCache {
    public:
        TextureCache();
        ~TextureCache();

        Texture2D GetTexture(const std::string &filepath, unsigned filterType = GL_LINEAR);
        // Unloads a texture from the cache
        void UnloadTexture(const std::string &filepath);
        // This way is slower, prefer to use filepath overload.
        void UnloadTexture(const Texture2D &texture);

        void UnloadAll();
    private:
        static bool LoadPNG(const std::string &filepath, Texture2D *texture, unsigned filterType = GL_LINEAR);
        static bool LoadPVRCCZ(const std::string &filepath, Texture2D *texture, unsigned filterType = GL_LINEAR);
        static GLuint GenerateTexture(unsigned long width, unsigned long height,
                                    std::vector<unsigned char> *buf, unsigned filterType);

        // Not sure what to use this for yet...
        static bool WriteToBinary(const std::string &filename, std::vector<unsigned char> *buf);

        std::map<std::string, Texture2D> textures;
    };
}



