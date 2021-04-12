/* =============================================================================
 * Layer
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>

namespace SDG::Tiled
{
    struct  Layer
    {
        virtual ~Layer() = default;

        int id;
        std::string name;
    };
}