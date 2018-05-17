#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>
#include <rt/scene/bvh/bvhnode.hpp>
#include <rt/utils/timer.hpp>

namespace rt
{
    class BVHBestSplit
    {
    public:
        int axis;
        int splitIndex;
        float cost;
        
        rt::AABB AABBLeft;
        rt::AABB AABBRight;
        rt::AABB centroidAABB_L;
        rt::AABB centroidAABB_R;
        int numPrimitives_L;
        int numPrimitives_R;

        BVHBestSplit()
        {
            cost = std::numeric_limits<float>::max();
        }
    };

    /**
     * Construction method is an implementation of Wald 2007 - "On fast construction 
     * of SAH-based bounding volume hierarchies", just not as efficient...
     */
    class BVH
    {
    public:
        struct Bin
        {
            std::vector<Tri*> mTris;
            rt::AABB mAABB;
            rt::AABB mCentroidsAABB;

            // Centroid bounds
            float mLeft;
            float mRight;

            Bin()
            {
                mAABB = rt::AABB::infinity();
                mCentroidsAABB = rt::AABB::infinity();
            }
        };

        int mNumBins;
        BVHNode* mRoot;
        rt::Scene* mTargetScene;

        BVH();
        ~BVH();

        void construct(rt::Scene* scene);

    private:
        Bin** mBins;
        void createBins();
        void freeBins();
        int deepestLevel;
        int numSplitNodes;
        int numLeafNodes;

        rt::BVHNode* recursiveConstruct(Tri** tris, int numTris, const AABB& centroidAABB, int level);

        
        rt::BVHBestSplit findBestSplit();
    };

    
}