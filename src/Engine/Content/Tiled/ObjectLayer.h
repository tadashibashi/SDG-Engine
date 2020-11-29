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
    struct SDG_API ObjectLayer : public Layer
    {
        std::vector<Object> objects;
    };
}