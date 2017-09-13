#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

size_t readDigitsCount(istream & in)
{
    size_t digitsCount;
    if (!(in >> digitsCount))
    {
        throw invalid_argument("invalid digits count");
    }

    return digitsCount;
}

size_t count(size_t digitsCount, size_t sumValue)
{
    if (digitsCount == 0)
    {
        return sumValue == 0 ? 1 : 0;
    }

    size_t res = 0;
    for (size_t j = 0; j <= min(size_t(9), sumValue); ++j)
    {
        res += count(digitsCount - 1, sumValue - j);
    }

    return res;
};

size_t getSuperLuckyTicketsCount(size_t digitsCount)
{
    size_t res = 0;
    for (size_t sumValue = 0; sumValue <= digitsCount * 9; ++sumValue)
    {
        auto d = count(digitsCount, sumValue);
        cout << sumValue << " : " << d << endl;
        res += d * d;
    }

    return res;
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        cout << getSuperLuckyTicketsCount(readDigitsCount(cin)) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

