#pragma once

#include <rt/scene/scene.hpp>
#include <rt/buffer/buffer.hpp>

namespace rt
{
    class RayTracer
    {
    public:
        typedef struct
        {
            int bounces = 0;
            int bounceSamples = 8;
        } Settings;

        rt::RayTracer::Settings mSettings;

        virtual void init(rt::Scene* s){};
        virtual void Trace(rt::Scene* s, rt::Buffer* b, glm::ivec2 renderRegionMin, glm::ivec2 renderRegionMax) = 0;
    };
}