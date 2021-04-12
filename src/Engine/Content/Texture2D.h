/* =============================================================================
 * Texture2D
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/Math/Vector2.h>
#include <Engine/GraphicsLibrary.h>

namespace SDG
{
    class Texture2D
    {
    public:
        explicit Texture2D(unsigned int id) : id(id), size() {}

        Texture2D() : id{}, size{} { }
        Texture2D(unsigned int id, int width, int height)
            : id(id), size(width, height) { }
        [[nodiscard]] uint GetID() const { return id; }
        [[nodiscard]] Point GetSize() const { return size; }
        [[nodiscard]] int GetWidth() const { return size.w; }
        [[nodiscard]] int GetHeight() const { return size.h; }

        bool operator==(const Texture2D &other) const { return id == other.id; }
        [[nodiscard]] bool IsLoaded() const { return id != 0; }
        int *SizeData() { return &size.x; }
    private:
        // Texture ID
        uint id;
        // Pixel dimensions
        Point size;
    };
}