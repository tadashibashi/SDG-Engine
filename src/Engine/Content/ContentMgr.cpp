/* =============================================================================
 * ContentMgr
 * 
 * 
 * ===========================================================================*/
#include "ContentMgr.h"
#include <Engine/Content/TextureCache.h>
#include "AtlasCache.h"

namespace SDG
{
    ContentMgr::ContentMgr():
            textures_(new TextureCache), atlasses_(new AtlasCache(*this))
    {
    }

    ContentMgr::~ContentMgr()
    {
        delete atlasses_; // atlasses must be deleted before textures
        delete textures_;
    }

    Texture2D ContentMgr::LoadTexture(std::string filepath, unsigned filterType)
    {
        FormatString(filepath);
        return textures_->GetTexture(filepath, filterType);
    }

    Sprite *ContentMgr::GetSprite(std::string key)
    {
        FormatString(key);
        return atlasses_->GetSprite(key);
    }

    void ContentMgr::FormatString(std::string &str)
    {
        if (str.empty())
            return;
        if (str.starts_with("./"))
            str.erase(0, 2);
        else if (str.starts_with("/"))
            str.erase(0, 1);
    }

    SpriteAtlas *ContentMgr::LoadAtlas(const std::string& imageFilePath)
    {
        return atlasses_->Load(imageFilePath);
    }

    void ContentMgr::UnloadTexture(std::string filepath)
    {
        FormatString(filepath);
        textures_->UnloadTexture(filepath);
    }

    void ContentMgr::ChangeCurrentAtlas(const std::string& atlasImagePath)
    {
        atlasses_->ChangeCurrentAtlas(atlasImagePath);
    }

    void ContentMgr::UnloadAll()
    {
        delete atlasses_;
        delete textures_;
        atlasses_ = new AtlasCache(*this);
        textures_ = new TextureCache;
    }

}