#pragma once

namespace rt
{
    class hit
    {
    public:
        bool intersected;
        float4 position;
        float4 normal;
        float distance;

        hit() : intersected(false) {};
    };
};
