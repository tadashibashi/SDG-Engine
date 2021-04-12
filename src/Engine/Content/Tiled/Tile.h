/* =============================================================================
 * Tile
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>
#include <map>
#include "Property.h"

namespace SDG::Tiled
{
    // Tile metadata
    struct  Tile
    {
        int id;
        std::string type;
        std::map<std::string, Property> properties;
    };

}



