#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

using Item = long long int;

struct Segment
{
	Segment(vector<Item> const& s)
		: values(s.size() * 4, 0)
		, size(s.size())
	{
		build(s, 1, 0, size - 1);
	}

	void add(Item i, Item deltha)
	{
		_add(1, 0, size - 1, i, deltha);
	}

	Item rsq(Item i, Item j)
	{
		return sum(1, 0, size - 1, i, j);
	}

private:
	void _add(Item v, Item tl, Item tr, Item pos, Item deltha) {
		if (tl == tr)
		{
			values[v] += deltha;
		}
		else
		{
			auto tm = (tl + tr) / 2;
			if (pos <= tm)
			{
				_add(v * 2, tl, tm, pos, deltha);
			}	
			else
			{
				_add(v * 2 + 1, tm + 1, tr, pos, deltha);
			}
			values[v] = min(values[v * 2], values[v * 2 + 1]);
		}
	}

	Item sum(Item v, Item tl, Item tr, Item l, Item r)
	{
		if (l > r)
		{
			return std::numeric_limits<Item>::max();
		}
		if (l == tl && r == tr)

		{
			return values[v];
		}
			
		auto tm = (tl + tr) / 2;
		return min(sum(v * 2, tl, tm, l, min(r, tm)), sum(v * 2 + 1, tm + 1, tr, max(l, tm + 1), r));
	}

	void build(vector<Item> const& a, Item v, Item tl, Item tr)
	{
		if (tl == tr)
		{
			values[v] = a[tl];
		}
			
		else
		{
			auto tm = (tl + tr) / 2;
			build(a, v * 2, tl, tm);
			build(a, v * 2 + 1, tm + 1, tr);
			values[v] = min(values[v * 2], values[v * 2 + 1]);
		}
	}

	size_t size;
	vector<Item> values;
};

using Data = Segment;

istringstream getLineStrm(istream & in)
{
	string str;
	getline(in, str);
	return istringstream(str);
}


Data readData(istream & in)
{
	size_t size;

	auto strm = getLineStrm(in);
	if (!(strm >> size) || (size <= 0))
	{
		throw invalid_argument("invalid data size");
	}

	
	string str;
	vector<Item> items;
	items.reserve(size);
	auto s = getLineStrm(in);
	for (size_t i = 0; i < size && !s.eof();)
	{
		Item v;

		if (s >> v)
		{
			items.push_back(v);
			++i;
		}
	}

	return Data(items);
}

void runAdd(Data & d, size_t i, Item item)
{
	d.add(i, item);
}

void runRsq(Data & d, size_t i, size_t j, ostream & o)
{
	o << d.rsq(i, j) << endl;
}

void runCommand(Data & d, istream && in, ostream & out)
{
	string command;
	if (in >> command)
	{
		if (command == "add")
		{
			size_t i;
			Item item;
			if (in >> i >> item)
			{
				runAdd(d, i, item);
			}
		}
		else if (command == "rmq")
		{
			size_t i, j;
			if (in >> i >> j)
			{
				runRsq(d, i, j, out);
			}
		}
	}
}

void printResult(Data & d, istream & in, ostream & out)
{
	string str;
	while (!in.eof() && getline(in, str))
	{
		runCommand(d, istringstream(str), out);
	}
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{
		printResult(readData(in), in, out);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}

