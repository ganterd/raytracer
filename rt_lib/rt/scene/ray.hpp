#pragma once

#include <glm/glm.hpp>

#include <rt/scene/mesh/tri.hpp>

#include <xmmintrin.h>

namespace rt
{
    class alignas(32) Ray
    {
    public:
        alignas(4) glm::vec3 mOrigin;
        alignas(4) glm::vec3 mDirection;
        alignas(4) glm::vec3 mInverseDirection;

        alignas(16) __m128 mSSEDirection;
        alignas(16) __m128 mSSEOrigin;
        alignas(32) __m256 mSSEOffset;
        alignas(32) __m256 mSSEInvDir;

        Ray(const glm::vec3& o, const glm::vec3& d)
        {
            mOrigin = o;
            mDirection = d;
            mInverseDirection = 1.0f / d;

            mSSEOrigin[0] = o[0];
            mSSEOrigin[1] = o[1];
            mSSEOrigin[2] = o[2];
            mSSEOrigin[3] = 0;

            mSSEDirection[0] = d[0];
            mSSEDirection[1] = d[1];
            mSSEDirection[2] = d[2];
            mSSEDirection[3] = 0;
            
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