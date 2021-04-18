#include "SpriteAtlas.h"

//#include <sol/sol.hpp>
#include <tinyxml2/tinyxml2.h>

#include <Engine/Content/ContentMgr.h>
#include <Engine/Log.h>

SDG::SpriteAtlas::SpriteAtlas(ContentMgr &content) : content_(content)
{
}

SDG::SpriteAtlas::~SpriteAtlas()
{
    if (!frames_.empty())
        Close();
}

bool SDG::SpriteAtlas::Load(const std::string &atlasPath, const std::string &imagePath)
{
    // Load associated image file
    auto texture = content_.LoadTexture(imagePath);

    if      (atlasPath.ends_with(".lua")) {}
        //LoadFromTexturePackerCoronaSDKLua(atlasPath, texture);
    else if (atlasPath.ends_with(".xml"))
        LoadFromTexturePackerGenericXML(atlasPath, texture);
    else
    {
        SDG_CORE_ERR("SDG::SpriteAtlas::Load failed! Atlas at path \"{0}\" is not a supported file type.", atlasPath);
        return false;
    }

    this->texture_ = texture;
    this->imagePath_ = imagePath;
    this->atlasPath_ = atlasPath;

    return true;
}

void SDG::SpriteAtlas::LogFrames()
{
    for (auto &[key, frame] : frames_)
    {
        SDG_LOG("=====\"" + key + "\"=====\n",
            "x:", frame.x, "\n",
            "y:", frame.y, "\n",
            "w:", frame.w, "\n",
            "h:", frame.h, "\n\n");
    }
}

SDG::Frame SDG::SpriteAtlas::GetFrame(const std::string &key) const
{
    if (frames_.contains(key))
    {
        return frames_.at(key);
    }
    else
    {
        SDG_ERR("Atlas does not contain Frame with key: " + key);
        return Frame();
    }
}

bool SDG::SpriteAtlas::LoadSprites(const std::string &spriteConfigPath)
{
    bool result = false;
    if (spriteConfigPath.ends_with(".xml"))
        result = LoadSpritesFromXML(spriteConfigPath);
    else if (spriteConfigPath.ends_with(".lua")) {}
        //result = LoadSpritesFromLua(spriteConfigPath);
    else
        SDG_CORE_ERR("SDG::SpriteAtlas::LoadSprites failed to load sprites from file \"{0}\": "
                     "file extension not supported.", spriteConfigPath);
    return result;
}

void SDG::SpriteAtlas::Close()
{
    for (auto &[k, v]: sprites_)
    {
        delete v;
    }
    sprites_.clear();
    frames_.clear();

    content_.UnloadTexture(imagePath_);
    imagePath_ = std::string();
    atlasPath_ = std::string();
}

SDG::Sprite *SDG::SpriteAtlas::GetSprite(const std::string &key)
{
    return (sprites_.contains(key)) ? sprites_[key] : nullptr;
}

//bool SDG::SpriteAtlas::LoadFromTexturePackerCoronaSDKLua(const std::string &atlas_path, Texture2D texture)
//{
//    // Load atlas file
//    sol::state lua;
//    lua.open_libraries(sol::lib::base);
//    auto lr = lua.script("sheetInfo = dofile('" + atlas_path + "')");
//    if (!lr.valid())
//    {
//        sol::error err = lr;
//        SDG_ERR("In SpriteAtlas::Load: Error loading lua file \"" + atlas_path +
//            "\":\n\n\t", err.what());
//        return false;
//    }
//
//    sol::table indices = lua["sheetInfo"]["frameIndex"];
//    sol::table rawFrames = lua["sheetInfo"]["sheet"]["frames"];
//
//    std::map<std::string, Frame> frames;
//
//    // Load info into frame map
//    for (const auto &[k, v] : indices)
//    {
//        int index = v.as<int>();
//        std::string key = k.as<std::string>();
//        sol::object sourceX, sourceY, sourceWidth, sourceHeight;
//        sourceX = rawFrames[index]["sourceX"];
//        sourceY = rawFrames[index]["sourceY"];
//        sourceWidth = rawFrames[index]["sourceWidth"];
//        sourceHeight = rawFrames[index]["sourceHeight"];
//
//        Frame f
//        {
//            rawFrames[index]["x"],
//            rawFrames[index]["y"],
//            rawFrames[index]["width"],
//            rawFrames[index]["height"],
//            sourceX.valid() ? sourceX.as<int>() : 0,
//            sourceY.valid() ? sourceY.as<int>() : 0,
//            sourceWidth.valid() ? sourceWidth.as<int>() : rawFrames[index]["width"],
//            sourceHeight.valid() ? sourceHeight.as<int>() : rawFrames[index]["height"],
//            texture,
//            false,
//            false,
//            0,  // default, unsupported in Corona SDK
//            0,  // default, unsupported in Corona SDK
//            key
//        };
//
//        frames[key] = f;
//    }
//
//    // Commit changes.
//    this->frames_.swap(frames);
//
//    return true;
//}

