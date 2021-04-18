#include <Engine/Log.h>
#include "AtlasCache.h"
#include <cassert>
#include <tinyxml2/tinyxml2.h>

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
    SpriteAtlas *AtlasCache::Load(const std::string &atlasPath)
    {
        if (cache_.contains(atlasPath))     // Cache already contains this path return it.
        {
            ChangeCurrentAtlas(atlasPath);
            return cache_[atlasPath];
        }
        else                                // Cache does not contain this path, load it.
        {
            auto *atlas = new SpriteAtlas(content_);

            std::string imagePath, spriteConfigPath;
            if (atlasPath.ends_with(".lua")) // assume .png of the same name
            {
                std::string stub = atlasPath.substr(0, atlasPath.find_first_of('.'));
                imagePath = stub + ".png";
                spriteConfigPath = stub + "_sprites.lua";
            }
            else if (atlasPath.ends_with(".xml"))
            {
                std::string stub = atlasPath.substr(0, atlasPath.find_first_of('.'));

                tinyxml2::XMLDocument doc;
                tinyxml2::XMLError result = doc.LoadFile(atlasPath.c_str());
                if (result != tinyxml2::XML_SUCCESS)
                {
                    delete atlas;
                    SDG_CORE_ERR("AtlasCache::Load failed to open xml file \"{0}\"", atlasPath);
                    return nullptr;
                }

                tinyxml2::XMLElement *atlasElement = doc.FirstChildElement("TextureAtlas");
                if (!atlasElement)
                {
                    delete atlas;
                    SDG_CORE_ERR("AtlasCache::Load failed to get the TextureAtlas element in file \"{0}\". "
                                 "This is most likely an invalid TexturePacker generic xml file.", atlasPath);
                    return nullptr;
                }

                auto *imagePathAttribute = atlasElement->FindAttribute("imagePath");
                if (!imagePathAttribute)
                {
                    delete atlas;
                    SDG_CORE_ERR("SDG::AtlasCache::Load failed to get the TextureAtlas element attribute \"imagePath\" "
                                 "in file {0}. This is most likely an invalid TexturePacker generic xml file.",
                                 atlasPath);
                    return nullptr;
                }
                imagePath = stub.substr(0, stub.find_last_of('/')) + "/" + imagePathAttribute->Value();
                spriteConfigPath = stub + "_sprites.xml";
            }
            else
            {
                SDG_CORE_ERR("SDG::AtlasCache::Load failed with atlas path \"{0}\": file extension not supported (must be .xml or .lua)", atlasPath);
                return nullptr;
            }

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
                SDG_ERR("Failed to load sprites from atlas with image path:", atlasPath);
                return nullptr;
            }

            // No errors, commit changes.
            cache_[atlasPath] = atlas;
            ChangeCurrentAtlas(atlasPath);
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