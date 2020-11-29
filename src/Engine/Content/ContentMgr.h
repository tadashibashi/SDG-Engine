/* =============================================================================
 * ContentMgr
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>
#include "Texture2D.h"
#include "Sprite.h"
#include "SpriteAtlas.h"


namespace SDG
{
    class TextureCache;
    class AtlasCache;

    class SDG_API ContentMgr {
    public:
        ContentMgr();
        ~ContentMgr();

        // Currently supports pvr.ccz and png files formatted in RGBA.
        Texture2D LoadTexture(std::string filepath, unsigned filterType = GL_LINEAR);
        void UnloadTexture(std::string filepath);
        Sprite *GetSprite(std::string key);
        SpriteAtlas *LoadAtlas(std::string imageFilePath);

        // Changes the current atlas GetSprite will retrieve from
        // until the next call to this function is made or a call to
        // LoadAtlas is successfully made.
        void ChangeCurrentAtlas(const std::string& atlasImagePath);
        void UnloadAll();
    private:
        TextureCache *textures_;
        AtlasCache *atlasCache_;
        // Strips a preceeding "." or "./" from filepath to keep it consistent.
        static void FormatString(std::string &str);
    };

}



