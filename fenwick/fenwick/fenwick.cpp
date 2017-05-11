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

struct Fenwick
{
	Fenwick(size_t s)
		: v(s, 0)
		, size(s)
	{}

	void add(Item i, Item delta)
	{
		for (; i < size; i = (i | (i + 1)))
		{
			v[i] += delta;
		}
	}

	Item sum(Item r)
	{
		Item result = 0;
		for (; r >= 0; r = (r & (r + 1)) - 1)
		{
			result += v[r];
		}
	
		return result;
	}

	Item sum(Item i, Item j)
	{
		return sum(j) - sum(i - 1);
	}

	size_t size;
	vector<Item> v;
};

using Data = Fenwick;

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

	Data d(size);
	string str;

	auto s = getLineStrm(in);
	for (size_t i = 0; i < size && !s.eof();)
	{
		Item v;
		
		if (s >> v)
		{
			d.add(i, v);
			++i;
		}
	}

	return d;
}

void runAdd(Data & d, size_t i, Item item)
{
	d.add(i, item);
}

void runRsq(Data & d, size_t i, size_t j, ostream & o)
{
	o << d.sum(i, j) << endl;
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
		else if (command == "rsq")
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

