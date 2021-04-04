#include "Sprite.h"
#include <algorithm>

SDG::Sprite::Sprite(const Sprite &other)
{
    baseSpeed = other.baseSpeed;
    frames = other.frames;
    reel = other.reel;
    mask = other.mask;
}

void SDG::Sprite::Swap(Sprite &other)
{
    std::swap(baseSpeed, other.baseSpeed);
    frames.swap(other.frames);
    reel.swap(other.reel);
    std::swap(mask, other.mask);
    std::swap(offset, other.offset);
}

SDG::Sprite::Sprite(std::vector<Frame> frames, std::vector<int> reel,
                    SDG::Rectangle mask, SDG::Vector2 offset, float base_speed)
        : frames(std::move(frames)), reel(std::move(reel)),
          baseSpeed(base_speed), mask(mask), offset(offset)
{
}
