#include "SpriteAtlas.h"

#include <sol/sol.hpp>
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

    // Load atlas file
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    auto lr = lua.script("sheetInfo = dofile('" + atlasPath + "')");
    if (!lr.valid())
    {
        sol::error err = lr;
        SDG_ERR("In SpriteAtlas::Load: Error loading lua file \"" + atlasPath + 
            "\":\n\n\t", err.what());
        return false;
    }

    sol::table indices = lua["sheetInfo"]["frameIndex"];
    sol::table rawFrames = lua["sheetInfo"]["sheet"]["frames"];

    std::map<std::string, Frame> frames;

    // Load info into frame map
    for (const auto &[k, v] : indices)
    {
        int index = v.as<int>();
        std::string key = k.as<std::string>();

        Frame f
        {
            rawFrames[index]["x"],
            rawFrames[index]["y"],
            rawFrames[index]["width"],
            rawFrames[index]["height"],
            rawFrames[index]["sourceX"],
            rawFrames[index]["sourceY"],
            rawFrames[index]["sourceWidth"],
            rawFrames[index]["sourceHeight"],
            texture
        };

        frames[key] = f;
    }

    // Commit changes.
    this->frames_.swap(frames);
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
    // Load sprite info from corresponding sprites file.
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file(spriteConfigPath);

    sol::table sprinfo = lua["sprites"];
    std::map<std::string, Sprite *> temp_sprite_map;
    try {
        for (auto &s : sprinfo) {
            sol::table current_info = s.second;
            sol::table frame_info = current_info["frames"];
            sol::table reel_info = current_info["reel"];

            std::vector<Frame> frames;
            std::vector<int> reel;

            for (auto &f : frame_info) {
                std::string frame_key = f.second.as<std::string>();
                if (frames_.count(frame_key) == 0)
                {
                    std::string errMsg = "Frame key\"";
                    errMsg += frame_key;
                    errMsg += "\" does not exist in atlas \"";
                    errMsg += spriteConfigPath;
                    errMsg += "\"";
                    throw std::runtime_error(errMsg);
                }

                frames.emplace_back(frames_[f.second.as<std::string>()]);
            }

            for (auto &r : reel_info) {
                int frame_index = r.second.as<int>() - 1;

                if (frame_index < 0 || frame_index >= frames.size()) {
                    throw std::runtime_error("Reel frame index " + std::to_string(frame_index) +
                                             " out of range in sprite file \"" + spriteConfigPath + "\", sprite index: "
                                             + std::to_string(r.first.as<int>()) + ". Add +1 for Lua indices.");
                }
                reel.emplace_back(frame_index);
            }

            // Get base speed (fps).
            float base_speed = current_info["base_speed"];

            // Get offset.
            Vector2 offset;
            offset.x = current_info["offset"]["x"];
            offset.y = current_info["offset"]["y"];
            Rectangle mask;
            mask.x = current_info["mask"]["x"];
            mask.y = current_info["mask"]["y"];
            mask.w = current_info["mask"]["w"];
            mask.h = current_info["mask"]["h"];
            auto *sprite = new Sprite(std::move(frames), std::move(reel), mask, offset, base_speed);

            temp_sprite_map[current_info["key"]] = sprite;
        }
    } catch(...) { // clean up if any problems interpreting data.
        for (auto &[k, v] : temp_sprite_map) {
            delete v;
        }
        throw;
    }

    // Clean out any remaining sprites in the current map.
    for (auto &[k, v] : sprites_)
    {
        delete v;
    }

    sprites_.swap(temp_sprite_map);
    return true;
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
