#include "stdafx.h"
#include "Reader.h"
#include "Data.h"

using namespace std;

namespace
{
    istringstream getLineStrm(istream & in)
    {
        string str;
        getline(in, str);
        return istringstream(str);
    }

    void readLine(istream && in, Data & d, size_t y)
    {
        char ch;
        size_t x = 0;
        while (x < d.width() && !in.eof() && (in >> ch))
        {
            auto type = Type(ch);
            auto currPoint = Point(x, y);

            if (type == Type::King)
            {
                d.start = currPoint;
                type = Type::Free;
            }

            d(currPoint.x, currPoint.y) = { type, 0 };
            if (isEnemy(type))
            {
                d(currPoint.x, currPoint.y).id = d.enemies.size();
                d.enemies.push_back({ type, currPoint });
            }
            ++x;
        }

        if (x != d.width())
        {
            throw invalid_argument("not enougth data on row " + to_string(y + 1));
        }
    }
};

namespace Reader
{
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
            readLine(getLineStrm(in), d, i);
        }

        return d;
    }
}