#include "stdafx.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <map>
#include <list>
#include <set>
#include <numeric>

using namespace std;

struct Point
{
    Point() = default;
    Point(unsigned x, unsigned y)
        :x(x),y(y)
    {}

    unsigned x = std::numeric_limits<unsigned>::max();
    unsigned y = std::numeric_limits<unsigned>::max();
};


struct Val
{
    Val(int weidth)
        : weight(weidth)
    {}

    int weight;
    int score = std::numeric_limits<int>::max();
    Point from;
};

struct Matrix
{
    Matrix(unsigned width, unsigned height)
        : width(width)
        , height(height)
    {
        vals.reserve(width * height);
    }

    set<pair<unsigned, unsigned>> getNeibs(unsigned x, unsigned y) const
    {
        set<pair<unsigned, unsigned>> neibs;
        if (hasPoint(x, y - 1))
        {
            neibs.emplace(x, y - 1);
        }
        if (hasPoint(x + 1, y))
        {
            neibs.emplace(x + 1, y);
        }
        if (hasPoint(x, y + 1))
        {
            neibs.emplace(x, y + 1);
        }
        if (hasPoint(x - 1, y))
        {
            neibs.emplace(x - 1, y);
        }

        return neibs;
    }

    Val & operator () (unsigned x, unsigned y)
    {
        if (!hasPoint(x, y))
        {
            throw logic_error("try to access undefined point");
        }

        return vals[y * width + x];
    }

    const Val & operator () (unsigned x, unsigned y) const
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
    std::vector<Val> vals;
};

Matrix readMatrix(istream & in)
{
    unsigned width, height;

    string str;
    getline(in, str);
    istringstream strm(str);

    if (!(strm >> height >> width) || (width * height == 0))
    {
        throw invalid_argument("invalid width height");
    }

    Matrix mtx(width, height);

    while (!in.eof())
    {
        int item;
        getline(in, str);
        istringstream strm(str);
        while (!strm.eof() && (strm >> item))
        {
            mtx.vals.push_back(Val(item));
        }
    }

    if (mtx.vals.size() != width * height)
    {
        throw invalid_argument("not enought data");
    }

    return mtx;
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto mtx = readMatrix(in);
        auto current = Point{ 0, 0 };
        mtx(0, 0).score = mtx(0, 0).weight;

        for (unsigned y = 0; y < mtx.height; ++y)
        {
            for (unsigned x = 0; x < mtx.width; ++x)
            {
                if (mtx.hasPoint(x + 1, y) && (mtx(x + 1, y).score > (mtx(x, y).score + mtx(x + 1, y).weight)))
                {
                    mtx(x + 1, y).score = mtx(x, y).score + mtx(x + 1, y).weight;
                    mtx(x + 1, y).from = { x, y };
                }

                if (mtx.hasPoint(x, y + 1) && (mtx(x, y + 1).score > (mtx(x, y).score + mtx(x, y + 1).weight)))
                {
                    mtx(x, y + 1).score = mtx(x, y).score + mtx(x, y + 1).weight;
                    mtx(x, y + 1).from = { x, y };
                }
            }
        }

        int sum = 0;
        list<Point> path;
        Point curr = { mtx.width - 1, mtx.height - 1 };
        while (mtx.hasPoint(curr.x, curr.y))
        {
            sum += mtx(curr.x, curr.y).weight;
            path.emplace_front(curr);
            curr = mtx(curr.x, curr.y).from;
        }

        out << sum << endl;
        for (auto item : path)
        {
            out << item.y + 1 << " " << item.x + 1<< endl;
        }
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}