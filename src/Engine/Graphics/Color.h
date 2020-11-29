/* =============================================================================
 * Color
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <Engine/GL.h>
#include <Engine/Log.h>
#include <string>

namespace SDG
{
    // Normalized Color
    struct SDG_API FColor
    {
        float r, g, b, a;
    };

    struct SDG_API Color
    {
        Color(GLubyte r, GLubyte g,
              GLubyte b, GLubyte a)
              : r(r), g(g), b(b), a(a) { }
        Color() : r(0), g(0), b(0), a(0) { }

        GLubyte r, g, b, a;

        // Returns a converted FColor, which contains normalized rgba values
        // (0 to 1.0f)
        [[nodiscard]] FColor ToFColor() const
        {
            return FColor {
                .r = (float)r / 255.f,
                .g = (float)g / 255.f,
                .b = (float)b / 255.f,
                .a = (float)a / 255.f
            };
        }

        [[nodiscard]] std::string ToString() const
        {
            return std::string("{ " + std::to_string(r) + ", " + std::to_string(g) +
                               ", " + std::to_string(b) + ", " + std::to_string(a) + " }");
        }

        // Converts a string with the format "#rrggbb" into a Color
        static Color FromRGBString(const std::string &str);

        // Converts a string with the format "#rrggbbaa" into a Color
        static Color FromRGBAString(const std::string &str);

    private:
        // Helper function that converts a hex char to a GLubyte
        static GLubyte HexCharToByte(char c);
    };
}