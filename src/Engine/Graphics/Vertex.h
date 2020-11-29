/* =============================================================================
 * Vertex
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include "Color.h"
#include <Engine/Math/Vector2.h>

namespace SDG
{
    struct SDG_API Vertex
    {
        Vertex() : position(), color(), uv() { }
        Vector2 position;
        Color color;
        Vector2 uv;
    };
}