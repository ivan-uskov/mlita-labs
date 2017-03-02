#include "stdafx.h"
#include "Long.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

pair<Long, Long> readNumbers(istream & in)
{
    string num1, num2;
    if (!(getline(in, num1) && getline(in, num2)))
    {
        throw invalid_argument("Expected two numbers");
    }

    return{ Long(istringstream(num1)), Long(istringstream(num2)) };
}

int main()
{
    ifstream in("input.txt");
    ofstream out("output.txt");

    try
    {
        auto numbers = readNumbers(in);
        numbers.first -= numbers.second;
        out << numbers.first << endl;
    }
    catch (exception const& e)
    {
        cout << e.what() << endl;
        return 1;
    }

    return 0;
}

