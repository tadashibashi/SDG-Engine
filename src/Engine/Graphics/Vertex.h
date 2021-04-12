/* =============================================================================
 * Vertex
 * Vertex data for graphics library rendering.
 * 
 * ===========================================================================*/
#pragma once

#include <Engine/Math/Vector2.h>
#include "Color.h"

namespace SDG
{
    // Vertex data for graphics library rendering.
    struct Vertex
    {
        Vertex() : position{}, color{}, uv{} { }
        Vertex(Vector2 position, Color color, Vector2 uv) : position{position}, color{color}, uv{uv} {}

        Vector2  position;
        Color    color;
        Vector2  uv;
    };
}
