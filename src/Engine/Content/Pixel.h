/* =============================================================================
 * Pixel
 * 
 * 
 * ===========================================================================*/
#pragma once

#include <Engine/Core.h>

namespace SDG
{
    struct Pixel
    {
        Pixel() : r{}, g{}, b{}, a{} {}
        Pixel(ubyte r, ubyte g, ubyte b, ubyte a) : r{r}, g{g}, b{b}, a{a} {}

        void Set(ubyte r, ubyte g, ubyte b, ubyte a) { }
        ubyte r, g, b, a;
    };
}