#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

class BigInteger
{
public:
    BigInteger() = default;

    BigInteger(size_t num)
    {
        mData.reserve(30);
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

            carry += mData[i] + rhs.mData[i];
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

    BigInteger & operator *= (BigInteger const& rhs)
    {
        BigInteger res;
        for (size_t i = 0; i < rhs.mData.size(); ++i)
        {
            BigInteger localRes;
            auto current = rhs.mData[i];
            if (current == 0)
            {
                continue;
            }

            for (size_t j = 0; j < mData.size(); ++j)
            {
                localRes += BigInteger(current * mData[j]).shift(j);
            }
            res += localRes.shift(i);
        }

        swap(res);
        return *this;
    }

    BigInteger & operator -= (const BigInteger & rhs)
    {
        auto get = [](std::vector<uint8_t> & numbers, int i) {
            while (numbers[i + 1] == 0)
            {
                numbers[i + 1] = 9;
                i += 1;
            }

            --numbers[i + 1];
        };

        for (int j = 0, i = 0; i < rhs.mData.size(); ++i, ++j)
        {
            if (mData[j] < rhs.mData[i])
            {
                get(mData, j);
                mData[j] += (10 - rhs.mData[i]);
            }
            else
            {
                mData[j] -= rhs.mData[i];
            }
        }

        auto rit = std::find_if(mData.rbegin(), mData.rend(), [](auto & item) {
            return (item != 0);
        });

        auto it = rit.base();
        if (it != mData.end())
        {
            mData.erase(it, mData.end());
        }

        return *this;
    }

    void swap(BigInteger & rhs)
    {
        mData.swap(rhs.mData);
        std::swap(mIsNegative, rhs.mIsNegative);
    }

private:
    BigInteger & shift(size_t count)
    {
        if (mData.size() > 0 && count > 0)
        {
            mData.insert(mData.begin(), count, 0);
        }

        return *this;
    }

    friend bool operator != (BigInteger const& lhs, BigInteger const& rhs);
    friend std::ostream & operator << (std::ostream & out, BigInteger const& num);

    std::vector<uint8_t> mData;
    bool mIsNegative = false;
};

bool operator != (BigInteger const& lhs, BigInteger const& rhs)
{
    return !(lhs.mData == rhs.mData && lhs.mIsNegative == rhs.mIsNegative);
}

BigInteger operator * (BigInteger const& lhs, BigInteger const& rhs)
{
    return (BigInteger(lhs) *= rhs);
}

BigInteger operator + (BigInteger const& lhs, BigInteger const& rhs)
{
    return (BigInteger(lhs) += rhs);
}

BigInteger operator - (BigInteger const& lhs, BigInteger const& rhs)
{
    return (BigInteger(lhs) -= rhs);
}

std::ostream & operator << (std::ostream & out, BigInteger const& num)
{
    if (num.mData.empty())
    {
        out << 0;
    }

    std::for_each(num.mData.rbegin(), num.mData.rend(), [&out](auto item) {
        out << std::to_string(item);
    });

    return out;
}