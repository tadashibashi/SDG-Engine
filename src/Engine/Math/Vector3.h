#pragma once
#include <Engine/Core.h>
#include <type_traits>
#include <cmath>

namespace SDG
{
    template <typename T> requires std::is_arithmetic_v<T>
    class  Vec3_
    {
    public:
        Vec3_() : x(0), y(0), z(0) { }
        Vec3_(T x, T y, T z) : x(x), y(y), z(z) { }

        union
        {
            struct { T x, y, z; };
            struct { T r, g, b; };
        };

        static double Distance(const Vec3_ &p1, const Vec3_ &p2)
        {
            double a = static_cast<double>(p1.x) - static_cast<double>(p2.x);
            double b = static_cast<double>(p1.y) - static_cast<double>(p2.y);
            double c = static_cast<double>(p1.z) - static_cast<double>(p2.z);

            return std::sqrt(a * a + b * b + c * c);
        }

        // Distance from zero.
        double Length()
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Only applicable if template type is a decimal/floating-point type. 
        void Normalize()
        {
            double length = this->Length();
            if (length != 0)
            {
                x /= length;
                y /= length;
                z /= length;
            }
        }

        Vec3_ &operator+=(const Vec3_ &other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        Vec3_ &operator-=(const Vec3_ &other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            return *this;
        }

        Vec3_ &operator*=(const Vec3_ &other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        Vec3_ &operator/=(const Vec3_ &other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        }

        bool operator==(const Vec3_ &other)
        {
            return (x == other.x &&
                    y == other.y &&
                    z == other.z);
        }

        bool operator!=(const Vec3_ &other)
        {
            return !(*this == other);
        }

        template <typename U> requires
            std::is_arithmetic_v<U>
        Vec3_ &operator*=(const U scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        template <typename U> requires
            std::is_arithmetic_v<U>
            Vec3_ &operator/=(const U scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        template <typename U> requires
            std::is_arithmetic_v<U> &&
            std::is_convertible_v<T, U>
            explicit operator Vec3_<U>()
        {
            return Vec3_<U>(
                static_cast<U>(x),
                static_cast<U>(y),
                static_cast<U>(z));
        }
    };

    template <typename T>
    Vec3_<T> operator+(Vec3_<T> a, Vec3_<T> b)
    {
        return a += b;
    }

    template <typename T>
    Vec3_<T> operator-(const Vec3_<T> a, const Vec3_<T> b)
    {
        return a -= b;
    }

    template <typename T>
    Vec3_<T> operator*(const Vec3_<T> a, const Vec3_<T> b)
    {
        return a *= b;
    }

    template <typename T>
    Vec3_<T> operator/(const Vec3_<T> a, const Vec3_<T> b)
    {
        return a /= b;
    }

    template <typename T, typename U> requires
        std::is_arithmetic_v<U>
        Vec3_<T> operator/(const Vec3_<T> v, U scalar)
    {
        return v /= scalar;
    }        
        
    template <typename T, typename U> requires
        std::is_arithmetic_v<U>
        Vec3_<T> operator*(const Vec3_<T> v, U scalar)
    {
        return v *= scalar;
    }

    typedef Vec3_<float> Vector2;
    typedef Vec3_<int>   Point;

    template class Vec3_<float>;
    template class Vec3_<int>;
}
