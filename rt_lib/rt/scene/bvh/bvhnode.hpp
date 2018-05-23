#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
    typedef struct _BVHNode
    {
        rt::AABB mAABB = rt::AABB::infinity();
        bool mIsLeaf = false;
        struct _BVHNode* mLeft;
        struct _BVHNode* mRight;

        static const char mMaxTris = 4;
        char mNumTris = 0;
        Tri* mTris[mMaxTris];
    } BVHNode;
}