/* =============================================================================
 * TiledMap
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <string>
#include <vector>
#include "TileSet.h"
#include "ObjectLayer.h"
#include "TileLayer.h"
#include <tinyxml2.h>

namespace SDG::Tiled
{
    struct SDG_API TileMap
    {
    public:
        TileMap() = default;
        explicit TileMap(const std::string &filepath)
        {
            Load(filepath);
        }

        int width, height, tileWidth, tileHeight;
        bool isInfinite;
        int nextLayerID, nextObjectID;
        std::vector<TileSet> tileSets;
        std::vector<TileLayer> tileLayers;
        std::vector<ObjectLayer> objectLayers;
        Color backgroundcolor;

        tinyxml2::XMLError Load(const std::string &path);
    private:
        static bool ParseCSVData(const char *data, std::vector<int> &output);
    };
}
