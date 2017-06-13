#pragma once

namespace Utils
{
    size_t makeBinary(size_t i);
    size_t toBinarySet(size_t count);
    void iterateCortages(size_t size, std::function<void(size_t)> && fn);
}