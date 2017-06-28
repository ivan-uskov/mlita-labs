#pragma once
#include "Data.h"
#include "Utils.h"
#include <queue>

struct KillCache : public Matrix3D<bool, false>
{
    KillCache(Data const& d)
        : Matrix3D<bool, false>(d.width(), d.height(), size_t(std::pow(size_t(2), d.enemies.size())))
        , d(d)
    {
        Utils::iterateCortages(d.enemies.size(), [&](size_t i) {
            for (size_t j = 0; j < d.enemies.size(); ++j)
            {
                if (Utils::makeBinary(j) & i)
                {
                    auto const& enemyPos = d.enemies[j].point;
                    (*this)(enemyPos.x, enemyPos.y, i) = true;
                }
            }

            std::queue<Point> killList;
            for (size_t j = 0; j < d.enemies.size(); ++j)
            {
                if (Utils::makeBinary(j) & i)
                {
                    add(killList, d.enemies[j], i);
                }
            }

            while (!killList.empty())
            {
                auto p = killList.front();
                (*this)(p.x, p.y, i) = true;
                killList.pop();
            }
        });
    }

    void add(std::queue<Point> & l, Enemy const& e, size_t i)
    {
        switch (e.type)
        {
        case Type::Rook:
            addRook(l, e.point, i);
            break;
        case Type::Bishop:
            addBishop(l, e.point, i);
            break;
        case Type::Knight:
            addKnight(l, e.point);
            break;
        }
    }

    void addRook(std::queue<Point> & l, const Point & p, size_t i)
    {
        auto directions = std::vector<SignedPoint>{
            { 1, 0 },
            { -1, 0 },
            { 0, 1 },
            { 0, -1 }
        };

        for (auto & direction : directions)
        {
            auto current = p + direction;
            while (d.hasPoint(current) && !(*this)(current.x, current.y, i))
            {
                l.push(current);
                current = current + direction;
            }
        }
    }

    void addBishop(std::queue<Point> & l, const Point & p, size_t i)
    {
        auto directions = std::vector<SignedPoint>{
            { 1, 1 },
            { -1, 1 },
            { -1, -1 },
            { 1, -1 }
        };

        for (auto & direction : directions)
        {
            auto current = p + direction;
            while (d.hasPoint(current) && !(*this)(current.x, current.y, i))
            {
                l.push(current);
                current = current + direction;
            }
        }
    }

    void addKnight(std::queue<Point> & l, const Point & p)
    {
        auto killList = std::vector<Point>{
            { p.x + 1, p.y - 2 }, { p.x + 2, p.y - 1 },
            { p.x + 2, p.y + 1 }, { p.x + 1, p.y + 2 },
            { p.x - 1, p.y + 2 }, { p.x - 2, p.y + 1 },
            { p.x - 2, p.y - 1 }, { p.x - 1, p.y - 2 }
        };

        for (auto & point : killList)
        {
            if (d.hasPoint(point))
            {
                l.push(point);
            }
        }
    }

    Data const& d;
};