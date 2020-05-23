#pragma once

#include <cmath>

class mutils
{
public:
    inline static float radians(float d)
    {
        return d * M_PIf32 / 180.0f;
    }
};