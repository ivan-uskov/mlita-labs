#pragma once
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

class Long
{
public:
    Long(std::istream & in)
    {
        std::istream_iterator<char> inBegin(in);
        std::istream_iterator<char> inEnd;
        mNumbers.reserve(2000);
        std::for_each(inBegin, inEnd, [&](char ch) {
            mNumbers.push_back(ch - '0');
        });
    }

    void operator -= (const Long & rhs)
    {
        auto get = [](std::vector<uint8_t> & numbers, int i) {
            while (numbers[i - 1] == 0)
            {
                numbers[i - 1] = 9;
                i -= 1;
            }

            --numbers[i - 1];
        };

        for (int j = mNumbers.size() - 1, i = rhs.mNumbers.size() - 1; i >= 0; --i, --j)
        {
            if (mNumbers[j] < rhs.mNumbers[i])
            {
                get(mNumbers, j);
                mNumbers[j] += (10 - rhs.mNumbers[i]);
            }
            else
            {
                mNumbers[j] -= rhs.mNumbers[i];
            }
        }
    }

private:
    friend std::ostream & operator << (std::ostream & out, const Long & num);
    std::vector<uint8_t> mNumbers;
};

std::ostream & operator << (std::ostream & out, const Long & num)
{
    std::ostream_iterator<char> outBegin(out);
    auto begin = std::find_if(num.mNumbers.begin(), num.mNumbers.end(), [](uint8_t i) {return i > 0; });
    if (begin != num.mNumbers.end())
    {
        std::transform(begin, num.mNumbers.end(), outBegin, [](uint8_t num) {
            return static_cast<char>(num + '0');
        });
    }
    else
    {
        out << '0';
    }

    return out;
}