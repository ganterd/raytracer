#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
    class BVHNode
    {
    public:
        bool mIsLeaf;
        BVHNode* mLeft;
        BVHNode* mRight;
        rt::AABB mAABB;

        static const char mMaxTris = 4;
        char mNumTris;
        Tri* mTris[mMaxTris];

        BVHNode()
        {
            mIsLeaf = false;
            mNumTris = 0;
            mAABB = rt::AABB::infinity();
        }
    };
}