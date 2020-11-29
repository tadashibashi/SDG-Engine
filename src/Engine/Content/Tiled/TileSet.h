/* =============================================================================
 * TiledTileset
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <map>
#include <string>
#include <vector>
#include "Tile.h"
#include <utility>

namespace SDG::Tiled

{   struct SDG_API Image
    {
        Image(std::string pPath, int pWidth, int pHeight):
                path(std::move(pPath)), width(pWidth), height(pHeight) { }
        Image(const char *pPath, int pWidth, int pHeight):
                path(pPath), width(pWidth), height(pHeight) { }
        Image() : path(), width(0), height(0) { }

        std::string path;
        int width;
        int height;
    };

    struct SDG_API TileSet
    {
        int firstgid;
        std::string name;
        int tileWidth;
        int tileHeight;
        int tileCount;
        int columns;
        std::vector<Image> images;
        std::map<int, Tile> tiles;
    };
}