#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>

namespace rt
{
    class bvh_node
    {
    public:
        rt::aabb mAABB = rt::aabb::infinity();
        
        bool mIsLeaf = false;
        bvh_node *mLeft;
        bvh_node *mRight;

        static const char mMaxTris = 4;
        char mNumTris;
        rt::tri* mTris[mMaxTris];

        void AssignLeft(bvh_node* n)
        {
            mLeft = n;
            mAABB.grow(n->mAABB);
        }

        void AssignRight(bvh_node* n)
        {
            mRight = n;
            mAABB.grow(n->mAABB);
        }
    };
}