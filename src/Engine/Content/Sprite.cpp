#include "Sprite.h"

#include <Engine/Log.h>
#include <algorithm>

SDG::Sprite::Sprite(const Sprite &other) : baseSpeed(other.baseSpeed), frames(other.frames),
    reel(other.reel), mask(other.mask), key(other.key)
{
}

void SDG::Sprite::Swap(Sprite &other)
{
    std::swap(baseSpeed, other.baseSpeed);
    frames.swap(other.frames);
    reel.swap(other.reel);
    std::swap(mask, other.mask);
    std::swap(offset, other.offset);
    std::swap(key, other.key);
}

SDG::Sprite::Sprite(std::string key, std::vector<Frame> frames, std::vector<int> reel,
                    SDG::Rectangle mask, SDG::Vector2 offset, float base_speed)
        : key(std::move(key)), frames(std::move(frames)), reel(std::move(reel)),
          baseSpeed(base_speed), mask(mask), offset(offset)
{
}

void SDG::Sprite::Log() const
{
    std::string reelStr;
    for (int i : reel) reelStr += std::to_string(i) + " ";
    std::string frameKeysStr;
    for (const Frame &f : frames) frameKeysStr += std::string(f.key) + "\n\t";
    SDG_CORE_LOG("\n===== Sprite: \"{0}\" =====\n"
                 "mask:   x {1}, y {2}, w {3}, h {4}\n"
                 "offset: x {5}, y {6}\n"
                 "reel: [ {7}]\n"
                 "frames: {8}\n",
                 key, mask.x, mask.y, mask.w, mask.h, offset.x, offset.y, reelStr, frameKeysStr);

}
