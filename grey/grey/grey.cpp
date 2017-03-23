#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

struct Grey
{
	std::vector<bool> vals;
	std::vector<size_t> stack;
	unsigned size, count;
	bool isEven = false;

	void operator ++ ()
	{
		if (isEven)
		{
			if (vals.back())
			{
				stack.pop_back();
			}
			else
			{
				stack.push_back(vals.size() - 1);
			}
			vals.back() = !vals.back();
		}
		else
		{
			if (stack.back() == 0)
			{
				vals.front() = !vals.front();
				stack.pop_back();
			}
			else
			{
				auto i = stack.back();
				if (vals[i - 1] == 1)
				{
					stack.erase(--(--stack.end()));
				}
				else
				{
					stack.insert(--stack.end(), i - 1);
				}
				vals[i - 1] = !vals[i - 1];
			}
		}

		isEven = !isEven;
	}

	void printNext(ostream & out)
	{
		while (count-- > 0)
		{
			++(*this);
			print(out);
		}
	}

	void print(ostream & out)
	{
		ostream_iterator<bool> outIt(out, " ");
		copy(vals.begin(), vals.end(), outIt);
		out << endl;
	}
};

Grey readGrey(istream & in)
{
	Grey grey;
	if (!(in >> grey.size >> grey.count))
	{
		throw invalid_argument("first line incorrect");
	}

	uint8_t number;
	for (unsigned i = 0; (i < grey.size) && in && (in >> number);)
	{
		grey.vals.push_back(number == '1');
	}

	if (grey.vals.size() != grey.size)
	{
		throw invalid_argument("Not enought parts");
	}

	auto evenCount = count_if(grey.vals.begin(), grey.vals.end(), [](auto item) {return item; });
	grey.isEven = (evenCount % 2 == 0);

	for (size_t i = 0; i < grey.vals.size(); ++i)
	{
		if (grey.vals[i])
		{
			grey.stack.push_back(i);
		}
	}

	return grey;
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{
		readGrey(in).printNext(out);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}


	return 0;
}

