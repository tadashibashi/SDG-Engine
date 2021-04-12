/* =============================================================================
 * ObjectLayer
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include "Object.h"
#include <vector>
#include <string>
#include "Layer.h"

namespace SDG::Tiled
{
    struct  ObjectLayer : public Layer
    {
        std::vector<Object> objects;
    };
}