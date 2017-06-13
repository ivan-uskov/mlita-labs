#include "stdafx.h"
#include "Utils.h"

namespace Utils
{
    size_t makeBinary(size_t i)
    {
        return size_t(1) << i;
    }

    size_t toBinarySet(size_t count)
    {
        size_t bSet = 0;
        for (size_t i = 0; i < count; ++i)
        {
            bSet |= makeBinary(i);
        }

        return bSet;
    }

    void iterateCortages(size_t size, std::function<void(size_t)> && fn)
    {
        for (size_t i = 0; i < pow(2, size); ++i)
        {
            fn(i);
        }
    }
}
