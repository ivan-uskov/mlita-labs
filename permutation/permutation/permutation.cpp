#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>

using namespace std;

struct Permutation
{
	std::vector<size_t> perm;
	unsigned size, count;

	void operator ++ ()
	{
		auto getI = [&] {
			for (size_t i = size - 2; i >= 0; --i)
			{
				if (perm[i] < perm[i + 1])
				{
					return i;
				}
			}

			return size;
		};

		auto getL = [&] (size_t i) {
			for (size_t l = size - 1; l > 0; --l)
			{
				if ((perm[l] > perm[i]) && (l != i))
				{
					return l;
				}
			}
			return size;
		};
		
		auto i = getI();
		auto l = getL(i);
		swap(perm[i], perm[l]);
		reverse(perm.begin() + i + 1, perm.end());
	}

	void printNextPerms(ostream & out)
	{
		while (count-- > 0)
		{
			++(*this);
			printPerm(out);
		}
	}

	void printPerm(ostream & out)
	{
		ostream_iterator<size_t> outIt(out, " ");
		copy(perm.begin(), perm.end(), outIt);
		out << endl;
	}
};



Permutation readPermutation(istream & in)
{
	Permutation permutation;
	if (!(in >> permutation.size >> permutation.count))
	{
		throw invalid_argument("first line incorrect");
	}

	size_t number;
	for (unsigned i = 0; (i < permutation.size) && in && (in >> number);)
	{
		permutation.perm.push_back(number);
	}

	if (permutation.perm.size() != permutation.size)
	{
		throw invalid_argument("Not enought parts");
	}

	return permutation;
}

int main()
{
	ifstream in("input.txt");
	ofstream out("output.txt");

	try
	{
		auto permutation = readPermutation(in);
		permutation.printNextPerms(out);
	}
	catch (exception const& e)
	{
		cout << e.what() << endl;
		return 1;
	}


    return 0;
}

