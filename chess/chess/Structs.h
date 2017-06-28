#pragma once

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


template <typename T, T defaultValue>
struct Matrix3D
{
    Matrix3D(size_t mX, size_t mY, size_t mZ)
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

template <typename T>
struct Matrix2D
{
    Matrix2D(size_t mX, size_t mY)
        : mX(mX)
        , mY(mY)
        , v(mX * mY, T())
    {}

    T & operator () (const Point & p)
    {
        return (*this)(p.x, p.y);
    }

    const T & operator () (const Point & p) const
    {
        return (*this)(p.x, p.y);
    }

    T & operator () (size_t x, size_t y)
    {
        auto pos = getPos(x, y);
        if (pos >= v.size())
        {
            throw std::logic_error("try to access undefined point " + std::to_string(x) + " " + std::to_string(y));
        }

        return v[pos];
    }

    const T & operator () (size_t x, size_t y) const
    {
        auto pos = getPos(x, y);
        if (pos >= v.size())
        {
            throw std::logic_error("try to access undefined point " + std::to_string(x) + " " + std::to_string(y));
        }

        return v[pos];
    }

    size_t hasPoint(size_t x, size_t y) const
    {
        return x < width() && y < height();
    }

    size_t hasPoint(const Point & p) const
    {
        return hasPoint(p.x, p.y);
    }

    size_t getPos(size_t x, size_t y) const
    {
        return y * mX + x;
    }

    size_t height() const
    {
        return mY;
    }

    size_t width() const
    {
        return mX;
    }

    std::vector<Point> getClosest(const Point & p)
    {
        auto possibleNeibs = std::vector<Point>{
            { p.x, p.y - 1 },
            { p.x, p.y + 1 },
            { p.x + 1, p.y },
            { p.x - 1, p.y },
            { p.x - 1, p.y - 1 },
            { p.x - 1, p.y + 1 },
            { p.x + 1, p.y - 1 },
            { p.x + 1, p.y + 1 }
        };
        std::vector<Point> neibs;
        neibs.reserve(8);
        for (auto const& neib : possibleNeibs)
        {
            if (hasPoint(neib))
            {
                neibs.push_back(neib);
            }
        }

        return neibs;
    }

    std::vector<T> v;
    size_t mX, mY;
};