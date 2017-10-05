#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <memory>
#include "BigInteger.h"
#include "Cache.h"

using namespace std;

struct CacheItem
{
    size_t val = 0;
    bool initialized = false;
};

typedef Matrix3D<CacheItem> Cache;
unique_ptr<Cache> mCache;

size_t readDigitsCount(istream & in)
{
    size_t digitsCount;
    if (!(in >> digitsCount))
    {
        throw invalid_argument("invalid digits count");
    }

    return digitsCount;
}

bool in(long num, long min, long max)
{
    return min <= num && num <= max;
}

size_t f(long i, long j, long k)
{
    if (k < 0)
    {
        return 0;
    }
    else if (k == 0 && j > 0)
    {
        return 0;
    }
    else if (i == 1)
    {
        return j == k && in(j, 0, 9) ? 1 : 0;
    }
    else if (j == k && k == 0)
    {
        return 1;
    }

    auto & cacheItem = mCache->operator()(size_t(i), size_t(j), size_t(k));
    if (!cacheItem.initialized)
    {
        cacheItem.initialized = true;
        cacheItem.val = f(i - 1, j, k - j) + (j == 0 ? 0 : f(i - 1, j - 1, k - j)) + (j == 9 ? 0 : f(i - 1, j + 1, k - j));
    }

    return cacheItem.val;
}

size_t getMaxDigitsSum(uint8_t lastDigit, size_t digitsCount)
{
    auto progressiveCount = min(digitsCount, size_t(9 - lastDigit + 1));
    return ((2 * lastDigit + (progressiveCount - 1)) * progressiveCount) / 2 + (digitsCount - progressiveCount) * 9;
}

size_t getMinDigitsSum(uint8_t lastDigit, size_t digitsCount)
{
    auto progressiveCount = min(digitsCount, size_t(lastDigit));
    return ((2 * (lastDigit - progressiveCount + 1) + (progressiveCount - 1)) * progressiveCount) / 2;
}

size_t getSuperLuckyTicketsCount(size_t digitsCount)
{
    if (digitsCount == 0)
    {
        return 0;
    }

    mCache.reset(new Cache(digitsCount + 1, 10, digitsCount * 9 + 1, CacheItem()));

    size_t count = 0;

    for (uint8_t lastDigit = 0; lastDigit <= 9; ++lastDigit)
    {
        auto maxDigitsSum = getMaxDigitsSum(lastDigit, digitsCount);
        for (size_t digitsSum = getMinDigitsSum(lastDigit, digitsCount); digitsSum <= maxDigitsSum; ++digitsSum)
        {
            size_t numbersCount = f(long(digitsCount), lastDigit, long(digitsSum));
            count += numbersCount * (numbersCount + (lastDigit == 0 ? 0 : f(long(digitsCount), lastDigit - 1, long(digitsSum))) + (lastDigit == 9 ? 0 : f(long(digitsCount), lastDigit + 1, long(digitsSum))));
        }
    }

    return count;
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