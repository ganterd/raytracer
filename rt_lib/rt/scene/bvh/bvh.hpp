#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>
#include <rt/scene/bvh/bvhnode.hpp>
#include <rt/utils/timer.hpp>

#include <xmmintrin.h>
#include <emmintrin.h>

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

        bool cast(rt::Ray* r, rt::RayHit& hit);
        bool castDual(rt::BVHNode* n, rt::Ray* ray, rt::RayHit& hit);

        bool occluded(rt::Ray* ray, const float distance);
        bool occludedDual(BVHNode* n, rt::Ray* ray, const float distance);

    private:
        rt::BVH::Bin** mBins;
        void createBins();
        void freeBins();
        int deepestLevel;
        int numSplitNodes;
        int numLeafNodes;

        rt::AABB** totalAABBRight;
        rt::AABB** totalCentroidAABBRight;
        int** totalPrimitivesRight;

        BVHNode* mAllocatedNodes;
        size_t mCurrentlyAllocatedNodes;

        rt::BVHNode* recursiveConstruct(Tri** tris, int numTris, const AABB& centroidAABB, int level);

        BVHNode* newNode();
        rt::BVHBestSplit findBestSplit();
    };

    
}