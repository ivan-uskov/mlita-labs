using System;
using System.Numerics;
using System.IO;

namespace superlucky_tickets_c_sharp
{
    class Program
    {
        static void Main(string[] args)
        {
            var digitsCount = Int32.Parse((new StreamReader("input.txt")).ReadLine());
            var ticketsCount = (new TicketsCountCalculator()).calculate(digitsCount);

            var w = (new StreamWriter("output.txt"));
            w.WriteLine(ticketsCount);
            w.Close();
        }
    }

    class TicketsCountCalculator
    {
        private Cache<BigInteger?> cache;

        public BigInteger calculate(int digitsCount)
        {
            if (digitsCount <= 0)
            {
                return 0;
            }

            cache = new Cache<BigInteger?>(digitsCount + 1, 10, digitsCount * 9 + 1);

            BigInteger count = 0;

            for (int lastDigit = 0; lastDigit <= 9; ++lastDigit)
            {
                var maxDigitsSum = getMaxDigitsSum(lastDigit, digitsCount);
                for (int digitsSum = getMinDigitsSum(lastDigit, digitsCount); digitsSum <= maxDigitsSum; ++digitsSum)
                {
                    var numbersCount = f(digitsCount, lastDigit, digitsSum);
                    count += numbersCount * (numbersCount + (lastDigit == 0 ? 0 : f(digitsCount, lastDigit - 1, digitsSum)) + (lastDigit == 9 ? 0 : f(digitsCount, lastDigit + 1, digitsSum)));
                }
            }

            return count;
        }

        static bool inRange(int val, int min, int max)
        {
            return min <= val && val <= max;
        }

        private int getMaxDigitsSum(int lastDigit, int digitsCount)
        {
            var progressiveCount = Math.Min(digitsCount, 9 - lastDigit + 1);
            return ((2 * lastDigit + (progressiveCount - 1)) * progressiveCount) / 2 + (digitsCount - progressiveCount) * 9;
        }

        private int getMinDigitsSum(int lastDigit, int digitsCount)
        {
            var progressiveCount = Math.Min(digitsCount, lastDigit);
            return ((2 * (lastDigit - progressiveCount + 1) + (progressiveCount - 1)) * progressiveCount) / 2;
        }

        private BigInteger f(int i, int j, int k)
        {
            if (k < 0)
            {
                return 0;
            }
            if (k == 0 && j > 0)
            {
                return 0;
            }
            if (i == 1)
            {
                return j == k && inRange(j, 0, 9) ? 1 : 0;
            }
            if (j == k && k == 0)
            {
                return 1;
            }

            var item = cache.get(i, j, k);
            item = item ?? f(i - 1, j, k - j) + (j == 0 ? 0 : f(i - 1, j - 1, k - j)) + (j == 9 ? 0 : f(i - 1, j + 1, k - j));
            cache.set(i, j, k, item);

            return item.GetValueOrDefault();
        }
    }

    class Cache<ItemT>
    {
        private ItemT[] items;
        private int maxX;
        private int maxY;
        private int maxZ;

        public Cache(int mX, int mY, int mZ)
        {
            maxX = mX;
            maxY = mY;
            maxZ = mZ;
            items = new ItemT[mX * mY * mZ];
        }

        public ItemT get(int x, int y, int z)
        {
            var pos = getPos(x, y, z);
            if (pos >= items.Length)
            {
                throw new Exception("Invalid point");
            }

            return items[pos];
        }

        public void set(int x, int y, int z, ItemT v)
        {
            var pos = getPos(x, y, z);
            if (pos >= items.Length)
            {
                throw new Exception("Invalid point");
            }

            items[pos] = v;
        }

        private int getPos(int x, int y, int z)
        {
            return z * maxY * maxX + y * maxX + x;
        }
    }
}