bool SDG::SpriteAtlas::LoadFromTexturePackerGenericXML(const std::string &atlas_path, Texture2D texture)
{
    // Open xml document, check for errors
    tinyxml2::XMLDocument doc{};
    tinyxml2::XMLError result = doc.LoadFile(atlas_path.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        SDG_CORE_ERR("SDG::SpriteAtlas::LoadFromTexturePackerGenericXML tinyxml2 failed to open file \"{0}\": {1}",
                     atlas_path, tinyxml2::XMLDocument::ErrorIDToName(result));
        return false;
    }

    tinyxml2::XMLElement *atlas = doc.FirstChildElement("TextureAtlas");

    // Get frames from each xml child element
    std::map<std::string, Frame> frames;
    Frame f;

    for (tinyxml2::XMLElement *current = atlas->FirstChildElement(); current != nullptr;
        current = current->NextSiblingElement())
    {
        // Get data
        const tinyxml2::XMLAttribute *attr;
        attr = current->FindAttribute("n");
        if (!attr)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadTexturePackerGenericXML tinyxml2 document from path \"{0}\" is invalid. "
                         "There is a missing attribute \"n\" in one of the sprite elements");
            return false;
        }
        const char *key = attr->Value();

        attr = current->FindAttribute("x");
        if (!attr)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadTexturePackerGenericXML tinyxml2 document from path \"{0}\" is invalid. "
                         "There is a missing attribute \"x\" in the sprite element on line {1}",
                         atlas_path, current->GetLineNum());
            return false;
        }
        f.x = attr->IntValue();

        attr = current->FindAttribute("y");
        if (!attr)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadTexturePackerGenericXML tinyxml2 document from path \"{0}\" is invalid. "
                         "There is a missing attribute \"y\" in the sprite element on line {1}",
                         atlas_path, current->GetLineNum());
            return false;
        }
        f.y = attr->IntValue();

        attr = current->FindAttribute("w");
        if (!attr)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadTexturePackerGenericXML tinyxml2 document from path \"{0}\" is invalid. "
                         "There is a missing attribute \"w\" in the sprite element on line {1}",
                         atlas_path, current->GetLineNum());
            return false;
        }
        f.w = attr->IntValue();

        attr = current->FindAttribute("h");
        if (!attr)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadTexturePackerGenericXML tinyxml2 document from path \"{0}\" is invalid. "
                         "There is a missing attribute \"h\" in the sprite element on line {1}",
                         atlas_path, current->GetLineNum());
            return false;
        }
        f.h = attr->IntValue();

        attr = current->FindAttribute("pX");
        f.pivX = (!attr) ? 0 : attr->FloatValue(); // Whether or not this sheet supports pivot points, it will be set.

        attr = current->FindAttribute("pY");
        f.pivY = (!attr) ? 0 : attr->FloatValue(); // Whether or not this sheet supports pivot points, it will be set.

        f.hasPivot = true; //(f.pivX != 0 || f.pivY != 0);

        attr = current->FindAttribute("oX");
        f.ox = (attr) ? attr->IntValue() : 0;

        attr = current->FindAttribute("oY");
        f.oy = (attr) ? attr->IntValue() : 0;

        attr = current->FindAttribute("oW");
        f.ow = (attr) ? attr->IntValue() : f.w;

        attr = current->FindAttribute("oH");
        f.oh = (attr) ? attr->IntValue() : f.h;

        f.rotated = (bool)current->FindAttribute("r"); // whether or not this attr exists determines value
        f.texture = texture;

        f.key = key;

        frames[key] = f;
        f.Log();
    }

    // Commit changes.
    this->frames_.swap(frames);

    return true;
}

