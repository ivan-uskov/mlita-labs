#include "stdafx.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <vector>

using namespace std;

typedef pair<long long int, long long int> Point;
typedef vector<Point> Way;

istringstream getLineStrm(istream & in)
{
	string str;
	getline(in, str);
	return istringstream(str);
}


Way readWay(istream & in)
{
	unsigned count;
	auto strm = getLineStrm(in);
	if (!(strm >> count) || (count <= 0))
	{
		throw invalid_argument("invalid width height");
	}

	Way way;
	way.reserve(count);

	string str;

	for (size_t i = 0; i < count && !in.eof(); ++i)
	{
		long long int x, y;
		auto s = getLineStrm(in);
		if (s >> x >> y)
		{
			way.push_back({ x, y });
		}
	}

	if (way.size() != count)
	{
		throw invalid_argument("not enought data");
	}

	return way;
}

void iteratePairs(Way const& w, function<void(Point const&, Point const&, Point const&)> && f)
{
	auto size = w.size();
	for (size_t i = 0; i < size; ++i)
	{
		if ((i + 2) < size)
		{
			f(w[i], w[i + 1], w[i + 2]);
		}
	}
}

bool isLeftTurn(const Point & a, const Point & b, const Point & c)
{
	auto d = ((a.first - c.first) * (b.second - c.second) - (b.first - c.first) * (a.second - c.second));
	return d > 0;
}

unsigned countLeftTurns(Way const& w)
{
	unsigned count = 0;
	iteratePairs(w, [&count](auto & a, auto & b, auto & c) {
		if (isLeftTurn(a, b, c))
		{
			++count;
		}
	});

	return count;
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{
		auto way = readWay(in);
		out << countLeftTurns(way) << endl;
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}