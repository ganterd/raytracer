#pragma once

#include <cmath>
#include <math.h>

class mutils
{
public:
    inline static float radians(float d)
    {
        return d * (float)M_PI / 180.0f;
    }
};