/* =============================================================================
 * AtlasCache
 * Manager storage of SpriteAtlas objects. Owned by the main ContentMgr.
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <map>
#include <string>
#include <Engine/Content/SpriteAtlas.h>

namespace SDG
{
    class SDG_API AtlasCache {
    public:
        explicit AtlasCache(ContentMgr &contentMgr);
        ~AtlasCache();

        SpriteAtlas *Load(const std::string &imagePath);

        // Gets a Sprite from the current SpriteAtlas.
        // Current SpriteAtlas is determined by either the latest successful call
        // to GetAtlas or ChangeCurrentAtlas.
        Sprite *GetSprite(const std::string &key);

        // Will unload the atlas if it is in this cache.
        void Unload(const std::string &key);
        // Will unload the atlas if it is in this cache.
        void Unload(SpriteAtlas *atlas);
        // Changes the current atlas GetSprite will get from.
        // A successful call to GetAtlas will also change the current atlas.
        void ChangeCurrentAtlas(const std::string &imagePath);
    private:
        std::map<std::string, SpriteAtlas *> cache_;
        SpriteAtlas *current_;
        ContentMgr &content_;
    };

}



