#ifndef SDG_SPRITE_H
#define SDG_SPRITE_H

#include <Engine/Core.h>
#include "Frame.h"
#include <vector>
#include <Engine/Math/Vector2.h>
#include <Engine/Math/Rectangle.h>

namespace SDG
{
    class SDG_API Sprite {
    public:
        Sprite() : baseSpeed(1.f), mask() {}
        Sprite(std::vector<Frame> frames, std::vector<int> reel, Rectangle mask, Vector2 offset, float base_speed)
            : frames(std::move(frames)), reel(std::move(reel)), baseSpeed(base_speed), mask(mask), offset(offset)
        {
        }

        Sprite(const Sprite &other);
        Sprite &operator=(const Sprite &other)
        {
            Sprite temp(other);
            Swap(temp);
            return *this;
        }

        void Swap(Sprite &other);

        // Gets the frame at indicated reel position. 
        const Frame &operator[](int i) const
        {
            return frames[reel[i]];
        }

        // Gets the frame at indicated reel position. 
        // Throws an exception if out of range.
        [[nodiscard]] const Frame &At(int index) const
        {
            return frames.at(reel.at(index));
        }

        [[nodiscard]] float GetBaseSpeed() const { return baseSpeed; }

        // Returns the size of the reel
        [[nodiscard]] size_t GetLength() const { return reel.size(); }
        [[nodiscard]] int GetReelValue(int index) const { return reel[index]; }
        [[nodiscard]] Vector2 GetOffset() const { return offset; }
        [[nodiscard]] Rectangle GetMask() const { return mask; }
        [[nodiscard]] bool HasMask() const { return !mask.IsEmpty(); }

    private:
        std::vector<Frame> frames;
        std::vector<int> reel;
        float baseSpeed;
        Rectangle mask; // Intended collision mask box.
        Vector2 offset;
    };
}

#endif
