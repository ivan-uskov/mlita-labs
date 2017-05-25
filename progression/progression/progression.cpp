#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
#include <set>

using namespace std;

using Item = long long int;

struct ProgressionChecker
{
	ProgressionChecker(ostream & out)
		: out(out)
	{}

	void processData(vector<Item> & data)
	{
		if (!data.size())
		{
			return;
		}

		d.clear();
		sort(data.begin(), data.end());
		for (size_t i = 0; i < data.size(); ++i)
		{
			if ((i + 1) < data.size())
			{
				d.emplace(data[i+1] - data[i]);
			}
		}

		if (d.size() < 2)
		{
			out << "Yes" << endl;
		}
		else
		{
			out << "No" << endl;
		}
	}

	ostream & out;
	set<Item> d;
};

using Processor = ProgressionChecker;

istringstream getLineStrm(istream & in)
{
	string str;
	getline(in, str);
	return istringstream(str);
}

void processData(istream & in, Processor & p)
{
	size_t size;

	auto strm = getLineStrm(in);
	if (!(strm >> size) || (size <= 0))
	{
		throw invalid_argument("invalid data size");
	}

	string str;
	vector<Item> data;
	data.reserve(100);

	for (size_t i = 0; i < size && !in.eof();)
	{
		size_t lineCount;
		auto c = getLineStrm(in);
		c >> lineCount;
		data.clear();
		auto s = getLineStrm(in);
		Item v;
		for (size_t i = 0; i < lineCount && !s.eof();)
		{
			if (s >> v)
			{
				data.push_back(v);
			}
		}
		p.processData(data);
	}
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{

		Processor p(out);
		processData(in, p);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}


	return 0;
}
