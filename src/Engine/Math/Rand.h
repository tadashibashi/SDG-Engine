/* =============================================================================
 * Rand
 * 
 * 
 * ===========================================================================*/
#pragma once
#ifndef SDG_MATH_H
#define SDG_MATH_H

#include <random>
#include <memory>
namespace SDG
{
    // Static class for randomization functions
    class Rand {
    public:
        static float Next();
        static float Next(float n);
        static int INext(int n);

        static float Range(float low, float high);
        static int IRange(int low, int high);

        static bool Chance(int n, int outof);
        static bool Chance(float n, float outof);

        template <typename T>
        static T Choose(const std::initializer_list<T> &list)
        {
            auto it = list.begin();
            int index = INext((int)list.size());
            for (int i = 0; i < index; ++it, ++i);

            return *it;
        }

    private:
        static std::random_device s_device_;
        static std::mt19937 s_mt_;
        static std::uniform_real_distribution<float> s_dist_;
    };

}

#endif
