#pragma once
#include <Engine/Core.h>
#include <type_traits>
#include <cmath>
#include <sstream>
#include <string>

namespace SDG
{
    template <typename T> requires std::is_arithmetic_v<T>
    class Vec2_
    {
    public:
        Vec2_() : x(0), y(0) { }
        Vec2_(T x, T y) : x(x), y(y) { }

        union
        {
            struct { T x, y; };
            // Width and Height. Not to be confused with the xyzw paradigm.
            struct { T w, h; };
        };

        // Gets the distance between two vectors
        static float Distance(const Vec2_ &p1, const Vec2_ &p2)
        {
            float a = static_cast<float>(p1.x) - static_cast<float>(p2.x);
            float b = static_cast<float>(p1.y) - static_cast<float>(p2.y);

            return std::sqrt(a * a + b * b);
        }

        // Returns a rotated point around {0, 0} anchor
        static Vec2_ Rotate(const Vec2_ &v, float angle)
        {
            return Vec2_((T)(v.x * std::cos(angle) - v.y * std::sin(angle)),
                         (T)(v.x * std::sin(angle) + v.y * std::cos(angle)));
        }

        // Distance from zero.
        float Length() const
        {
            return std::sqrt((float)(x * x + y * y));
        }

        // Scales x and y so that it totals 1 in length
        // (Only applicable on floating-point types)
        Vec2_ &Normalize()
        {
            T length = (T)this->Length();
            if (length != 0)
            {
                x /= length;
                y /= length;
            }
            return *this;
        }

        // Formats Vec2_ as string: "{x, y}"
        std::string ToString() const
        {
            std::ostringstream str;
            str << *this;
            return str.str();
        }

        // Math operators
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
    std::ostream &operator<<(std::ostream &out, const Vec2_<T> &v)
    {
        out << "{" << std::to_string(v.x) << ", " << std::to_string(v.y) << "}";
        return out;
    }

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
