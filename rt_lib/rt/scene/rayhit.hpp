#pragma once

#include <rt/math/float4.hpp>

namespace rt
{
    class hit
    {
    public:
        bool intersected;
        float4 position;
        float4 normal;
        float distance = std::numeric_limits<float>::infinity();

        hit() : intersected(false) {};
    };
};
