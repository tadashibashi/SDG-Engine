/* =============================================================================
 * Color
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/GraphicsLibrary.h>
#include <Engine/Log.h>
#include <string>

namespace SDG
{
    // Normalized Color
    struct FColor
    {
        float r, g, b, a;
    };

    struct Color
    {
        Color(unsigned char r, unsigned char g,
              unsigned char b, unsigned char a)
              : r{r}, g{g}, b{b}, a{a} { }

        Color() : r{}, g{}, b{}, a{} { }

        explicit Color(int rgbaHex) : r(), g(), b(), a()
        {
            a = rgbaHex % 256;
            rgbaHex /= 256;
            b = rgbaHex % 256;
            rgbaHex /= 256;
            g = rgbaHex % 256;
            rgbaHex /= 256;
            r = rgbaHex % 256;
        }

        unsigned char r, g, b, a;

        // Returns a converted FColor, which contains normalized rgba values
        // (0 to 1.0f)
        [[nodiscard]]
        FColor ToFColor() const;

        [[nodiscard]]
        explicit operator FColor() const { return ToFColor(); }

        [[nodiscard]]
        std::string ToString() const;

        [[nodiscard]]
        explicit operator std::string() const { return ToString(); }

        // Converts a string with the format "#rrggbb" into a Color
        [[nodiscard]]
        static Color FromRGBString(const std::string &str);

        // Converts a string with the format "#rrggbbaa" into a Color
        [[nodiscard]]
        static Color FromRGBAString(const std::string &str);

    private:
        // Helper function that converts a hex char to a unsigned char
        [[nodiscard]]
        static unsigned char HexCharToByte(char c);
    };
}
