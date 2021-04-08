/* =============================================================================
 * Math
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Core.h>
#include <cmath>
#include "Vector2.h"
#include <initializer_list>
#include <algorithm>

namespace SDG
{
    const float PI = 3.1415926535897932f;
    const float RAD_TO_DEG = 180.f / PI;
    const float DEG_TO_RAD = PI / 180.f;

    class Math
    {
    public:
        template <typename T> requires std::is_floating_point_v<T>
        static T Lerp(T val, T dest, T amt)
        {
            return (dest - val) * amt + val;
        }

        static Vector2 Lerp(Vector2 val, Vector2 dest, float amt)
        {
            return Vector2(Lerp(val.x, dest.x, amt), Lerp(val.y, dest.y, amt));
        }

        template <typename T> requires std::is_floating_point_v<T>
        static T RadToDeg(T rad)
        {
            return rad * RAD_TO_DEG;
        }

        template <typename T> requires std::is_floating_point_v<T>
        static T DegToRad(T degrees)
        {
            return degrees * DEG_TO_RAD;
        }

        template <typename T> requires std::is_floating_point_v<T>
        static float TrajectoryX(T degrees, T length)
        {
            return std::cos(DegToRad(degrees)) * length;
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static float TrajectoryY(T degrees, T length)
        {
            // Negative value to match downward y coordinate system
            return -(std::sin(DegToRad(degrees)) * length);
        }

        /**
         * Get the resulting Vector from an angle and length
         * @param degrees
         * @param length
         * @return
         */
        static Vector2 Trajectory(float degrees, float length)
        {
            return Vector2(TrajectoryX(degrees, length), TrajectoryY(degrees, length));
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Clamp(T value, T min, T max)
        {
            return Max(Min(value, max), min);
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Min(T a, T b)
        {
            return a < b ? a : b;
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Min(const std::initializer_list<T> &nums)
        {
            return std::min(nums);
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Max(T a, T b)
        {
            return a > b ? a : b;
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Max(const std::initializer_list<T> &nums)
        {
            return std::max(nums);
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Abs(T n)
        {
            return n < 0 ? -n : n;
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Sign(T n)
        {
            return (T)(n < 0 ? -1 : 1);
        }

        template <typename T> requires std::is_arithmetic_v<T>
        static T Add(std::initializer_list<T> num)
        {
            T total = 0;
            for (auto n: num)
            {
                total += n;
            }

            return total;
        }

        /**
        * Modulo function that does not reflect across 0
        */
        template <typename T> requires std::is_arithmetic_v<T>
        static float Mod(T x, T n)
        {
            return (T)fmodf((fmodf((float)x, (float)n) + n), (float)n);
        }

        /**
         * Return a number that 'wraps around' to the opposite boundary when either boundary is exceeded.
         * @param x Number to wrap.
         * @param n1 First boundary; can be higher or lower than n2.
         * @param n2 Second boundary; can be higher or lower than n1.
         */
        template <typename T> requires std::is_arithmetic_v<T>
        static T Wrap(T x, T n1, T n2)
        {
            if (n1 == n2) { return n1; }

            if (n1 < n2) {
                return Mod(x - n1, n2 - n1) + n1;
            } else {
                return Mod(x - n2, n1 - n2) + n2;
            }
        }

        static Vector2 Wrap(Vector2 val, Vector2 low, Vector2 high)
        {
            return Vector2(Wrap(val.x, low.x, high.x), Wrap(val.y, low.y, high.y));
        }

        static float PointDirection(float x1, float y1, float x2, float y2)
        {
            float diffX = x2 - x1;
            float diffY = y2 - y1;
            int quadrant = GetQuadrant(diffX, diffY);

            if (quadrant % 2 == 0)
            { // if quadrants 0 or 2
                return std::abs(RadToDeg(std::atan(diffY / diffX))) + (float)quadrant * 90;
            }
            else
            {   // quadrants 1 or 3
                return 90-std::abs(RadToDeg(std::atan((diffY / diffX)))) + (float)quadrant * 90;
            }
        }

    private:
        /**
         * Returns the quadrant the coordinate is in starting with quadrant zero
         * on the bottom right, going clockwise until quadrant 3 on the top-right.
         * This function is a helper used by point direction
         */
        static int GetQuadrant(float x, float y)
        {
            if (x > 0 && y >= 0) {
                return 3;
            } else if (x <= 0 && y > 0) {
                return 2;
            } else if (x < 0 && y <= 0) {
                return 1;
            } else if (x >= 0 && y < 0) {
                return 0;
            }
            return -1;
        }

    };
}