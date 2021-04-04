/* =============================================================================
 * SceneStore
 * 
 * 
 * ===========================================================================*/
#include "SceneCache.h"

namespace SDG
{
    SceneCache::SceneCache()
    {
    }

    SceneCache::~SceneCache()
    {
        for (auto &[k, v]: scenes_)
        {
            delete v;
        }
    }
}