/* =============================================================================
 * Color
 * 
 * 
 * ===========================================================================*/
#include "Color.h"

#include <stdexcept>

static const GLubyte HEX_BASE = 16;
static const size_t RGBA_REQUIRED_STRING_LENGTH = 9;
static const size_t RGB_REQUIRED_STRING_LENGTH = 7;
static const GLubyte GLUBYTE_MAX = 255;

namespace SDG
{

    Color Color::FromRGBAString(const std::string &str)
    {
        if (str.length() != RGBA_REQUIRED_STRING_LENGTH)
        {
            throw std::logic_error(
                    "Color::FromRGBString failed: str of hex chars \"" +
                    str + "\" is invalid in length. Must be " +
                    std::to_string(RGBA_REQUIRED_STRING_LENGTH) + "chars long "
                          "with format #rrggbbaa");
        }
        else
        {
            return Color(
                    HexCharToByte(str[1]) * HEX_BASE + HexCharToByte(str[2]),
                    HexCharToByte(str[3]) * HEX_BASE + HexCharToByte(str[4]),
                    HexCharToByte(str[5]) * HEX_BASE + HexCharToByte(str[6]),
                    HexCharToByte(str[7]) * HEX_BASE + HexCharToByte(str[8]));
        }
    }

    Color Color::FromRGBString(const std::string &str) {
        if (str.length() != RGB_REQUIRED_STRING_LENGTH)
        {
            throw std::logic_error(
                    "Color::FromRGBString failed: str of hex chars \"" +
                    str + "\" is invalid in length. Must be " +
                    std::to_string(RGB_REQUIRED_STRING_LENGTH) + " chars long "
                          "with format #rrggbb");
        }
        else
        {
            return Color(
                    HexCharToByte(str[1]) * HEX_BASE + HexCharToByte(str[2]),
                    HexCharToByte(str[3]) * HEX_BASE + HexCharToByte(str[4]),
                    HexCharToByte(str[5]) * HEX_BASE + HexCharToByte(str[6]),
                    GLUBYTE_MAX);
        }
    }

    GLubyte Color::HexCharToByte(char c)
    {
        static const std::string digits = "0123456789abcdef";

        c = (char)std::tolower(c);
        size_t pos = digits.find_first_of(c);
        if (pos == std::string::npos)
            SDG_ERR("HexCharToByte failed:", c, "is not a valid hex char");

        return (GLubyte)digits.find_first_of(c);
    }

    std::string Color::ToString() const
    {
        return std::string("{ " + std::to_string(r) + ", " + std::to_string(g) +
                           ", " + std::to_string(b) + ", " + std::to_string(a) + " }");
    }

    FColor Color::ToFColor() const
    {
        return FColor {
                .r = (float)r / 255.f,
                .g = (float)g / 255.f,
                .b = (float)b / 255.f,
                .a = (float)a / 255.f
        };
    }
}