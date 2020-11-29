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
