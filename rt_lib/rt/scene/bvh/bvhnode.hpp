#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
    typedef struct alignas(alignof(__m256)) _BVHNode
    {
        alignas(alignof(__m256)) rt::AABB mAABB = rt::AABB::infinity();
        bool mIsLeaf = false;
        struct _BVHNode* mLeft;
        struct _BVHNode* mRight;

        static const char mMaxTris = 4;
        char mNumTris = 0;
        Tri* mTris[mMaxTris];
    } BVHNode;
}