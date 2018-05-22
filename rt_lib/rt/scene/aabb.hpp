#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

#include <rt/scene/ray.hpp>

#include <xmmintrin.h>

namespace rt
{
    class AABB
    {
    public:
        glm::vec3 mMin;
        glm::vec3 mMax;
        glm::vec3 mSize;
        __m256 mSSEBounds;

        AABB()
        {
            mSize = glm::vec3(0);
        }

        /**
         * Grow to accommodate point p
         */
        void grow(const glm::vec3& p)
        {
            mMin = glm::min(mMin, p);
            mMax = glm::max(mMax, p);
            mSize = mMax - mMin;
            refreshSSE();
        }

        /**
         * Grow to accommodate another AABB
         */
        void grow(const AABB& other)
        {
            mMin = glm::min(mMin, other.mMin);
            mMax = glm::max(mMax, other.mMax);
            mSize = mMax - mMin;
            refreshSSE();
        }

        void refreshSSE()
        {
            mSSEBounds[0] = mMin.x;
            mSSEBounds[1] = mMin.y;
            mSSEBounds[2] = mMin.z;
            mSSEBounds[3] = 0.0f;

            mSSEBounds[4] = mMax.x;
            mSSEBounds[5] = mMax.y;
            mSSEBounds[6] = mMax.z;
            mSSEBounds[7] = 0.0f;
        }

        float surfaceArea()
        {
            return 2.0f * mSize.y * mSize.x + 2.0f * mSize.y * mSize.z + 2.0f * mSize.x * mSize.z;
        }


        static AABB infinity()
        {
            AABB newAABB;
            newAABB.mMin = glm::vec3(+std::numeric_limits<float>::infinity());
            newAABB.mMax = glm::vec3(-std::numeric_limits<float>::infinity());
            newAABB.mSize = glm::vec3(0.0f);
            return newAABB;
        }

        bool intersect(const rt::Ray& r) const
        {
            __m256 t1 = _mm256_mul_ps(_mm256_add_ps(mSSEBounds, r.mSSEOffset), r.mSSEInvDir);
            __m128 _min = _mm_min_ps(_mm_load_ps(&t1[0]), _mm_load_ps(&t1[4]));
            __m128 _max = _mm_max_ps(_mm_load_ps(&t1[0]), _mm_load_ps(&t1[4]));

            float tmin = _min[0];
            tmin = std::max(tmin, _min[1]);
            tmin = std::max(tmin, _min[2]);

            float tmax = _max[0];
            tmax = std::min(tmax, _max[1]);
            tmax = std::min(tmax, _max[2]);
        
            return tmax >= tmin;
        }

        friend std::ostream& operator<< (std::ostream& os, const AABB& a)
        {
            os << "[" << a.mMin.x << "," << a.mMin.y << "," << a.mMin.z << "]->";
            os << "[" << a.mMax.x << "," << a.mMax.y << "," << a.mMax.z << "]";
            return os;
        }
    };
}