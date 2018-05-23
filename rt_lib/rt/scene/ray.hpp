#pragma once

#include <glm/glm.hpp>

#include <rt/scene/mesh/tri.hpp>

#include <xmmintrin.h>

namespace rt
{
    class alignas(alignof(__m256)) Ray
    {
    public:
        alignas(alignof(__m256)) __m256 mSSEOffset;
        alignas(alignof(__m256)) __m256 mSSEInvDir;

        glm::vec3 mOrigin;
        glm::vec3 mDirection;
        glm::vec3 mInverseDirection;

        

        Ray(const glm::vec3& o, const glm::vec3& d)
        {
            mOrigin = o;
            mDirection = d;
            mInverseDirection = 1.0f / d;

            
            mSSEOffset[0] = mSSEOffset[4] = -o.x;
            mSSEOffset[1] = mSSEOffset[5] = -o.y;
            mSSEOffset[2] = mSSEOffset[6] = -o.z;
            mSSEOffset[3] = mSSEOffset[7] = 0.0f;

            mSSEInvDir[0] = mSSEInvDir[4] = mInverseDirection.x;
            mSSEInvDir[1] = mSSEInvDir[5] = mInverseDirection.y;
            mSSEInvDir[2] = mSSEInvDir[6] = mInverseDirection.z;
            mSSEInvDir[3] = mSSEInvDir[7] = 0.0f;
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