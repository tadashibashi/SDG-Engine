/* =============================================================================
 * Object
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include "Property.h"
#include <map>
#include <string>

// Forward declaration
namespace tinyxml2
{
    class XMLElement;
}

namespace SDG::Tiled
{
    struct SDG_API Object
    {
        int id;
        std::string name;
        std::string type;
        float x, y;
        float width, height;
        float rotation;
        std::map<std::string, Property> properties;

        static Object Parse(tinyxml2::XMLElement *element, const std::string &rootPath);
        void Log();
    };
}