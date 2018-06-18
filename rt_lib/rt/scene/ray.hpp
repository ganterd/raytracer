#pragma once

#include <glm/glm.hpp>

#include <rt/scene/mesh/tri.hpp>

#include <xmmintrin.h>

namespace rt
{
    class alignas(32) Ray
    {
    public:
        alignas(32) __m256 mSSEDualOrigin;
        alignas(32) __m256 mSSEDualInvDir;
        
        alignas(16) __m128 mSSEDirection;
        alignas(16) __m128 mSSEOrigin;
        alignas(16) __m128 mSSEInvDir;

        

        Ray(const glm::vec3& o, const glm::vec3& d)
        {
            setOrigin(_mm_set_ps(0.0f, o.z, o.y, o.x));
            setDirection(_mm_set_ps(1.0f, d.z, d.y, d.x));
        }

        Ray(const __m128& o, const __m128& d)
        {
            setOrigin(o);
            setDirection(d);
        }

        void setOrigin(const __m128& o)
        {
            mSSEOrigin = o;
            mSSEDualOrigin = _mm256_insertf128_ps(mSSEDualOrigin, mSSEOrigin, 0);
            mSSEDualOrigin = _mm256_insertf128_ps(mSSEDualOrigin, mSSEOrigin, 1);
        }

        void setDirection(const __m128& d)
        {

            mSSEDirection = d;
            mSSEInvDir = _mm_div_ps(_mm_set1_ps(1.0f), mSSEDirection);
            mSSEDualInvDir = _mm256_insertf128_ps(mSSEDualInvDir, mSSEInvDir, 0);
            mSSEDualInvDir = _mm256_insertf128_ps(mSSEDualInvDir, mSSEInvDir, 1);
        }

        glm::vec3 direction() const
        {
            return glm::vec3(mSSEDirection[0], mSSEDirection[1], mSSEDirection[2]);
        }
    };

    class Tri;
    typedef struct _RayHit
    {
    public:

        float mDistance = std::numeric_limits<float>::max();
        glm::vec3 mHitPosition;
        glm::vec3 mSurfaceNormal;
        glm::vec3 mInterpolatedNormal;
        Tri* mTri = nullptr;

        int mTrisTested = 0;
        int mAABBsTested = 0;
    } RayHit;
}