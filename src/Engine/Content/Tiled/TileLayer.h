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
    struct  TileLayer : public Layer
    {
        int width;
        int height;
        std::vector<int> tiles;
        std::string dataEncoding;
    };
}