//bool SDG::SpriteAtlas::LoadSpritesFromLua(const std::string &spriteConfigPath)
//{
//    // Load sprite info from corresponding sprites file.
//    sol::state lua;
//    lua.open_libraries(sol::lib::base);
//    lua.script_file(spriteConfigPath);
//
//    sol::table sprinfo = lua["sprites"];
//    std::map<std::string, Sprite *> temp_sprite_map;
//    try {
//        for (auto &s : sprinfo) {
//            sol::table current_info = s.second;
//            sol::table frame_info = current_info["frames"];
//            sol::table reel_info = current_info["reel"];
//
//            std::vector<Frame> frames;
//            std::vector<int> reel;
//
//            for (auto &f : frame_info) {
//                std::string frame_key = f.second.as<std::string>();
//                if (frames_.count(frame_key) == 0)
//                {
//                    std::string errMsg = "Frame key\"";
//                    errMsg += frame_key;
//                    errMsg += "\" does not exist in atlas \"";
//                    errMsg += spriteConfigPath;
//                    errMsg += "\"";
//                    throw std::runtime_error(errMsg);
//                }
//
//                frames.emplace_back(frames_[f.second.as<std::string>()]);
//            }
//
//            for (auto &r : reel_info) {
//                int frame_index = r.second.as<int>() - 1;
//
//                if (frame_index < 0 || frame_index >= (int)frames.size()) {
//                    throw std::runtime_error("Reel frame index " + std::to_string(frame_index) +
//                                             " out of range in sprite file \"" + spriteConfigPath + "\", sprite index: "
//                                             + std::to_string(r.first.as<int>()) + ". Add +1 for Lua indices.");
//                }
//                reel.emplace_back(frame_index);
//            }
//
//            // Get base speed (fps).
//            float base_speed = current_info["base_speed"];
//
//            // Get offset.
//            Vector2 offset;
//            offset.x = current_info["offset"]["x"];
//            offset.y = current_info["offset"]["y"];
//            Rectangle mask;
//            mask.x = current_info["mask"]["x"];
//            mask.y = current_info["mask"]["y"];
//            mask.w = current_info["mask"]["w"];
//            mask.h = current_info["mask"]["h"];
//            auto *sprite = new Sprite(current_info["key"], std::move(frames), std::move(reel), mask, offset, base_speed);
//
//            temp_sprite_map[current_info["key"]] = sprite;
//        }
//    } catch(...) { // clean up if any problems interpreting data.
//        for (auto &[k, v] : temp_sprite_map) {
//            delete v;
//        }
//        throw; // pass exception transparently
//    }
//
//    // Clean out any remaining sprites in the current map.
//    for (auto &[k, v] : sprites_)
//    {
//        delete v;
//    }
//
//    sprites_.swap(temp_sprite_map);
//    return true;
//}

