#pragma once
#include "Data.h"
#include "Utils.h"

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
                    auto const& enemy = d.enemies[j];
                    add(enemy, i);
                }
            }
        });
    }

    void add(Enemy const& e, size_t i)
    {
        switch (e.type)
        {
        case Type::Rook:
            addRook(e.point, i);
            break;
        case Type::Bishop:
            addBishop(e.point, i);
            break;
        case Type::Knight:
            addKnight(e.point, i);
            break;
        }
    }

    void addRook(const Point & p, size_t i)
    {

    }

    void addBishop(const Point & p, size_t i)
    {

    }

    void addKnight(const Point & p, size_t i)
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
                v[getPos(point.x, point.y, i)] = true;
            }
        }
    }

    Data const& d;
};