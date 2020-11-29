#pragma once
#include <type_traits>
#include <Engine/Core.h>

namespace SDG
{
    template <typename T> requires std::is_arithmetic_v<T>
    class SDG_API Rect_
    {
    public:
        Rect_() : x(0), y(0), w(0), h(0) { }
        Rect_(T x, T y, T w, T h) : x(x), y(y), w(w), h(h) { }

        // data members
        T x, y, w, h;

        void Set(T x, T y, T w, T h) noexcept
        {
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }

        T GetArea() const noexcept
        {
            return w * h;
        }

        T GetLeft() const noexcept
        {
            return x;
        }

        T GetRight() const noexcept
        {
            return x + w;
        }

        T GetTop() const noexcept
        {
            return y;
        }

        T GetBottom() const noexcept
        {
            return y + h;
        }

        bool IsEmpty() const noexcept
        {
            return (*this == Rect_());
        }

        bool operator==(const Rect_ &other)
        {
            return (x == other.x && y == other.y &&
                    w == other.w && h == other.y);
        }

        bool operator!=(const Rect_ &other)
        {
            return !(*this == other);
        }

        template<typename U> requires std::is_arithmetic_v<U> &&
            std::is_convertible_v<U, T>
        bool Intersects(const Rect_<U> &other)
        {
            T topA(GetTop()), bottomA(GetBottom()), 
                leftA(GetLeft()), rightA(GetRight()),
                topB(other.GetTop()), bottomB(other.GetBottom()), 
                leftB(other.GetLeft()), rightB(other.GetRight());
            return !(topB > bottomA || bottomB < topA || rightB < leftA || leftB > rightA);
        }

        // Convertible by casting to other _Rect types.
        template<typename U> requires 
            std::is_arithmetic_v<U> &&
            std::is_convertible_v<U, T>
        explicit operator Rect_<U>()
        {
            return Rect_<U> (
                static_cast<U>(x),
                static_cast<U>(y),
                static_cast<U>(w),
                static_cast<U>(h));
        }
    };

    typedef Rect_<float> FRectangle;
    typedef Rect_<int>   Rectangle;

    template class Rect_<float>;
    template class Rect_<int>;
}

