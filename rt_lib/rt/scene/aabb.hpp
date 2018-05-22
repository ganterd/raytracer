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
        __m128 mSSEMin;
        __m128 mSSEMax;

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
            float p[4];
            p[0] = mMin.x;
            p[1] = mMin.y;
            p[2] = mMin.z;
            p[4] = 0.0f;
            mSSEMin = _mm_load_ps(p);

            p[0] = mMax.x;
            p[1] = mMax.y;
            p[2] = mMax.z;
            mSSEMax = _mm_load_ps(p);
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
            __m128 t1 = _mm_add_ps(mSSEMin, r.mSSEOffset);
            __m128 t2 = _mm_add_ps(mSSEMax, r.mSSEOffset);

            t1 = _mm_mul_ps(t1, r.mSSEInvDir);
            t2 = _mm_mul_ps(t2, r.mSSEInvDir);

            __m128 _min = _mm_min_ps(t1, t2);
            __m128 _max = _mm_max_ps(t1, t2);

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