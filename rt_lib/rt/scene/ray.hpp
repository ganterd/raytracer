#pragma once

#include <glm/glm.hpp>

#include <rt/scene/mesh/tri.hpp>

namespace rt
{
    class Ray
    {
    public:
        glm::vec3 mOrigin;
        glm::vec3 mDirection;  
    };

    class Tri;
    class RayHit
    {
    public:
        RayHit()
        {
            mDistance = std::numeric_limits<float>::max();
        }

        float mDistance;
        glm::vec3 mHitPosition;
        glm::vec3 mSurfaceNormal;
        glm::vec3 mInterpolatedNormal;
        Tri* mTri;
    };
}