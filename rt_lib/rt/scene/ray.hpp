#pragma once

#include <glm/glm.hpp>

#include <rt/scene/mesh/tri.hpp>

#include <xmmintrin.h>

namespace rt
{
    class Ray
    {
    public:
        glm::vec3 mOrigin;
        glm::vec3 mDirection;
        glm::vec3 mInverseDirection;

        __m128 mSSEOffset;
        __m128 mSSEInvDir;

        Ray(const glm::vec3& o, const glm::vec3& d)
        {
            mOrigin = o;
            mDirection = d;
            mInverseDirection = 1.0f / d;

            float p[4];
            p[3] = 0.0f;

            p[0] = -o.x;
            p[1] = -o.y;
            p[2] = -o.z;
            mSSEOffset = _mm_load_ps(p);

            p[0] = mInverseDirection.x;
            p[1] = mInverseDirection.y;
            p[2] = mInverseDirection.z;
            mSSEInvDir = _mm_load_ps(p);
        }
    };

    class Tri;
    class RayHit
    {
    public:
        RayHit()
        {
            mDistance = std::numeric_limits<float>::max();
            mTri = nullptr;
        }

        float mDistance;
        glm::vec3 mHitPosition;
        glm::vec3 mSurfaceNormal;
        glm::vec3 mInterpolatedNormal;
        Tri* mTri;
    };
}