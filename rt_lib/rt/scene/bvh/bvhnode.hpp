#pragma once

#include <rt/scene/scene.hpp>

namespace rt
{
    class BVHNode
    {
    public:
        bool mIsLeaf;
        BVHNode* mLeft;
        BVHNode* mRight;

        BVHNode()
        {
            mIsLeaf = false;
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
        }
    };
}