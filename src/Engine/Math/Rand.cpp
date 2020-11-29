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
    std::uniform_real_distribution<double> Rand::s_dist_(0.0, 1.0);

    void Rand::Init()
    {

    }

    double Rand::Next()
    {
        return s_dist_(s_mt_);
    }

    double Rand::Next(double n)
    {
        return Next() * n;
    }

    int Rand::INext(int n)
    {
        return static_cast<int>(Next(n));
    }

    double Rand::Range(double low, double high)
    {
        SDG_ASSERT(low <= high);

        double diff = high - low;
        return Next() * diff + low;
    }

    int Rand::IRange(int low, int high)
    {
        return static_cast<int>(Range(low, high));
    }


}