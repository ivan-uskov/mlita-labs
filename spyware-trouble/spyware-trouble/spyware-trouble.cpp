#include "stdafx.h"
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <list>
#include <numeric>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

struct Point
{
    Point(unsigned x, unsigned y)
        :x(x), y(y)
    {}

    Point operator + (const Point & rhs)
    {
        return Point(x + rhs.x, y + rhs.y);
    }

    bool operator != (const Point & rhs)
    {
        return !(x == rhs.x && y == rhs.y);
    }

    int x;
    int y;
};

struct Val
{
    Val(bool isPsyho = false)
        : isPsyho(isPsyho)
    {}

    bool isPsyho;
};

struct Matrix
{
    Matrix(unsigned width, unsigned height, unsigned pointLength)
        : width(width)
        , height(height)
        , pointLength(pointLength)
    {
        vals.reserve(width * height);
    }

    Val & operator () (Point p)
    {
        return vals[p.y * width + p.x];
    }

    const Val & operator () (Point p) const
    {
        return vals[p.y * width + p.x];
    }

    const Val & operator () (unsigned x, unsigned y) const
    {
        return (*this)(Point(x, y));
    }

    int width, height, pointLength;
    std::vector<Val> vals;
};

auto getLineStrm(istream & in)
{
    string str;
    getline(in, str);
    return istringstream(str);
}

Matrix readMatrix(istream & in)
{
    unsigned width, height, pointLength;

    {
        auto strm = getLineStrm(in);
        if (!(strm >> height >> width >> pointLength) || (width * height * pointLength == 0))
        {
            throw invalid_argument("invalid arguments");
        }
    }

    Matrix mtx(width + 2, height + 2, pointLength);
    auto lineOfEmptyPoints = vector<Val>(mtx.width, Val());

    copy(lineOfEmptyPoints.begin(), lineOfEmptyPoints.end(), back_inserter(mtx.vals));
    while (!in.eof())
    {
        mtx.vals.push_back(Val());
        {
            int item;
            auto strm = getLineStrm(in);
            while (!strm.eof() && (strm >> item))
            {
                mtx.vals.push_back(Val(item == 1));
            }
        }
        mtx.vals.push_back(Val());
    }
    copy(lineOfEmptyPoints.begin(), lineOfEmptyPoints.end(), back_inserter(mtx.vals));

    if (mtx.vals.size() != mtx.width * mtx.height)
    {
        throw invalid_argument("not enought data");
    }

    return mtx;
}


Point getFirstPoint(Matrix & mtx)
{
    for (int y = 0; y < mtx.height; ++y)
    {
        for (int x = 0; x < mtx.width; ++x)
        {
            if (mtx(x, y).isPsyho)
            {
                return Point(x - 1, y);
            }
        }
    }

    throw logic_error("empty field");
}

float calcPathDistance(Matrix & mtx)
{
    

    auto right = [](Point p) {
        return Point(-p.y, p.x);
    };

    auto left = [](Point p) {
        return Point(p.y, -p.x);
    };

    auto nearPsyho = [&](Point p, Point direction) {
        return mtx(p + right(direction)).isPsyho;
    };

    auto topP = [](Point p, Point direction) {
        return p + direction;
    };

    auto topRightP = [&topP, &right](Point p, Point direction) {
        return topP(p, direction) + right(direction);
    };

    auto first = getFirstPoint(mtx);
    auto curr = first;
    list<Point> path;
    auto direction = Point(0, -1);

    float distance = 0;
    auto go = [&curr, &path, &mtx, &distance] (Point next) {
        if (abs(curr.x - next.x) + abs(curr.y - next.y) == 1)
        {
            distance += mtx.pointLength;
        }
        else
        {
            distance += (1.f / 4) * float(M_PI) * mtx.pointLength + mtx.pointLength;
        }

        path.push_back(curr);
        curr = next;
    };

    do
    {
        {
            auto next = topP(curr, direction);
            if (mtx(next).isPsyho)
            {
                direction = left(direction);
                continue;
            }
            else if (nearPsyho(next, direction))
            {
                go(next);
                continue;
            }
        }

        {
            auto next = topRightP(curr, direction);
            if (nearPsyho(next, right(direction)))
            {
                direction = right(direction);
                go(next);
                continue;
            }
        }

        throw logic_error("some wents wrong");

    } while (curr != first);

    return roundf(distance * 10) / 10;
}

int main()
{
    try
    {
        ifstream in("input.txt");
        ofstream out("output.txt");

        auto mtx = readMatrix(in);
        out << calcPathDistance(mtx) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}