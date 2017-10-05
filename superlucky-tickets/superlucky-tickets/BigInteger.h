#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

class BigInteger
{
public:
    BigInteger(size_t num)
    {
        while (num > 0)
        {
            mData.push_back(num % 10);
            num /= 10;
        }
    }

    BigInteger & operator += (BigInteger const& rhs)
    {
        uint8_t carry = 0;
        size_t i = 0;
        for (; i < rhs.mData.size(); ++i)
        {
            if (i == mData.size())
            {
                mData.push_back(0);
            }

            carry = mData[i] + rhs.mData[i];
            mData[i] = carry % 10;
            carry /= 10;
        }

        while (carry > 0)
        {
            if (i == mData.size())
            {
                mData.push_back(0);
            }

            carry += mData[i];
            mData[i] = carry % 10;
            carry /= 10;
            ++i;
        }

        return *this;
    }

private:
    friend std::ostream & operator << (std::ostream & out, BigInteger const& num);

    std::vector<uint8_t> mData;
    bool mIsNegative = false;
};

std::ostream & operator << (std::ostream & out, BigInteger const& num)
{
    std::for_each(num.mData.rbegin(), num.mData.rend(), [&out](auto item) {
        out << std::to_string(item);
    });

    return out;
}