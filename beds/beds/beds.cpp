#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <map>
#include <set>

using namespace std;

enum Val
{
    Bad = '#',
    None = '.'
};

struct Matrix
{
    Matrix(unsigned width, unsigned height)
        : width(width)
        , height(height)
    {
        vals.reserve(width * height);
    }

    set<pair<unsigned, unsigned>> getNeib(unsigned x, unsigned y) const
    {
        set<pair<unsigned, unsigned>> neibs;
        if (hasPoint(x, y - 1) && ((*this)(x, y - 1) == Val::Bad))
        {
            neibs.emplace(x, y - 1);
        }
        if (hasPoint(x + 1, y) && ((*this)(x + 1, y) == Val::Bad))
        {
            neibs.emplace(x + 1, y);
        }
        if (hasPoint(x, y + 1) && ((*this)(x, y + 1) == Val::Bad))
        {
            neibs.emplace(x, y + 1);
        }
        if (hasPoint(x - 1, y) && ((*this)(x - 1, y) == Val::Bad))
        {
            neibs.emplace(x - 1, y);
        }

        return neibs;
    }

    char & operator () (unsigned x, unsigned y)
    {
        if (!hasPoint(x, y))
        {
            throw logic_error("try to access undefined point");
        }

        return vals[y * width + x];
    }

    const char & operator () (unsigned x, unsigned y) const
    {
        if (!hasPoint(x, y))
        {
            throw logic_error("try to access undefined point");
        }

        return vals[y * width + x];
    }

    bool hasPoint(unsigned x, unsigned y) const
    {
        return (x >= 0 && x < width) && (y >= 0 && y < height);
    }

    unsigned width, height;
    std::string vals;
};

Matrix readMatrix(istream & in)
{
    unsigned width, height;

    if (!(in >> height >> width) || (width * height == 0))
    {
        throw invalid_argument("invalid width height");
    }

    Matrix mtx(width, height);

    string str;

    while (!in.eof())
    {
        getline(in, str);
        mtx.vals += str;
    }

    if (mtx.vals.size() != width * height)
    {
        throw invalid_argument("not enought data");
    }

    return mtx;
}

void fillFrom(Matrix & mtx, unsigned x, unsigned y)
{
    if (mtx(x, y) == Val::Bad)
    {
        mtx(x, y) = Val::None;
        auto neibs = mtx.getNeib(x, y);
        for (auto neib : neibs)
        {
            fillFrom(mtx, neib.first, neib.second);
        }
    }
}

unsigned getBedsCount(Matrix & mtx)
{
    unsigned count = 0;
    while (1)
    {
        auto pos = mtx.vals.find(Val::Bad);
        if (pos >= mtx.vals.size())
        {
            break;
        }

        unsigned y = pos / mtx.width;
        unsigned x = pos - y * mtx.width;

        ++count;

        fillFrom(mtx, x, y);
    }
    return count;
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto mtx = readMatrix(in);
        out << getBedsCount(mtx) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}