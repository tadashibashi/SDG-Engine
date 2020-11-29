/* =============================================================================
 * SceneStore
 * 
 * 
 * ===========================================================================*/
#include "SceneStore.h"

namespace SDG
{
    SceneStore::SceneStore()
    {
    }

    SceneStore::~SceneStore()
    {
        for (auto &[k, v]: scenes_)
        {
            delete v;
        }
    }
}