#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
    typedef struct alignas(alignof(__m256)) _BVHNode
    {
        alignas(alignof(__m256)) __m256 mChildrenMinBounds;
        alignas(alignof(__m256)) __m256 mChildrenMaxBounds;
        
        alignas(alignof(__m128)) rt::AABB mAABB = rt::AABB::infinity();
        bool mIsLeaf = false;
        struct _BVHNode* mLeft;
        struct _BVHNode* mRight;

        static const char mMaxTris = 2;
        char mNumTris;
        Tri* mTris[mMaxTris];

        void intersectBothChildren(rt::Ray* ray, bool& left, bool& right) const
        {
            __m256 _tmin = _mm256_mul_ps(_mm256_sub_ps(mChildrenMinBounds, ray->mSSEDualOrigin), ray->mSSEDualInvDir);
            __m256 _tmax = _mm256_mul_ps(_mm256_sub_ps(mChildrenMaxBounds, ray->mSSEDualOrigin), ray->mSSEDualInvDir);
            __m256 _min = _mm256_min_ps(_tmin, _tmax);
            __m256 _max = _mm256_max_ps(_tmin, _tmax);

            float tmin = std::max(std::max(_min[0], _min[1]), _min[2]);
            float tmax = std::min(std::min(_max[0], _max[1]), _max[2]);
            left = (tmax >= tmin && tmax > 0);

            tmin = std::max(std::max(_min[4], _min[5]), _min[6]);
            tmax = std::min(std::min(_max[4], _max[5]), _max[6]);
            right = (tmax >= tmin && tmax > 0);
        }

        void AssignLeft(struct _BVHNode* n)
        {
            mLeft = n;
            mChildrenMinBounds = _mm256_insertf128_ps(mChildrenMinBounds, n->mAABB.mMin, 0);
            mChildrenMaxBounds = _mm256_insertf128_ps(mChildrenMaxBounds, n->mAABB.mMax, 0);
            mAABB.grow(n->mAABB);
        }

        void AssignRight(struct _BVHNode* n)
        {
            mRight = n;
            mChildrenMinBounds = _mm256_insertf128_ps(mChildrenMinBounds, n->mAABB.mMin, 1);
            mChildrenMaxBounds = _mm256_insertf128_ps(mChildrenMaxBounds, n->mAABB.mMax, 1);
            mAABB.grow(n->mAABB);
        }
    } BVHNode;
}