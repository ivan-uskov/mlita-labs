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

constexpr size_t MAX_ENEMIES = 14;

struct Data : public Matrix2D<Type, Type::Free>
{
    Data(size_t width, size_t height)
        : Matrix2D<Type, Type::Free>(width, height)
    {
        enemies.reserve(MAX_ENEMIES);
    }

    Point start;
    std::vector<Enemy> enemies;
};

std::ostream & operator << (std::ostream & out, Data const& d);