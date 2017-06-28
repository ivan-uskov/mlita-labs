#pragma once
#include "Structs.h"

enum Type
{
    Free = '.',
    Rook = 'R',
    Bishop = 'B',
    Knight = 'K',
    King = '*'
};

struct Enemy
{
    Type type;
    Point point;
};

struct DataItem
{
    DataItem() = default;
    DataItem(Type t, size_t id)
        : type(t)
        , id(id)
    {}

    Type type;
    size_t id;
};

bool isEnemy(Type t);
constexpr size_t MAX_ENEMIES = 14;

struct Data : public Matrix2D<DataItem>
{
    Data(size_t width, size_t height)
        : Matrix2D<DataItem>(width, height)
    {
        enemies.reserve(MAX_ENEMIES);
    }

    Point start;
    std::vector<Enemy> enemies;
};

std::ostream & operator << (std::ostream & out, Data const& d);