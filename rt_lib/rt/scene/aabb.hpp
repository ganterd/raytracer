#pragma once

#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>

#include <rt/scene/ray.hpp>

#include <xmmintrin.h>
#include <immintrin.h>

namespace rt
{
    class AABB
    {
    public:
        //alignas(4)glm::vec3 mMin;
        //alignas(4)glm::vec3 mMax;
        alignas(16)__m128 mMin;
        alignas(16)__m128 mMax;
        alignas(4)glm::vec3 mSize;

        AABB()
        {
            mMin = _mm_set1_ps(0.0f);
            mMax = _mm_set1_ps(0.0f);
            mSize = glm::vec3(0);
        }

        /**
         * Grow to accommodate point p
         */
        void grow(const glm::vec3& p)
        {
            __m128 SSEp = _mm_set_ps(0.0f, p.z, p.y, p.x);
            mMin = _mm_min_ps(mMin, SSEp);
            mMax = _mm_max_ps(mMax, SSEp);
            //mMin = glm::min(mMin, p);
            //mMax = glm::max(mMax, p);
            mSize.x = mMax[0] - mMin[0];
            mSize.y = mMax[1] - mMin[1];
            mSize.z = mMax[2] - mMin[2];
            //refreshSSE();
        }

        /**
         * Grow to accommodate another AABB
         */
        void grow(const AABB& other)
        {
            //mMin = glm::min(mMin, other.mMin);
            //mMax = glm::max(mMax, other.mMax);
            mMin = _mm_min_ps(mMin, other.mMin);
            mMax = _mm_max_ps(mMax, other.mMax);
            mSize.x = mMax[0] - mMin[0];
            mSize.y = mMax[1] - mMin[1];
            mSize.z = mMax[2] - mMin[2];
            //refreshSSE();
        }

        float surfaceArea()
        {
            return 2.0f * mSize.y * mSize.x + 2.0f * mSize.y * mSize.z + 2.0f * mSize.x * mSize.z;
        }


        static AABB infinity()
        {
            AABB newAABB;
            newAABB.mMin = _mm_set_ps(
                +std::numeric_limits<float>::infinity(),
                +std::numeric_limits<float>::infinity(),
                +std::numeric_limits<float>::infinity(),
                +std::numeric_limits<float>::infinity()
            );
            newAABB.mMax = _mm_set_ps(
                -std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity(),
                -std::numeric_limits<float>::infinity()
            );
            newAABB.mSize = glm::vec3(0.0f);
            return newAABB;
        }

        bool intersect(const rt::Ray& r) const
        {
            __m128 _tmin = _mm_mul_ps(_mm_sub_ps(mMin, r.mSSEOrigin), r.mSSEInvDir);
            __m128 _tmax = _mm_mul_ps(_mm_sub_ps(mMax, r.mSSEOrigin), r.mSSEInvDir);
            __m128 _min = _mm_min_ps(_tmin, _tmax);
            __m128 _max = _mm_max_ps(_tmin, _tmax);

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
            os << "[" << a.mMin[0] << "," << a.mMin[1] << "," << a.mMin[2] << "]->";
            os << "[" << a.mMax[0] << "," << a.mMax[1] << "," << a.mMax[2] << "]";
            return os;
        }
    };
}