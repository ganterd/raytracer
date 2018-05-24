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

        // |l.min.x|l.min.y|l.min.z|0.0f|l.max.x|l.max.y|l.max.z|0.0f|r.min.x|r.min.y|r.min.z|0.0f|r.max.x|r.max.y|r.max.z|0.0f|

        static const char mMaxTris = 8;
        char mNumTris = 0;
        Tri* mTris[mMaxTris];
    } BVHNode;
}