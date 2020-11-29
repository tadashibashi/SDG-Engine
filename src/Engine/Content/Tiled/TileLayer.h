/* =============================================================================
 * TiledTileLayer
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>
#include <vector>
#include "Layer.h"

namespace SDG::Tiled
{
    struct SDG_API TileLayer : public Layer
    {
        int width;
        int height;
        std::vector<int> tiles;
        std::string dataEncoding;
    };
}