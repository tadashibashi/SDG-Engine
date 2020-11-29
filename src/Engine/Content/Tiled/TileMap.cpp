/* =============================================================================
 * TileMap
 * 
 * 
 * ===========================================================================*/
#include "TileMap.h"
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cstring>

#ifndef XMLCheck
#define XMLCheck(err) \
    if ((err) != tinyxml2::XML_SUCCESS) \
    {                                   \
        std::cout << "TinyXML2 Error, with code:" << err << '\n'; \
        return err;                   \
    }
#endif

namespace SDG::Tiled
{
    int hexchar_to_int(char c)
    {
        c = (char)std::tolower(c);
        std::string digits = "0123456789abcdef";
        return digits.find_first_of(c);
    }

    tinyxml2::XMLError TileMap::Load(const std::string &path)
    {
        using namespace tinyxml2;

        XMLDocument doc;
        XMLError result;
        result = doc.LoadFile(path.c_str());
        XMLCheck(result);

        XMLElement *root = doc.RootElement();
        if (!root) return XML_ERROR_FILE_READ_ERROR;

        int pWidth, pHeight, pTilewidth, pTileheight, pNextlayerid, pNextobjectid, pInfinite;
        const char *pBackgroundcolor;
        result = root->QueryAttribute("width", &pWidth);
        XMLCheck(result);
        result = root->QueryAttribute("height", &pHeight);
        XMLCheck(result);
        result = root->QueryAttribute("tilewidth", &pTilewidth);
        XMLCheck(result);
        result = root->QueryAttribute("tileheight", &pTileheight);
        XMLCheck(result);
        result = root->QueryAttribute("nextlayerid", &pNextlayerid);
        XMLCheck(result);
        result = root->QueryAttribute("nextobjectid", &pNextobjectid);
        XMLCheck(result);
        result = root->QueryAttribute("infinite", &pInfinite);
        XMLCheck(result);
        result = root->QueryAttribute("backgroundcolor", &pBackgroundcolor);
        if (result != XML_SUCCESS)
        {
            pBackgroundcolor = "#00000000";
        }

        std::vector<TileSet> pTileSets;
        std::vector<TileLayer> pTileLayers;
        std::vector<ObjectLayer> pObjectLayers;

        // ============ PARSE INFO ============
        for (XMLElement *element = root->FirstChildElement();
            element != nullptr;
            element = element->NextSiblingElement())
        {
            if (std::strcmp(element->Name(), "tileset") == 0) // tile set
            {
                TileSet pTileSet;

                // Query TileSet Attributes
                const char *pTileSetName;
                int pTileSetTileWidth, pTileSetTileHeight, pTileSetFirstGID,
                    pTileSetTileCount, pTileSetColumns;
                result = element->QueryAttribute("name", &pTileSetName);
                XMLCheck(result);
                result = element->QueryAttribute("firstgid", &pTileSetFirstGID);
                XMLCheck(result);
                result = element->QueryAttribute("tilewidth", &pTileSetTileWidth);
                XMLCheck(result);
                result = element->QueryAttribute("tileheight", &pTileSetTileHeight);
                XMLCheck(result);
                result = element->QueryAttribute("tilecount", &pTileSetTileCount);
                XMLCheck(result);
                result = element->QueryAttribute("columns", &pTileSetColumns);
                XMLCheck(result);

                std::vector<Image> pTileSetImages;
                std::map<int, Tile> pTileSetTiles;

                // Query TileSet Images and Tiles Metadata
                for (XMLElement *tileSetChild = element->FirstChildElement();
                     tileSetChild != nullptr;
                     tileSetChild = tileSetChild->NextSiblingElement())
                {
                    if (std::strcmp(tileSetChild->Name(), "image") == 0)
                    {
                        const char *pTileSetImagePath;
                        int pTileSetImageWidth, pTileSetImageHeight;

                        result = tileSetChild->QueryAttribute("source", &pTileSetImagePath);
                        XMLCheck(result);
                        result = tileSetChild->QueryAttribute("width", &pTileSetImageWidth);
                        XMLCheck(result);
                        result = tileSetChild->QueryAttribute("height", &pTileSetImageHeight);
                        XMLCheck(result);
                        Image pImage(pTileSetImagePath, pTileSetImageWidth, pTileSetImageHeight);
                        pTileSetImages.push_back(std::move(pImage));
                    }
                    else
                    if (std::strcmp(tileSetChild->Name(), "tile") == 0)
                    {
                        Tile pTile;

                        int pTileSetTileID;
                        const char *pTileSetTileType;
                        result = tileSetChild->QueryAttribute("id", &pTileSetTileID);
                        XMLCheck(result);
                        result = tileSetChild->QueryAttribute("type", &pTileSetTileType);
                        XMLCheck(result);

                        std::map<std::string, Property> pTileProperties;
                        // Get Tile Properties
                        for (XMLElement *tileProperty = tileSetChild->FirstChildElement("properties")->FirstChildElement("property");
                            tileProperty != nullptr;
                            tileProperty = tileProperty->NextSiblingElement())
                        {
                            Property p = Property::Parse(tileProperty);
                            pTileProperties[p.GetName()] = std::move(p);
                        }

                        pTile.id = pTileSetTileID;
                        pTile.type = pTileSetTileType;
                        pTile.properties.swap(pTileProperties);

                        pTileSetTiles[pTile.id] = std::move(pTile);
                    }

                }

                // Commit Tileset and push into tilesets vector
                pTileSet.images.swap(pTileSetImages);
                pTileSet.tiles.swap(pTileSetTiles);
                pTileSet.tileCount = pTileSetTileCount;
                pTileSet.tileWidth = pTileSetTileWidth;
                pTileSet.tileHeight = pTileSetTileHeight;
                pTileSet.name = pTileSetName;
                pTileSet.firstgid = pTileSetFirstGID;
                pTileSet.columns = pTileSetColumns;
                pTileSets.push_back(std::move(pTileSet));
            }
            else if (std::strcmp(element->Name(), "layer") == 0)       // Tile Layer
            {
                TileLayer pTileLayer;
                // Parse main meta data
                const char *pLayerName;
                int pLayerID, pLayerWidth, pLayerHeight;
                result = element->QueryAttribute("id", &pLayerID);
                XMLCheck(result);
                result = element->QueryAttribute("name", &pLayerName);
                XMLCheck(result);
                result = element->QueryAttribute("width", &pLayerWidth);
                XMLCheck(result);
                result = element->QueryAttribute("height", &pLayerHeight);
                XMLCheck(result);

                // Get tile placement data
                std::vector<int> pTileData; const char *pTileDataEncoding;
                {
                    XMLElement *pTileDataElement = element->FirstChildElement("data");
                    if (!pTileDataElement)
                    {
                        throw std::runtime_error("Invalid Tiled TMX. There was no tile data in a tile layer.");
                    }
                    const char *pTileDataTextPtr = pTileDataElement->GetText();
                    if (!pTileDataTextPtr)
                    {
                        throw std::runtime_error("Could not retrieve TileLayer data from TMX element");
                    }


                    result = pTileDataElement->QueryAttribute("encoding", &pTileDataEncoding);
                    XMLCheck(result);

                    // Parse tile placement data CSV
                    if (std::strcmp(pTileDataEncoding, "csv") != 0)
                    {
                        throw std::runtime_error("TileLayer data encoding type " + std::string(pTileDataEncoding) +
                                                 " not supported. Must be csv (for the time being).");
                    }

                    if (!ParseCSVData(pTileDataTextPtr, pTileData))
                    {
                        throw std::runtime_error("Failed to parse TMX CSV data");
                    }
                }

                // Commit changes
                pTileLayer.id = pLayerID;
                pTileLayer.name = pLayerName;
                pTileLayer.width = pLayerWidth;
                pTileLayer.height = pLayerHeight;
                pTileLayer.dataEncoding = pTileDataEncoding;
                pTileLayer.tiles.swap(pTileData);
                pTileLayers.push_back(std::move(pTileLayer));
            }
            else if (std::strcmp(element->Name(), "objectgroup") == 0) // Object Layer
            {
                ObjectLayer pObjectLayer;

                // Parse top-level ObjectLayer metadata
                int pObjectLayerID; const char *pObjectLayerName;
                {
                    result = element->QueryAttribute("id", &pObjectLayerID);
                    XMLCheck(result);
                    result = element->QueryAttribute("name", &pObjectLayerName);
                    XMLCheck(result);
                }

                std::vector<Object> pObjects;
                // Parse ObjectLayer objects
                for (XMLElement *pObjectElement = element->FirstChildElement();
                    pObjectElement != nullptr;
                    pObjectElement = pObjectElement->NextSiblingElement())
                {
                    Object pObject = Object::Parse(pObjectElement, path);
                    pObjects.push_back(std::move(pObject));
                }

                pObjectLayer.objects.swap(pObjects);
                pObjectLayer.id = pObjectLayerID;
                pObjectLayer.name = pObjectLayerName;
                pObjectLayers.push_back(std::move(pObjectLayer));
            }
        }

        // Commit changes at the end
        this->width = pWidth;
        this->height = pHeight;
        this->tileWidth = pTilewidth;
        this->tileHeight = pTileheight;
        this->nextLayerID = pNextlayerid;
        this->nextObjectID = pNextobjectid;
        this->isInfinite = (pInfinite != 0);
        this->tileSets.swap(pTileSets);
        this->tileLayers.swap(pTileLayers);
        this->objectLayers.swap(pObjectLayers);

        // parse color hex string.
        backgroundcolor = Color::FromRGBString(std::string(pBackgroundcolor));

        return XML_SUCCESS;
    }

    bool TileMap::ParseCSVData(const char *data, std::vector<int> &output)
    {
        std::vector<int> temp;
        // CSV encoding.
        char *pTileDataCopy = strdup(data);
        const char *pChar = std::strtok(pTileDataCopy, ",\n");
        while(pChar != nullptr)
        {
            temp.push_back(std::stoi(pChar));
            pChar = std::strtok(nullptr, ",\n");
        }
        free(pTileDataCopy);

        temp.swap(output);

        return true;
    }
}