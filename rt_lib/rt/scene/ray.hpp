#pragma once

#include <glm/glm.hpp>

#include <rt/scene/mesh/tri.hpp>

#include <xmmintrin.h>

namespace rt
{
    class alignas(16) Ray
    {
    public:
        alignas(16) __m128 mSSEDirection;
        alignas(16) __m128 mSSEOrigin;
        alignas(16) __m128 mSSEInvDir;

        Ray(const glm::vec3& o, const glm::vec3& d)
        {
            mSSEOrigin = _mm_set_ps(0.0f, o.z, o.y, o.x);
            mSSEDirection = _mm_set_ps(1.0f, d.z, d.y, d.x);
            mSSEInvDir = _mm_div_ps(_mm_set1_ps(1.0f), mSSEDirection);
        }

        Ray(const __m128& o, const __m128& d)
        {
            mSSEOrigin = o;
            mSSEDirection = d;
            mSSEInvDir = _mm_div_ps(_mm_set1_ps(1.0f), mSSEDirection);
        }

        glm::vec3 direction() const
        {
            return glm::vec3(mSSEDirection[0], mSSEDirection[1], mSSEDirection[2]);
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