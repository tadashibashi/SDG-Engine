/* =============================================================================
 * Rand
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <random>
#include <memory>
namespace SDG
{

    class Rand {
    public:
        static void Init();
        static double Next();
        static double Next(double n);
        static int INext(int n);

        static double Range(double low, double high);

        static int IRange(int low, int high);
    private:
        static std::random_device s_device_;
        static std::mt19937 s_mt_;
        static std::uniform_real_distribution<double> s_dist_;
    };

}



