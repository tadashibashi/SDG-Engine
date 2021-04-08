#include <Engine/Log.h>
#include "AtlasCache.h"
#include <cassert>

namespace SDG
{
    // ========================================================================
    // Constructors / Destructors
    // ========================================================================
    AtlasCache::AtlasCache(ContentMgr &contentMgr):
        content_(contentMgr), current_(nullptr)
    {
    }

    AtlasCache::~AtlasCache()
    {
        for (auto &[k, v]: cache_)
        {
            v->Close();
            delete v;
        }
        cache_.clear();
    }

    // ========================================================================
    // Load
    // ========================================================================
    SpriteAtlas *AtlasCache::Load(const std::string &imagePath)
    {
        if (cache_.contains(imagePath))     // Cache already contains this path return it.
        {
            ChangeCurrentAtlas(imagePath);
            return cache_[imagePath];
        }
        else                                // Cache does not contain this path, load it.
        {
            auto *atlas = new SpriteAtlas(content_);

            std::string atlasPath = imagePath.substr(0, imagePath.find_first_of('.')) + ".lua";

            std::string spriteConfigPath = imagePath.substr(0, imagePath.find_first_of('.')) + "_sprites.lua";

            SDG_CORE_LOG("Loading atlas:\nImagePath: {0}\nAtlasPath: {1}\nSpriteConfig: {2}", imagePath, atlasPath,
                spriteConfigPath);

            // Check for errors
            if (!atlas->Load(atlasPath, imagePath))
            {
                delete atlas;
                SDG_ERR("Failed to load atlas with image path:", imagePath);
                return nullptr;
            }

            if (!atlas->LoadSprites(spriteConfigPath))
            {
                delete atlas;
                SDG_ERR("Failed to load sprites from atlas with image path:", imagePath);
                return nullptr;
            }

            // No errors, commit changes.
            cache_[imagePath] = atlas;
            ChangeCurrentAtlas(imagePath);
            return atlas;
        }
    }

    // ========================================================================
    // Unload Atlas
    // ========================================================================
    void AtlasCache::Unload(const std::string& key)
    {
        if (cache_.contains(key))
        {
            cache_[key]->Close();
            delete cache_[key];
            cache_.erase(key);
        }
    }

    void AtlasCache::Unload(SpriteAtlas *atlas)
    {
        Unload(atlas->imagePath_);
    }

    // ========================================================================
    // ChangeCurrentAtlas
    // ========================================================================
    void AtlasCache::ChangeCurrentAtlas(const std::string &imagePath)
    {
        if (cache_.contains(imagePath))
        {
            current_ = cache_[imagePath];
        }
        else
        {
            SDG_CORE_ERR("Attempted to change AtlasCache's current atlas with "
                "image path: {1}, but it could not be found within cache", imagePath);
        }
    }

    // ========================================================================
    // GetSprite
    // ========================================================================
    Sprite *AtlasCache::GetSprite(const std::string &key)
    {
        assert(current_ != nullptr);
        return current_->GetSprite(key);
    }
}