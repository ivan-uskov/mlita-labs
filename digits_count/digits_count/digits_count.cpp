#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <fstream>

using namespace std;

size_t getNumberDigitsCount(size_t num)
{
    size_t count = 0;
    while (num > 0)
    {
        num /= 10;
        ++count;
    }

    return count;
}

size_t getRangeDigitsCount(pair<size_t, size_t> nums)
{
    size_t lower = nums.first;
    size_t higher = nums.second;

    auto lowerDigitsCount = getNumberDigitsCount(lower);
    auto higherDigitsCount = getNumberDigitsCount(higher);
    if (lowerDigitsCount == higherDigitsCount)
    {
        return (higher - lower + 1) * lowerDigitsCount;
    }

    auto next = pow(10, lowerDigitsCount);

    return getRangeDigitsCount({ lower, next - 1 }) + getRangeDigitsCount({next, higher});
}

pair<size_t, size_t> readBorders(istream & in)
{
    pair<size_t, size_t> val;
    if (in >> val.first >> val.second)
    {
        return val;
    }

    throw invalid_argument("two numbers expected");
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    out << getRangeDigitsCount(readBorders(in)) << endl;
    return 0;
}

