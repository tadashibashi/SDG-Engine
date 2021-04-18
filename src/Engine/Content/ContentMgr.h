/* =============================================================================
 * ContentMgr
 * Manages loading and unloading game content.
 * Currently supports images, sprites, sprite atlasses.
 * ===========================================================================*/
#pragma once
#include <string>
#include "Texture2D.h"
#include "Sprite.h"
#include "SpriteAtlas.h"

namespace SDG
{
    class TextureCache;
    class AtlasCache;

    class ContentMgr {
    public:
        ContentMgr();
        ~ContentMgr();

        // Loads an image file into a Texture2D.
        // Currently supports loading image formats { pvr.ccz | png } in RGBA
        Texture2D LoadTexture(std::string filepath, unsigned filterType = GL_LINEAR);
        void UnloadTexture(std::string filepath);

        // Gets a Sprite from the last loaded atlas. Use ChangeCurrentAtlas to load from another.
        Sprite *GetSprite(std::string key);

        // Loads a SpriteAtlas.
        SpriteAtlas *LoadAtlas(const std::string& atlasPath);

        // Changes the current atlas GetSprite will retrieve from
        // until the next call to this function is made or a call to
        // LoadAtlas is successfully made.
        void ChangeCurrentAtlas(const std::string& atlasImagePath);
        void UnloadAll();
    private:
        TextureCache *textures_;
        AtlasCache *atlasses_;
        // Strips a preceeding "." or "./" from filepath to keep it consistent.
        static void FormatString(std::string &str);
    };

}



