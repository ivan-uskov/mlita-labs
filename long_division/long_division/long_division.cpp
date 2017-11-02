#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "BigInteger.h"

using namespace std;

pair<BigInteger, BigInteger> read(istream & in)
{
    size_t firstDigitsCount, secondDigitsCount;
    BigInteger first, second;
    if (in >> firstDigitsCount >> secondDigitsCount >> first >> second)
    {
        if (firstDigitsCount != first.digitsCount())
        {
            throw invalid_argument("first number real digits count not equal to expected digits count");
        }
        if (secondDigitsCount != second.digitsCount())
        {
            throw invalid_argument("second number real digits count not equal to expected digits count");
        }

        return {first, second};
    }

    throw invalid_argument("invalid input file expected <first digits count> <second digits count> <first numer> <second numer>");
}

int main()
{
    try
    {
        ifstream in("input.txt");
        ofstream out("output.txt");

        auto numbers = read(in);
        auto res = divide(numbers.first, numbers.second);
        out << res.first << endl << res.second << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}