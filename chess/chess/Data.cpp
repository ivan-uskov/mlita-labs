#include "stdafx.h"
#include "Data.h"

std::ostream & operator << (std::ostream & out, Data const& d)
{
    for (size_t y = 0; y < d.height(); ++y)
    {
        for (size_t x = 0; x < d.width(); ++x)
        {
            out << char(d(x, y));
        }
        out << std::endl;
    }
    return out;
}