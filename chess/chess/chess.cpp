#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

istringstream getLineStrm(istream & in)
{
    string str;
    getline(in, str);
    return istringstream(str);
}

enum Item
{
    Free = '.',
    Rook = 'R',
    Bishop = 'B',
    Knight = 'K',
    King = '*'
};

struct Data
{
    Data(size_t width, size_t height)
        : width(width)
        , height(height)
    {
        vals.reserve(width * height);
    }

    Item & operator () (size_t x, size_t y)
    {
        if (!hasPoint(x, y))
        {
            throw logic_error("try to access undefined point " + to_string(x) + " " + to_string(y));
        }

        return vals[y * width + x];
    }

    const Item & operator () (size_t x, size_t y) const
    {
        if (!hasPoint(x, y))
        {
            throw logic_error("try to access undefined point " + to_string(x) + " " + to_string(y));
        }

        return vals[y * width + x];
    }

    bool hasPoint(size_t x, size_t y) const
    {
        return (x >= 0 && x < width) && (y >= 0 && y < height);
    }

    vector<Item> vals;
    size_t width, height;
};

Data readData(istream & in)
{
    size_t rows, columns;

    auto strm = getLineStrm(in);
    if (!(strm >> rows >> columns) || rows <= 0 || columns <= 0)
    {
        throw invalid_argument("invalid data size");
    }

    auto d = Data(columns, rows);
    for (size_t i = 0; i < rows && !in.eof(); ++i)
    {
        char ch;
        size_t j = 0;
        auto s = getLineStrm(in);
        while (j < columns && !s.eof())
        {
            if (s >> ch)
            {
                d.vals.push_back(Item(ch));
                ++j;
            }
        }

        if (j != columns)
        {
            throw invalid_argument("not enougth data on row " + to_string(i + 1));
        }
    }

    return d;
}

void printStepsCount(Data & d, ostream & out)
{
    for (size_t i = 0; i < d.height; ++i)
    {
        for (size_t j = 0; j < d.width; ++j)
        {
            out << char(d(j, i));
        }
        out << endl;
    }
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto d = readData(in);
        printStepsCount(d, out);
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

