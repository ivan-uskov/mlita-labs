#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <list>

using namespace std;

struct Doctors
{
	size_t count;
	vector<list<size_t>> dep;
	vector<bool> got;
	size_t start = numeric_limits<size_t>::max();

};

struct DoctorsPath
{
	bool found = false;
	list<size_t> path;
};

ostream & operator << (ostream & o, DoctorsPath const& dp)
{
	o << (dp.found ? "YES" : "NO") << endl;

	if (dp.found)
	{
		for (auto & item : dp.path)
		{
			o << item + 1 << endl;
		}
	}

	return o;
}

DoctorsPath getDoctorsPath(Doctors & d)
{
	DoctorsPath dp;
	if (!(d.start < d.count))
	{
		return dp;
	}

	auto findEmpty = [](Doctors const& d) {
		for (size_t i = 0; i < d.dep.size(); ++i)
		{
			if (d.dep[i].size() == 0 && !d.got[i])
			{
				return i;
			}
		}

		return d.dep.size();
	};

	auto remove = [](Doctors & d, size_t i) {
		d.got[i] = true;
		for (auto & dep : d.dep)
		{
			auto it = find(dep.begin(), dep.end(), i);
			if (it != dep.end())
			{
				dep.erase(it);
			}
		}
	};

	size_t curr;
	while ((curr = findEmpty(d)) != d.dep.size())
	{
		remove(d, curr);
		dp.path.push_back(curr);
	}

	auto it = find(d.got.begin(), d.got.end(), false);
	dp.found = (it == d.got.end());

	return dp;
}

istringstream getLineStrm(istream & in)
{
	string str;
	getline(in, str);
	return istringstream(str);
}


Doctors readDoctors(istream & in)
{
	Doctors d;

	auto strm = getLineStrm(in);
	if (!(strm >> d.count) || (d.count <= 0))
	{
		throw invalid_argument("invalid width height");
	}

	d.dep.reserve(d.count);
	d.got = vector<bool>(d.count, false);

	string str;

	for (size_t i = 0; i < d.count && !in.eof(); ++i)
	{
		list<size_t> l;

		unsigned c;
		auto s = getLineStrm(in);
		if (s >> c)
		{
			for (size_t v, i = 0; i < c && !s.eof() && (s >> v); ++i) l.push_back(v - 1);
			if (c != l.size())
			{
				throw invalid_argument("not enought data on line " + to_string(i));
			}

			d.dep.push_back(l);

			if (c == 0)
			{
				d.start = i;
			}
		}
	}

	if (d.dep.size() != d.count)
	{
		throw invalid_argument("not enought data");
	}

	return d;
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{
		auto d = readDoctors(in);
		out << getDoctorsPath(d) << endl;
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
