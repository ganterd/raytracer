#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>
#include <rt/scene/bvh/bvhnode.hpp>
#include <rt/utils/timer.hpp>
#include <rt/utils/export.hpp>

#include <xmmintrin.h>
#include <emmintrin.h>

namespace rt
{
    class bvh_best_split
    {
    public:
        int axis;
        int splitIndex;
        float cost;
        
        rt::aabb AABBLeft;
        rt::aabb AABBRight;
        rt::aabb centroidAABB_L;
        rt::aabb centroidAABB_R;
        int numPrimitives_L;
        int numPrimitives_R;

        bvh_best_split()
        {
            cost = std::numeric_limits<float>::max();
        }
    };

    /**
     * Construction method is an implementation of Wald 2007 - "On fast construction 
     * of SAH-based bounding volume hierarchies", just not as efficient...
     */
    class bvh
    {
    public:
        struct bin
        {
            std::vector<rt::tri*> mTris;
            rt::aabb mAABB;
            rt::aabb mCentroidsAABB;

            // Centroid bounds
            float mLeft;
            float mRight;

            bin()
            {
                mAABB = rt::aabb::infinity();
                mCentroidsAABB = rt::aabb::infinity();
            }
        };

        int mNumBins;
        rt::bvh_node *mRoot;
        rt::scene* mTargetScene;

        rt_lib bvh();
        rt_lib ~bvh();

        rt_lib void construct(rt::scene* scene);

        rt_lib rt::hit cast(const rt::ray &r);
        //bool occluded(rt::ray* ray, const float distance);

    private:
        rt::bvh::bin** mBins;
        void createBins();
        void freeBins();
        int deepestLevel;
        int numSplitNodes;
        int numLeafNodes;

        rt::aabb** totalAABBRight;
        rt::aabb** totalCentroidAABBRight;
        int** totalPrimitivesRight;

        bvh_node* mAllocatedNodes;
        size_t mCurrentlyAllocatedNodes;

        rt::bvh_node* recursiveConstruct(rt::tri** tris, int numTris, const aabb& centroidAABB, int level);

        bvh_node* newNode();
        rt::bvh_best_split findBestSplit();

        void cast(rt::bvh_node *n, const rt::ray &r, rt::hit &hit);
    };

    
}