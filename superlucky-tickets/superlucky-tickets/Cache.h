#pragma once
#include <vector>

template <typename T>
struct BasePoint
{
    BasePoint() = default;
    BasePoint(T x, T y)
        :x(x), y(y)
    {}

    BasePoint(std::initializer_list<T> l)
    {
        auto it = l.begin();
        x = *(it++);
        y = *it;
    }

    T x = std::numeric_limits<T>::max();
    T y = std::numeric_limits<T>::max();
};

using Point = BasePoint<size_t>;
using SignedPoint = BasePoint<int>;

template <typename LT, typename RT>
LT operator + (const LT & l, const RT & r)
{
    return LT(l.x + r.x, l.y + r.y);
}


template <typename T>
struct Matrix3D
{
    Matrix3D(size_t mX, size_t mY, size_t mZ, T defaultValue)
        : mX(mX)
        , mY(mY)
        , mZ(mZ)
        , v(mX * mY * mZ, defaultValue)
    {}

    typename std::vector<T>::reference operator () (size_t x, size_t y, size_t z)
    {
        auto pos = getPos(x, y, z);
        if (pos >= v.size())
        {
            throw std::logic_error("try to access undefined point " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));
        }

        return v[pos];
    }

    typename std::vector<T>::const_reference operator () (size_t x, size_t y, size_t z) const
    {
        auto pos = getPos(x, y, z);
        if (pos >= v.size())
        {
            throw std::logic_error("try to access undefined point " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));
        }

        return v[pos];
    }

    size_t hasPoint(size_t x, size_t y, size_t z) const
    {
        return getPos(x, y, z) < v.size();
    }

    size_t getPos(size_t x, size_t y, size_t z) const
    {
        return z * mY * mX + y * mX + x;
    }

    std::vector<T> v;
    size_t mX, mY, mZ;
};
