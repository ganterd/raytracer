#pragma once

#include <rt/scene/scene.hpp>
#include <rt/scene/aabb.hpp>
#include <rt/scene/bvh/bvhnode.hpp>

namespace rt
{
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
            AABB mAABB;

            // Centroid bounds
            float mLeft;
            float mRight;

            Bin()
            {
                mAABB.mMin = glm::vec3(+std::numeric_limits<float>::infinity());
                mAABB.mMax = glm::vec3(-std::numeric_limits<float>::infinity());
            }
        };

        int mNumBins;
        BVHNode* mRoot;
        rt::Scene* mTargetScene;

        BVH();
        ~BVH();

        void construct(rt::Scene* scene);

    };

    
}