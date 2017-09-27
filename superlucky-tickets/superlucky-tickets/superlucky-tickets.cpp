#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <math.h>

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

bool in(long num, long min, long max)
{
    return min <= num && num <= max;
}

size_t f(long i, long j, long k)
{
    if (k == 0 && j > 0)
    {
        return 0;
    }

    if (i == 1)
    {
        return j == k && in(j, 0, 9) ? 1 : 0;
    }

    if (j == k && k == 0)
    {
        return 1;
    }

    return f(i - 1, j, k - j) + (j == 0 ? 0 : f(i - 1, j - 1, k - j)) + (j == 9 ? 0 : f(i - 1, j + 1, k - j));
}

size_t getSuperLuckyTicketsCount(size_t digitsCount)
{
    if (digitsCount == 0)
    {
        return 0;
    }

    size_t count = 0;
    for (int digitsSum = 0; digitsSum <= long(digitsCount) * 9; ++digitsSum)
    {
        for (int lastDigit = 0; lastDigit <= 9; ++lastDigit)
        {
            size_t numbersCount  = f(digitsCount, lastDigit, digitsSum);
            count += numbersCount * (numbersCount + (lastDigit == 0 ? 0 : f(digitsCount, lastDigit - 1, digitsSum)) + (lastDigit == 9 ? 0 : f(digitsCount, lastDigit + 1, digitsSum)));
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
        out << getSuperLuckyTicketsCount(readDigitsCount(in)) << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

