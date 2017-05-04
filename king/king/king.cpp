#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

using Data = vector<pair<size_t, size_t>>;

istringstream getLineStrm(istream & in)
{
	string str;
	getline(in, str);
	return istringstream(str);
}


Data readData(istream & in)
{
	size_t s;
	Data d;

	auto strm = getLineStrm(in);
	if (!(strm >> s) || (s <= 0))
	{
		throw invalid_argument("invalid data size");
	}

	d.reserve(s);
	string str;

	for (size_t i = 0; i < s && !in.eof();)
	{
		pair<size_t, size_t> p;
		auto s = getLineStrm(in);
		if (s >> p.first >> p.second)
		{
			d.push_back(p);
			++i;
		}
	}

	if (d.size() != s)
	{
		throw invalid_argument("not enought data");
	}

	return d;
}

size_t printPointChoise(pair<size_t, size_t> const& p)
{
	const auto dstY = 1;
	const auto dstX = 1;

	size_t distance;
	auto x = p.second;
	auto y = p.first;

	size_t res;
	if (y == x)
	{
		distance = y - dstY;
		res = (distance % 2 == 0) ? 2 : 1;
	}
	else if (y < x)
	{
		distance = x - dstX;
		res = (distance % 2 == 0) ? 2 : 1;
		if (res == 2 && (y % 2 == 0))
		{
			res = 1;
		}
	}
	else
	{
		distance = y - dstY;
		res = (distance % 2 == 0) ? 2 : 1;
		if (res == 2 && (x % 2 == 0))
		{
			res = 1;
		}
	}

	return res;
}

void printResult(Data const& d, ostream & out)
{
	for (auto const& p : d)
	{
		out << printPointChoise(p) << endl;
	}
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{
		printResult(readData(in), out);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}


	return 0;
}

