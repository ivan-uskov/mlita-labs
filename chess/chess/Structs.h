#pragma once

struct Point
{
    Point() = default;
    Point(size_t x, size_t y)
        :x(x), y(y)
    {}

    Point(std::initializer_list<size_t> l)
    {
        auto it = l.begin();
        x = *(it++);
        y = *it;
    }

    size_t x = std::numeric_limits<size_t>::max();
    size_t y = std::numeric_limits<size_t>::max();
};


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

template <typename T, T defaultValue>
struct Matrix2D
{
    Matrix2D(size_t mX, size_t mY)
        : mX(mX)
        , mY(mY)
        , v(mX * mY, defaultValue)
    {}

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
        return getPos(x, y) < v.size();
    }

    size_t hasPoint(const Point & p) const
    {
        return getPos(p.x, p.y) < v.size();
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
            { x, y - 1 },
            { x, y + 1 },
            { x + 1, y },
            { x - 1, y },
            { x - 1, y - 1 },
            { x - 1, y + 1 },
            { x + 1, y - 1 },
            { x + 1, y + 1 }
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