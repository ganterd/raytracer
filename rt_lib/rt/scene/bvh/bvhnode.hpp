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

        BVHNode()
        {
            mIsLeaf = false;
            mAABB = rt::AABB::infinity();
        }
    };

    class BVHLeafNode : public BVHNode
    {
    public:
        static const char mMaxTris = 4;
        char mNumTris;
        Tri* mTris[mMaxTris];

        BVHLeafNode()
        {
            mNumTris = 0;
            mIsLeaf = true;
            mAABB = rt::AABB::infinity();
        }
    };
}