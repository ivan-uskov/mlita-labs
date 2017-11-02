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
                localRes += BigInteger(current * mData[j]).shift(long(j));
            }
            res += localRes.shift(long(i));
        }

        swap(res);
        return *this;
    }

    BigInteger & operator -= (const BigInteger & rhs)
    {
        auto get = [&rhs](std::vector<uint8_t> & numbers, long i) {
            while (numbers[i + 1] == 0)
            {
                numbers[i + 1] = 9;
                i += 1;
            }

            --numbers[i + 1];
        };

        for (size_t j = 0, i = 0; i < rhs.mData.size(); ++i, ++j)
        {
            if (mData[j] < rhs.mData[i])
            {
                get(mData, long(j));
                mData[j] += (10 - rhs.mData[i]);
            }
            else
            {
                mData[j] -= rhs.mData[i];
            }
        }

        removeZeroFront();

        return *this;
    }

    void swap(BigInteger & rhs)
    {
        mData.swap(rhs.mData);
    }

    size_t digitsCount() const
    {
        return mData.size();
    }

private:

    size_t removeZeroFront()
    {
        auto rit = std::find_if(mData.rbegin(), mData.rend(), [](auto & item) {
            return (item != 0);
        });

        auto it = rit.base();
        size_t size = 0;
        if (it != mData.end())
        {
            size = std::distance(it, mData.end());
            mData.erase(it, mData.end());
        }

        return size;
    }

    BigInteger & append(BigInteger const& rhs)
    {
        shift(long(rhs.mData.size()));
        return *this += rhs;
    }

    std::pair<BigInteger, BigInteger> splitFront(size_t size) const
    {
        if (mData.size() <= size)
        {
            return { *this, BigInteger() };
        }

        auto offset = mData.size() - size;
        auto dataBeg = mData.begin();
        std::pair<BigInteger, BigInteger> res;
        res.second.mData = std::vector<uint8_t>(dataBeg, dataBeg + offset);
        res.first.mData = std::vector<uint8_t>(dataBeg + offset, mData.end());

        return res;
    }

    BigInteger & shift(long count)
    {
        if (mData.size() > 0 && count > 0)
        {
            mData.insert(mData.begin(), count, 0);
        }

        return *this;
    }

    friend bool operator == (BigInteger const& lhs, BigInteger const& rhs);
    friend std::ostream & operator << (std::ostream & out, BigInteger const& num);
    friend std::istream & operator >> (std::istream & in, BigInteger & obj);
    friend bool operator < (BigInteger const& lhs, BigInteger const& rhs);

    friend std::pair<BigInteger, BigInteger> divide(const BigInteger & lhs, const BigInteger & rhs);

    std::vector<uint8_t> mData;
};


bool operator != (BigInteger const& lhs, BigInteger const& rhs)
{
    return !(lhs == rhs);
}

bool operator == (BigInteger const& lhs, BigInteger const& rhs)
{
    return lhs.mData == rhs.mData;
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

bool operator < (BigInteger const& lhs, BigInteger const& rhs)
{
    auto less = [&]() {
        for (size_t i = lhs.mData.size() - 1; i >= 0; --i)
        {
            if (lhs.mData[i] == rhs.mData[i])
            {
                continue;
            }

            return lhs.mData[i] < rhs.mData[i];
        }

        return false;
    };

    return (lhs.mData.size() == rhs.mData.size()) && !lhs.mData.empty() ? less() : lhs.mData.size() < rhs.mData.size();
}

bool operator <= (BigInteger const& lhs, BigInteger const& rhs)
{
    return lhs < rhs || lhs == rhs;
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

std::istream & operator >> (std::istream & in, BigInteger & obj)
{
    auto isWhiteSpace = [](int ch) {
        return ch == '\n' || ch == '\t' || ch == '\r' || ch == ' ';
    };

    while (!in.eof())
    {
        auto nextChar = in.peek();
        if (isWhiteSpace(nextChar))
        {
            in.get();
        }
        else
        {
            break;
        }
    }

    BigInteger num;
    while (!in.eof())
    {
        auto nextChar = in.peek();
        if ('0' <= nextChar && nextChar <= '9')
        {
            num.mData.push_back(nextChar - '0');
            in.get();
        }
        else
        {
            break;
        }
    }

    if (num.mData.empty())
    {
        in.setstate(std::ios::failbit);
    }
    else
    {
        std::reverse(num.mData.begin(), num.mData.end());
        num.removeZeroFront();
        obj.swap(num);
    }

    return in;
}

std::pair<BigInteger, BigInteger> divide(const BigInteger & lhs, const BigInteger & rhs)
{
    if (rhs == 0)
    {
        throw std::logic_error("division on zero");
    }

    if (lhs == rhs)
    {
        return { 1, 0 };
    }

    if (lhs < rhs)
    {
        return { 0, lhs };
    }

    auto div = [](BigInteger const & lhs, BigInteger const& rhs) {
        BigInteger tmp(lhs);
        BigInteger count = 0;
        while (rhs < tmp)
        {
            tmp -= rhs;
            count += 1;
        }

        if (rhs == tmp)
        {
            return std::pair<BigInteger, BigInteger>({ count + 1, 0 });
        }

        return std::pair<BigInteger, BigInteger>({ count, tmp });
    };

    auto res = BigInteger();

    auto prev = BigInteger();
    auto amount = lhs;
    auto fullAmount = BigInteger(prev).append(amount);
    while (rhs <= fullAmount)
    {
        auto curr = amount.splitFront(1);
        prev.append(curr.first);
        amount = curr.second;
        while (prev < rhs)
        {
            curr = amount.splitFront(1);
            prev.append(curr.first);
            amount = curr.second;
            res.shift(1);
        }

        auto localRes = div(prev, rhs);
        res.append(localRes.first);
        prev = localRes.second;
        size_t removedZerous = 0;
        if (prev == 0)
        {
            removedZerous = amount.removeZeroFront();
            res.shift(long(removedZerous));
        }
        fullAmount = BigInteger(prev).append(amount);
        if ((fullAmount < rhs) && (fullAmount.digitsCount() > 0))
        {
            res.shift(long(curr.second.digitsCount() - removedZerous));
        }
    }
    fullAmount.removeZeroFront();

    return { res, fullAmount };
}