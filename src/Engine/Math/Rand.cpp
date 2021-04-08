/* =============================================================================
 * Rand
 * 
 * 
 * ===========================================================================*/
#include "Rand.h"
#include <Engine/Log.h>

namespace SDG
{
    std::random_device Rand::s_device_;
    std::mt19937 Rand::s_mt_(s_device_());
    std::uniform_real_distribution<float> Rand::s_dist_(0.0, 1.0);

    float Rand::Next()
    {
        return s_dist_(s_mt_);
    }

    float Rand::Next(float n)
    {
        return Next() * n;
    }

    int Rand::INext(int n)
    {
        return static_cast<int>(Next((float)n));
    }

    float Rand::Range(float low, float high)
    {
        SDG_ASSERT(low <= high);
        return Next() * (high - low) + low;
    }

    int Rand::IRange(int low, int high)
    {
        return static_cast<int>(Range((float)low, (float)high));
    }

    bool Rand::Chance(int n, int outof) {
        if (outof > 0)
        {
            return INext(outof) < n;
        }
        else
        {
            return false;
        }
    }

    bool Rand::Chance(float n, float outof) {
        if (outof > 0)
        {
            return Next() < n / outof;
        }
        else
        {
            return false;
        }
    }


}