bool SDG::SpriteAtlas::LoadSpritesFromXML(const std::string &spriteConfigPath)
{
    // Open document
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(spriteConfigPath.c_str());
    if (result != tinyxml2::XML_SUCCESS)
    {
        SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\". TinyXML2 err: {1}",
                     spriteConfigPath, tinyxml2::XMLDocument::ErrorIDToName(result));
        return false;
    }

    // Get sprites
    tinyxml2::XMLElement *spritesElement = doc.FirstChildElement("sprites");
    if (!spritesElement)
    {
        SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": Failed to get sprites element.",
                     spriteConfigPath);
        return false;
    }

    // Fill temp sprite map with each sprite inside the sprites element
    std::map<std::string, Sprite *> temp;
    for (tinyxml2::XMLElement *current = spritesElement->FirstChildElement();
         current != nullptr;
         current = current->NextSiblingElement())
    {
        const char *key;
        std::vector<Frame> frames;
        std::vector<int> reel;
        Rectangle mask;
        Vector2 offset;
        float base_speed;
        tinyxml2::XMLError result;

        // Get the key
        result = current->QueryAttribute("key", &key);
        if (result != tinyxml2::XML_SUCCESS)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get attribute \"{1}\" from element on line {2}.",
                         spriteConfigPath, "key", current->GetLineNum());
            goto failed;
        }

        // Get the base_speed
        result = current->QueryFloatAttribute("base_speed", &base_speed);
        if (result != tinyxml2::XML_SUCCESS)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get float attribute \"{1}\" from element on line {2}.",
                         spriteConfigPath, "base_speed", current->GetLineNum());
            goto failed;
        }

        // Get the frames
        tinyxml2::XMLElement *framesElement = current->FirstChildElement("frames");
        if (!framesElement)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get child element \"{1}\" from sprite element on line {2}.",
                         spriteConfigPath, "frames", current->GetLineNum());
            goto failed;
        }

        // Visit each frame sub-element to get the frame keys
        for (tinyxml2::XMLElement *value = framesElement->FirstChildElement(); value != nullptr; value = value->NextSiblingElement())
        {
            // Get the frame key
            const char *frame_key = value->GetText();
            if (!frame_key)
            {
                SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                             "A frame key is missing text data on line {1}.",
                             spriteConfigPath, value->GetLineNum());
                goto failed;
            }

            // Find if this frame exists in the previously loaded atlas
            auto it = frames_.find(frame_key);
            if (it == frames_.end())
            {
                SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                             "Frame key \"{1}\" was not found in atlas on line {2}.",
                             spriteConfigPath, frame_key, value->GetLineNum());
                goto failed;
            }

            // Place the frame in the temporary frame vector
            frames.emplace_back(it->second);
        }

        if (frames.empty())
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Frame list from sprite on line {1} is empty. There must be at least one viable frame key.",
                         spriteConfigPath, current->GetLineNum());
            goto failed;
        }

        // Get the reel
        tinyxml2::XMLElement *reelElement = current->FirstChildElement("reel");
        if (!reelElement)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get child element \"{1}\" from sprite element on line {2}.",
                         spriteConfigPath, "reel", current->GetLineNum());
            goto failed;
        }

        // Visit each reel sub-element to get the reel indices
        for (tinyxml2::XMLElement *value = reelElement->FirstChildElement(); value != nullptr; value = value->NextSiblingElement())
        {
            // Get the frame key
            int reel_index;
            result = value->QueryIntText(&reel_index);
            if (result != tinyxml2::XML_SUCCESS)
            {
                SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                             "Failed to get reel index from element on line {1}. tinyxml2 err: {2}",
                             spriteConfigPath, value->GetLineNum(), tinyxml2::XMLDocument::ErrorIDToName(result));
                goto failed;
            }

            --reel_index; // remove one-based index

            // Ensure reel index is in range.
            if (reel_index >= (int)frames.size() || reel_index < 0)
            {
                SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                             "Reel index on line {1} is out of range.",
                             spriteConfigPath, value->GetLineNum());
                goto failed;
            }

            // Place the frame in the temporary frame vector
            reel.emplace_back(reel_index);
        }

        if (reel.empty())
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Reel from sprite on line {1} is empty. There must be at least one index.",
                         spriteConfigPath, current->GetLineNum());
            goto failed;
        }


        // Get the mask
        tinyxml2::XMLElement *maskElement = current->FirstChildElement("mask");
        if (!maskElement)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": Failed to get mask element from sprite on line {1}.",
                         spriteConfigPath, current->GetLineNum());
            goto failed;
        }

        result = maskElement->QueryAttribute("x", &mask.x);
        if (result != tinyxml2::XML_SUCCESS)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get mask attribute \"x\" on line {1}.",
                         spriteConfigPath, maskElement->GetLineNum());
            goto failed;
        }

        result = maskElement->QueryAttribute("y", &mask.y);
        if (result != tinyxml2::XML_SUCCESS)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get mask attribute \"y\" on line {1}.",
                         spriteConfigPath, maskElement->GetLineNum());
            goto failed;
        }

        result = maskElement->QueryAttribute("w", &mask.w);
        if (result != tinyxml2::XML_SUCCESS)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get mask attribute \"w\" on line {1}.",
                         spriteConfigPath, maskElement->GetLineNum());
            goto failed;
        }

        result = maskElement->QueryAttribute("h", &mask.h);
        if (result != tinyxml2::XML_SUCCESS)
        {
            SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed to load sprites from \"{0}\": "
                         "Failed to get mask attribute \"h\" on line {1}.",
                         spriteConfigPath, maskElement->GetLineNum());
            goto failed;
        }

        // Get offset
        tinyxml2::XMLElement *offsetElement = current->FirstChildElement("offset");
        if (offsetElement)
        {
            result = offsetElement->QueryAttribute("x", &offset.x);
            if (result != tinyxml2::XML_SUCCESS)
            {
                SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed with file \"{0}\": "
                             "could not retrieve x attribute from offset element on line {1}",
                             spriteConfigPath, offsetElement->GetLineNum());
                goto failed;
            }

            result = offsetElement->QueryAttribute("y", &offset.y);
            if (result != tinyxml2::XML_SUCCESS)
            {
                SDG_CORE_ERR("SDG::SpriteAtlas::LoadSpritesFromXML failed with file \"{0}\": "
                             "could not retrieve y attribute from offset element on line {1}",
                             spriteConfigPath, offsetElement->GetLineNum());
                goto failed;
            }
        }



        temp[key] = new Sprite(key, std::move(frames), std::move(reel), mask, offset, base_speed);
    }


    // Clean out any remaining sprites in the current map.
    for (auto &[k, v] : sprites_)
    {
        delete v;
    }

    sprites_.swap(temp);
    return true;

    failed:
    for (auto &[k, sprite] : temp)
    {
        delete sprite;
    }
    return false;
}
