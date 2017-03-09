#include "stdafx.h"
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <functional>
#include <algorithm>

using namespace std;

enum Side
{
    Left = 1,
    Right = -1
};

struct Railway
{
    unsigned workers, maxRepair;
    list<pair<unsigned, Side>> parts;
};

Railway readRailway(istream & in)
{
    Railway rail;
    if (!(in >> rail.workers >> rail.maxRepair))
    {
        throw invalid_argument("first line incorrect");
    }

    for (unsigned i = 0; in && (i < rail.workers);)
    {
        unsigned left, right;
        string str;
        if (getline(in, str) && (istringstream(str) >> left >> right))
        {
            rail.parts.push_back({ left, Side::Left });
            rail.parts.push_back({ right, Side::Right });
            ++i;
        }
    }

    if (rail.parts.size() != rail.workers * 2)
    {
        throw invalid_argument("Not enought parts");
    }

    return rail;
}

unsigned calcBandsCount(Railway & railway)
{
    railway.parts.sort([](auto & lhs, auto & rhs) {
        return (lhs.first == rhs.first) ? (lhs.second > rhs.second) : (lhs.first < rhs.first);
    });

    unsigned counter = 0, curr = 0;

    for (auto & part : railway.parts)
    {
        curr += part.second;
        if (curr > railway.maxRepair)
        {
            continue;
        }

        if (curr == railway.maxRepair && part.second == Side::Left)
        {
            ++counter;
        }
    }

    return counter;
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto railway = readRailway(in);
        out << calcBandsCount(railway) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}
