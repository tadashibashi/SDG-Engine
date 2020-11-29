#pragma once
#include <Engine/Core.h>
#include <type_traits>
#include <cmath>

namespace SDG
{
    template <typename T> requires std::is_arithmetic_v<T>
    class SDG_API Vec2_
    {
    public:
        Vec2_() : x(0), y(0) { }
        Vec2_(T x, T y) : x(x), y(y) { }

        union
        {
            struct { T x, y; };
            struct { T w, h; };
        };

        static float Distance(const Vec2_ &p1, const Vec2_ &p2)
        {
            float a = static_cast<float>(p1.x) - static_cast<float>(p2.x);
            float b = static_cast<float>(p1.y) - static_cast<float>(p2.y);

            return std::sqrt(a * a + b * b);
        }

        // Distance from zero.
        float Length()
        {
            return std::sqrt(x * x + y * y);
        }

        // Only applicable if template type is a decimal/floating-point type. 
        Vec2_ &Normalize()
        {
            float length = this->Length();
            if (length != 0)
            {
                x /= length;
                y /= length;
            }
            return *this;
        }

        Vec2_ &operator+=(const Vec2_ &other)
        {
            x += other.x;
            y += other.y;
            return *this;
        }          
        
        Vec2_ &operator-=(const Vec2_ &other)
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }        
        
        Vec2_ &operator*=(const Vec2_ &other)
        {
            x *= other.x;
            y *= other.y;
            return *this;
        }   

        Vec2_ &operator/=(const Vec2_ &other)
        {
            x /= other.x;
            y /= other.y;
            return *this;
        }

        template <typename U> requires
            std::is_arithmetic_v<U>
        Vec2_ &operator*=(const U scalar)
        {
            x *= scalar;
            y *= scalar;
            return *this;
        }        
        
        template <typename U> requires
            std::is_arithmetic_v<U>
        Vec2_ &operator/=(const U scalar)
        {
            x /= scalar;
            y /= scalar;
            return *this;
        }

        bool operator==(const Vec2_ &other)
        {
            return (x == other.x && y == other.y);
        }

        bool operator!=(const Vec2_ &other)
        {
            return !operator==(other);
        }

        template <typename U> requires 
            std::is_arithmetic_v<U> &&
            std::is_convertible_v<T, U>
        explicit operator Vec2_<U>()
        {
            return Vec2_<U>(
                static_cast<U>(x),
                static_cast<U>(y));
        }
    };

    template <typename T>
    Vec2_<T> operator+(Vec2_<T> a, Vec2_<T> b)
    {
        Vec2_<T> temp(a);
        return temp += b;
    }    
    
    template <typename T>
    Vec2_<T> operator-(const Vec2_<T> a, const Vec2_<T> b)
    {
        Vec2_<T> temp(a);
        return temp -= b;
    }    
    
    template <typename T>
    Vec2_<T> operator*(const Vec2_<T> a, const Vec2_<T> b)
    {
        Vec2_<T> temp(a);
        return temp *= b;
    }    
    
    template <typename T>
    Vec2_<T> operator/(const Vec2_<T> a, const Vec2_<T> b)
    {
        Vec2_<T> temp(a);
        return temp /= b;
    }    
    
    template <typename T, typename U> requires 
        std::is_arithmetic_v<U>
    Vec2_<T> operator/(const Vec2_<T> v, U scalar)
    {
        Vec2_<T> temp(v);
        return temp /= scalar;
    }    
    
    template <typename T, typename U> requires 
        std::is_arithmetic_v<U>
    Vec2_<T> operator*(const Vec2_<T> v, U scalar)
    {
        Vec2_<T> temp(v);
        return temp *= scalar;
    }

    typedef Vec2_<float> Vector2;
    typedef Vec2_<int>   Point;

    template class Vec2_<float>;
    template class Vec2_<int>;
}
