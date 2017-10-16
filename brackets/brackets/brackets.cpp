#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>

#include "BigInteger.h"

using namespace std;

std::pair<size_t, size_t> read(istream & in)
{
    size_t openBracketsCount, depth;
    if (!(in >> openBracketsCount >> depth))
    {
        throw invalid_argument("invalid digits count");
    }

    return {openBracketsCount, depth};
}

BigInteger getBracketsFormsCount(pair<size_t, size_t> nums)
{
    vector<vector<pair<bool, BigInteger>>> cache(nums.first + 1, vector<pair<bool, BigInteger>>(nums.second + 1, { false, 0}));

    function<BigInteger(size_t, size_t)> count = [&count, &cache](size_t openBracketsCount, size_t depth) -> BigInteger {
        if (openBracketsCount == 0)
        {
            return 1;
        }
        if (depth == 0)
        {
            return 0;
        }

        if (cache[openBracketsCount][depth].first)
        {
            return cache[openBracketsCount][depth].second;
        }

        BigInteger sum = 0;
        for (size_t i = 0; i <= openBracketsCount - 1; ++i)
        {
            sum += count(i, depth - 1) * count(openBracketsCount - i - 1, depth);
        }

        cache[openBracketsCount][depth].first = true;
        cache[openBracketsCount][depth].second = sum;

        return sum;
    };

    return count(nums.first, nums.second) - count(nums.first, nums.second - 1);
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        out << getBracketsFormsCount(read(in)) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}