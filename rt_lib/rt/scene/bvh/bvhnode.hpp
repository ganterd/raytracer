#pragma once

#include <rt/scene/scene.hpp>

namespace rt
{
    class BVHNode
    {
    public:
        bool mIsLeaf;
        BVHNode* mChildren[2];

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
            mIsLeaf = true;
        }
    };
}